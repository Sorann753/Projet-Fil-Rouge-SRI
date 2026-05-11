#include <iomanip>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <sys/mman.h>

#include "libcamera/libcamera.h"

#include "pfr-core/include/utils/matrix.h"
#include "pfr-core/include/vision/image.h"

static std::shared_ptr<libcamera::Camera> camera;

/**
 * @brief RAII class to manage the mapped page
 */
struct MappedImgBuffer{
    MappedImgBuffer(int fd, std::size_t sz) : _size(sz)
    {
        data = mmap(NULL, sz, PROT_READ, MAP_SHARED, fd, 0);
        isValid = (data != MAP_FAILED);
    }
    ~MappedImgBuffer(){
        if(data != MAP_FAILED){
            munmap(data, _size);
        }
    }

    // move constructors
    MappedImgBuffer(MappedImgBuffer&& other) noexcept
    : data(other.data), _size(other._size)
    {
        other.data = nullptr; //we stole the data
        other._size = 0;
    }

    MappedImgBuffer& operator=(MappedImgBuffer&& other) noexcept {
        if(this != &other){
            if(data && data != MAP_FAILED){
                munmap(data, _size);
            }
            data = other.data;
            other.data = nullptr;

            _size = other._size;
            other._size = 0;
        }
        return *this;
    }

    // forbid the copy
    MappedImgBuffer(const MappedImgBuffer&) = delete;
    MappedImgBuffer& operator=(const MappedImgBuffer&) = delete;

    const uint8_t* getPixels() const {
        return static_cast<uint8_t*>(data);
    }

    bool isValid = false;
    void* data;
    std::size_t _size;
};

/**
 * @brief RAII class to manage the image ressource
 */
struct ImageOwner{
    ImageOwner() : img({nullptr, 0, 0, false})
    {}

    ImageOwner(std::size_t height, std::size_t width) : img(initMatrix(height, width, sizeof(Pixel)))
    {
        isValid = img.isValid;
    }
    ~ImageOwner(){
        freeMatrix(&img);
    }

    /**
     * @brief allow us to get pixels with simple image(x, y) access
     */
    Pixel& operator()(const std::size_t X, const std::size_t Y){
        Pixel* px = static_cast<Pixel*>(img.content);
        auto id = matrixGetIndex(&img, X, Y);
        return px[id];
    }

    Image img;
    bool isValid = false;
};

ImageOwner BufferToImage(const MappedImgBuffer& dataBuffer, std::size_t width, std::size_t heigh, std::size_t stride)
{
    // pixels is a 3D matrix here
    const uint8_t* pixels = dataBuffer.getPixels();
    ImageOwner newImage(heigh, width);

    // we're pretty much forced to make a copy there to have the proper type
    for(auto x = 0; x < width; x++){
        for(auto y = 0; y < heigh; y++){
            std::size_t id = (y * stride) + (x * 3);

            newImage(x, y).R = pixels[id + 0];
            newImage(x, y).G = pixels[id + 1];
            newImage(x, y).B = pixels[id + 2];
        }
    }

    return newImage;
}

class RpiCamera{
public:
    RpiCamera() {
        using namespace libcamera;
        cm->start();

        // list the existing camera for debug
        for (auto const &camera : cm->cameras()){
            std::cout << "[DEBUG] - " << camera->id() << std::endl;
        }

        auto cameras = cm->cameras();
        if (cameras.empty()) {
            std::cout << "[ERROR] - No cameras were found on the system" << std::endl;
            return;
        }
    
        std::string cameraId = cameras[0]->id();
        camera = cm->get(cameraId);
        camera->acquire();
    
        auto config = camera->generateConfiguration( { StreamRole::Viewfinder } );
        if(!config){
            std::cout << "[ERROR] - Can't generate the configuration for some reason" << std::endl;
            return;
        }

        StreamConfiguration &streamConfig = config->at(0);
        streamConfig.size = {640, 480};

        std::cout << "[INFO] - Default viewfinder configuration is: " << streamConfig.toString() << std::endl;
        std::cout << "[INFO] - frame size is " << streamConfig.frameSize << std::endl;
        std::cout << "[INFO] - buffer count = " << streamConfig.bufferCount << std::endl;

        if(config->validate() != CameraConfiguration::Status::Valid){
            std::cout << "[ERROR] - Invalid Configuration" << std::endl;
            return;
        }
        std::cout << "[INFO] - Validated viewfinder configuration is: " << streamConfig.toString() << std::endl;
        camera->configure(config.get());

        allocator = std::make_unique<FrameBufferAllocator>(camera);
    
        for (StreamConfiguration &cfg : *config) {
            int ret = allocator->allocate(cfg.stream());
            if (ret < 0) {
                std::cout << "[CRITICAL] - NO MORE MEMORY AVAILABLE" << std::endl;
                return;
            }
    
            size_t allocated = allocator->buffers(cfg.stream()).size();
            std::cout << "[INFO] - Allocated " << allocated << " buffers for stream" << std::endl;
        }

        stream = streamConfig.stream();
        const std::vector<std::unique_ptr<FrameBuffer>> &buffers = allocator->buffers(stream);
    
        for (std::size_t i = 0; i < buffers.size(); i++) {
            std::unique_ptr<Request> request = camera->createRequest();
            if (!request)
            {
                std::cout << "[CRITICAL] - NO MORE MEMORY AVAILABLE" << std::endl;
                return;
            }
    
            const std::unique_ptr<FrameBuffer> &buffer = buffers[i];
            int ret = request->addBuffer(stream, buffer.get());
            if (ret < 0)
            {
                std::cout << "[ERROR] - Can't set buffer for request" << std::endl;
                std::cout << "[ERROR] - failed with code " << ret << std::endl;
                return;
            }
    
            requests.push_back(std::move(request));
        }

        camera->requestCompleted.connect(RpiCamera::requestComplete);

        camera->start();
        for (std::unique_ptr<Request> &request : requests){
            camera->queueRequest(request.get());
        }

        std::cout << "[SUCCESS] - Camera connected" << std::endl;
        isValid = true;
    }

    ~RpiCamera(){
        camera->stop();
        camera->release();
        if(stream != nullptr){
            allocator->free(stream);
            allocator.reset();
        }
        camera.reset();
        cm->stop();
    }

    ImageOwner consumeImage(){
        if(!imgReady.isValid){
            // return an empty image because there is nothing to get
            return ImageOwner();
        }

        ImageOwner img = BufferToImage(imgReady, readyConf.size.width, readyConf.size.height, readyConf.stride);
        return img;
    }

    static void requestComplete(libcamera::Request* request){
        std::cout << "[!!!]" << std::endl;
        using namespace libcamera;
        if (request->status() == Request::RequestCancelled){
            return;
        }

        // extract the buffer
        auto [stream, buffer] = *(request->buffers().begin());
        int fd = buffer->planes()[0].fd.get();
        std::size_t size = buffer->planes()[0].length;

        MappedImgBuffer content(fd, size);
        if(!content.isValid) return;

        imgReady = std::move(content);
        readyConf = stream->configuration();

        // keep the camera going
        request->reuse(Request::ReuseBuffers);
        camera->queueRequest(request);
    }

private:
    std::unique_ptr<libcamera::CameraManager> cm = std::make_unique<libcamera::CameraManager>();
    std::unique_ptr<libcamera::FrameBufferAllocator> allocator;
    libcamera::Stream* stream = nullptr;
    std::vector<std::unique_ptr<libcamera::Request>> requests;

    //TODO : make thread safe
    static MappedImgBuffer imgReady;
    static libcamera::StreamConfiguration readyConf;

    bool isValid = false;
};



int main(){
    using namespace std::chrono_literals;
    RpiCamera rpicam;

    std::cout << "a mimir" << std::endl;
    std::this_thread::sleep_for(100ms);   

    ImageOwner img = rpicam.consumeImage();

    std::cout << "[INFO] first pixel color : " << img(0, 0).R << "-" << img(0, 0).G << "-" << img(0, 0).B << std::endl;
    
    std::cout << "end" << std::endl;
    return 0;
}
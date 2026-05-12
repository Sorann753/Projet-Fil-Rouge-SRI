#include <iomanip>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>

#include <sys/mman.h>

#include "libcamera/libcamera.h"

#include "pfr-core/include/utils/matrix.h"
#include "pfr-core/include/vision/image.h"

static std::shared_ptr<libcamera::Camera> camera;

/**
 * @brief RAII class to manage the mapped page
 */
struct MappedImgBuffer{
    MappedImgBuffer() = default;

    MappedImgBuffer(int fd, std::size_t sz) : _size(sz)
    {
        data = mmap(NULL, sz, PROT_READ, MAP_SHARED, fd, 0);
        isValid = (data != MAP_FAILED);
    }
    ~MappedImgBuffer(){
        if(data != MAP_FAILED){
            // std::cout << "[DEBUG] - free img buffer" << std::endl;
            munmap(data, _size);
        }
    }

    // move constructors
    MappedImgBuffer(MappedImgBuffer&& other) noexcept
    : data(other.data), _size(other._size), isValid(other.isValid)
    {
        other.data = 0; //we stole the data
        other._size = 0;
        other.isValid = false;
    }

    MappedImgBuffer& operator=(MappedImgBuffer&& other) noexcept {
        if(this != &other){
            if(data && data != MAP_FAILED){
                munmap(data, _size);
            }
            data = other.data;
            other.data = 0;

            _size = other._size;
            other._size = 0;

            isValid = other.isValid;
            other.isValid = false;
        }
        return *this;
    }

    // forbid the copy
    MappedImgBuffer(const MappedImgBuffer&) = delete;
    MappedImgBuffer& operator=(const MappedImgBuffer&) = delete;

    const uint8_t* getPixels() const {
        if(!isValid) return nullptr;

        return static_cast<uint8_t*>(data);
    }

    bool isValid = false;
    void* data = nullptr;
    std::size_t _size = 0;
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
        // std::cout << "[DEBUG] free matrix" << std::endl;
        freeMatrix(&img);
    }

    ImageOwner(ImageOwner&& other) : img({nullptr, other.img.lines, other.img.columns, other.isValid})
    {
        img.content = other.img.content;
        other.img.content = 0;

        this->isValid = other.isValid;
        other.isValid = false;
    }

    ImageOwner& operator=(ImageOwner&& other){
        if(this != &other){
            if(img.isValid){
                freeMatrix(&img);
            }

            img.columns = other.img.columns;
            img.lines = other.img.lines;

            img.content = other.img.content;
            other.img.content = 0;

            img.isValid = other.isValid;
            this->isValid = other.isValid;
            other.isValid = false;
        }
        return *this;
    }

    // forbid the copy
    ImageOwner(const ImageOwner&) = delete;
    ImageOwner& operator=(const ImageOwner&) = delete;

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
    if(pixels == nullptr){
        return ImageOwner();
    }
    
    ImageOwner newImage(heigh, width);
    if(!newImage.isValid){
        std::cout << "[DEBUG] image invalid at creation???" << std::endl;
    }

    std::cout << "[INFO] - buffer : " << width << " " << heigh << " " << stride << std::endl;
    std::cout << "[INFO] - new image : " << newImage.img.columns << " " << newImage.img.lines << " " << newImage.isValid << std::endl;

    // we're pretty much forced to make a copy there to have the proper type
    for(auto y = 0; y < heigh; y++){
        for(auto x = 0; x < width; x++){
            std::size_t id = (y * stride) + (x * 4);
            assert(id < dataBuffer._size);

            newImage(x, y).B = pixels[id + 0];
            newImage(x, y).G = pixels[id + 1];
            newImage(x, y).R = pixels[id + 2];
        }
    }

    // std::cout << "[DEBUG] image validity : " << newImage.isValid << std::endl;

    // std::cout << "[DEBUG] - buffer converted" << std::endl;

    return newImage;
}

class RpiCamera{
public:
    RpiCamera() {
        using namespace libcamera;
        cm->start();

        // list the existing camera for debug
        for (auto const &camera : cm->cameras()){
            // std::cout << "[DEBUG] - " << camera->id() << std::endl;
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
        // std::cout << "[DEBUG] stopping the camera" << std::endl;
        camera->stop();
        camera->release();
        if(stream != nullptr){
            allocator->free(stream);
            allocator.reset();
        }
        camera.reset();
        cm->stop();
        // std::cout << "[DEBUG] camera terminated" << std::endl;
    }

    ImageOwner consumeImage(){
        if(!imgReady.isValid){
            // return an empty image because there is nothing to get
            return ImageOwner();
        }

        std::lock_guard<std::mutex> lock(imgLock);
        return BufferToImage(imgReady, readyConf.size.width, readyConf.size.height, readyConf.stride);
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
        std::cout << "buffer offset : " << buffer->planes()[0].offset << std::endl;

        MappedImgBuffer content(fd, size);
        if(!content.isValid) return;

        { // atomic operation
            std::lock_guard<std::mutex> lock(imgLock);
            // std::cout << "[DEBUG] atomic in" << std::endl;
            imgReady = std::move(content);
            std::cout << "(*)" << '\n';
            readyConf = stream->configuration();
            // std::cout << "[DEBUG] atomic out" << std::endl;
        }

        // keep the camera going
        request->reuse(Request::ReuseBuffers);
        camera->queueRequest(request);
    }

private:
    std::unique_ptr<libcamera::CameraManager> cm = std::make_unique<libcamera::CameraManager>();
    std::unique_ptr<libcamera::FrameBufferAllocator> allocator;
    libcamera::Stream* stream = nullptr;
    std::vector<std::unique_ptr<libcamera::Request>> requests;

    inline static std::mutex imgLock;
    inline static MappedImgBuffer imgReady;
    inline static libcamera::StreamConfiguration readyConf;

    bool isValid = false;
};



int main(){
    using namespace std::chrono_literals;
    RpiCamera rpicam;

    std::cout << "a mimir" << std::endl;
    std::this_thread::sleep_for(3000ms);   

    ImageOwner img (rpicam.consumeImage());
    // std::cout << "[DEBUG] - image consumed" << std::endl;
    // std::cout << "[DEBUG] - image validity outside : " << img.isValid << std::endl;

    if(!img.isValid){
        std::cout << "[ERROR] the image was invalid" << std::endl;
    }
    else{
        std::cout << "[INFO] first pixel color : " << (int)(img(0, 0).R) << "-" << (int)(img(0, 0).G) << "-" << (int)(img(0, 0).B) << std::endl;

        std::cout << "[SUCCESS]" << std::endl;
    }
    
    std::cout << "end" << std::endl;
    return 0;
}
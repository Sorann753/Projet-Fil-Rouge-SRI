#include <iomanip>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "libcamera/libcamera.h"

static std::shared_ptr<libcamera::Camera> camera;

class RpiCamera{
public:
    RpiCamera() : cm{std::make_unique<libcamera::CameraManager>()}
    {
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
    
        for (unsigned int i = 0; i < buffers.size(); ++i) {
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

    int requestImage(){
        //TODO
        return 0;
    }

    static void requestComplete(libcamera::Request* request){
        std::cout << "!!!" << std::endl;
        using namespace libcamera;
        if (request->status() == Request::RequestCancelled){
           return;
        }

        const std::map<const Stream*, FrameBuffer*>& buffers = request->buffers();
        for (auto bufferPair : buffers) {
            // actually process the images

            FrameBuffer *buffer = bufferPair.second;
            const FrameMetadata &metadata = buffer->metadata();
        
            std::cout << " seq: " << std::setw(6) << std::setfill('0') << metadata.sequence << " bytesused: ";

            unsigned int nplane = 0;
            for (const FrameMetadata::Plane &plane : metadata.planes())
            {
                std::cout << plane.bytesused;
                if (++nplane < metadata.planes().size()) std::cout << "/";
            }
            
            std::cout << std::endl;
        } 

        //request->reuse(Request::ReuseBuffers);
        //camera->queueRequest(request);
    }

private:
    std::unique_ptr<libcamera::CameraManager> cm;
    std::unique_ptr<libcamera::FrameBufferAllocator> allocator;
    libcamera::Stream* stream = nullptr;
    std::vector<std::unique_ptr<libcamera::Request>> requests;

    bool isValid = false;
};



int main(){
    using namespace std::chrono_literals;
    RpiCamera rpicam;

    int img = rpicam.requestImage();
    std::cout << "a mimir" << std::endl;
    std::this_thread::sleep_for(3000ms);   
    std::cout << "end" << std::endl;

    return 0;
}
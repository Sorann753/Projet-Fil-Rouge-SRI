#include <iomanip>
#include <iostream>
#include <memory>
#include <chrono>

#include "libcamera/libcamera.h"

static std::shared_ptr<libcamera::Camera> camera;

int main(){
    using namespace libcamera; //to remove later
    using namespace std::chrono_literals;

    std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();
    cm->start();

    for (auto const &camera : cm->cameras()){
        std::cout << camera->id() << std::endl;
    }

    auto cameras = cm->cameras();
    if (cameras.empty()) {
        std::cout << "No cameras were identified on the system."
                << std::endl;
        cm->stop();
        return -1;
    }

    std::string cameraId = cameras[0]->id();

    camera = cm->get(cameraId);

    camera->acquire();

    auto config = camera->generateConfiguration( { StreamRole::Viewfinder } );
    if(!config){
        camera->release();
        camera.reset();
        cm->stop();
        return -1;
    }

    StreamConfiguration &streamConfig = config->at(0);
    streamConfig.size = {640, 480};

    std::cout << "Default viewfinder configuration is: " << streamConfig.toString() << std::endl;
    std::cout << "[INFO] - frame size is " << streamConfig.frameSize << std::endl;
    std::cout << "[INFO] - buffer count = " << streamConfig.bufferCount << std::endl;

    if(config->validate() != CameraConfiguration::Status::Valid){
        camera->release();
        camera.reset();
        cm->stop();
        return -2;
    }
    std::cout << "Validated viewfinder configuration is: " << streamConfig.toString() << std::endl;
    camera->configure(config.get());

    auto allocator = std::make_unique<FrameBufferAllocator>(camera);

    for (StreamConfiguration &cfg : *config) {
        int ret = allocator->allocate(cfg.stream());
        if (ret < 0) {
            std::cerr << "Can't allocate buffers" << std::endl;
            return -ENOMEM;
        }

        size_t allocated = allocator->buffers(cfg.stream()).size();
        std::cout << "Allocated " << allocated << " buffers for stream" << std::endl;
    }

    Stream *stream = streamConfig.stream();
    const std::vector<std::unique_ptr<FrameBuffer>> &buffers = allocator->buffers(stream);
    std::vector<std::unique_ptr<Request>> requests;

    for (unsigned int i = 0; i < buffers.size(); ++i) {
        std::unique_ptr<Request> request = camera->createRequest();
        if (!request)
        {
            std::cerr << "Can't create request" << std::endl;
            return -ENOMEM;
        }

        const std::unique_ptr<FrameBuffer> &buffer = buffers[i];
        int ret = request->addBuffer(stream, buffer.get());
        if (ret < 0)
        {
            std::cerr << "Can't set buffer for request"
                << std::endl;
            return ret;
        }

        requests.push_back(std::move(request));
    }


    // cleanup
    camera->stop();
    allocator->free(stream);
    camera->release();
    camera.reset();
    cm->stop();

    return 0;
}
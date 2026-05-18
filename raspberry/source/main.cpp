#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>

static bool programStop = false;
static std::mutex arduinoMtx;

#include "utils/position.h"
#include "BiChannel.hpp"

#include "cartography/lidar.h"
#include "cartography/cartography.h"
#include "history/history.h"

#include "controller/arduinoController.hpp"
#include "controller/pilot.hpp"
#include "network/TcpServer.hpp"
// #include "sensor/WorldModel.hpp"



/** @note the following blocks have been put as comments
 * because they are unfinished and i tried getting
 * the minimum working for the presentation */

// // temporary just to silence the errors
// namespace libcamera{
//     class CameraManager{};
//     class FrameBufferAllocator{};
//     class Stream;
//     class Request;
// };

// class RpiCamera{
// public:
//     RpiCamera();
//     ~RpiCamera();
//     int getImage();
//     static void requestComplete(libcamera::Request* request);

// private:
//     std::unique_ptr<libcamera::CameraManager> cm = std::make_unique<libcamera::CameraManager>();
//     std::unique_ptr<libcamera::FrameBufferAllocator> allocator;
//     libcamera::Stream* stream = nullptr;
//     std::vector<std::unique_ptr<libcamera::Request>> requests;

//     bool isValid = false;
// };

// class Lidar {
// public: 
//     Lidar(){
//         lidar_start();
//     }

//     ~Lidar(){
//         lidar_close();
//     }

//     //TODO : encapsulate the lidar_update_scan
//     int scan();
// };

class History {
public:
    History(){
        history_init();
    }

    ~History(){
        history_close();
    }

    void log(HistoryLevel level, std::string msg){
        history_log(level, msg.c_str());
    }
};


void sensorTask(BiChannel<int, int>& sensorChan, ArduinoController& arduino){
    //! None of the main sensor are working yet
    // start the sensors
    // RpiCamera camera;
    // Lidar rpLidar;
    
    //init the map
    // WorldModel model;

    // pull data
    // camera.getImage();
    // rpLidar.scan();

    while(!programStop){
        std::pair<RobotData, bool> dat;
        { // atomic
            std::lock_guard<std::mutex> lock(arduinoMtx);
            dat = arduino.getArduinoData();
        }

        // update the world model
        if(dat.second && sensorChan.outCount() < 10){
            std::cout << "[DEBUG] pushed data : " << dat.first.distAv << std::endl;
            sensorChan.pushOut(dat.first.distAv);
        }
    }
}
void networkTask(BiChannel<int, int>& netChan){
    // start server
    TcpServer net(PORT);
    net.listen();
}
void controlTask(BiChannel<int, int>& netChan, BiChannel<int, int>& sensorChan, ArduinoController& hardware){
    // connect to arduino
    Pilot pilot {AutoPilot(hardware)};

    while(!programStop){
        pilot.readModel(sensorChan);
        pilot.readOrders(netChan);

        std::lock_guard<std::mutex> lock(arduinoMtx);
        std::cout << "[DEBUG] step" << std::endl;
        pilot.step();
    }
}


int main() {
    using namespace std::chrono_literals;

    /** @note the types for the network request/response and for the world model
     * have not been finished yet, those int are placeholders */
    BiChannel<int, int> networkChan;
    BiChannel<int, int> sensorChan;

    ArduinoController arduino;

    auto sensorProcess = std::thread(&sensorTask, std::ref(sensorChan), std::ref(arduino));
    auto networkProcess = std::thread(&networkTask, std::ref(networkChan));
    auto controlProcess = std::thread(&controlTask, std::ref(networkChan), std::ref(sensorChan), std::ref(arduino));

    // for testing purposes
    std::this_thread::sleep_for(3000ms); 
    programStop = true;

    networkProcess.join();
    sensorProcess.join();
    controlProcess.join();

    return 0;
}

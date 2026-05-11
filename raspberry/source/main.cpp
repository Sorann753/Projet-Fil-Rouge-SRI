#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

#include "utils/position.h"
#include "BiChannel.hpp"

#include "communication/tcp_server.h"
#include "cartography/lidar.h"
#include "cartography/cartography.h"
#include "history/history.h"

#include "controller/arduinoController.hpp"
#include "controller/pilot.hpp"


class Objects{};

class WorldMap{
public:
    void update(PolarCoordinate* raw, size_t pointCount){

    }
private:
};

struct WorldModel{
    std::vector<Objects> visibleObjects;
    WorldMap map;
    RobotPosition pos;
};

class TcpServer{
public:
    TcpServer(int port){
        server_socket = init_tcp_server(port);
    }

    /**
     * @brief start listening to all transmissions
     */
    void listen(){
        using namespace std::chrono_literals;
        // while server open
        while(serverOpen){
            // -> listen for connection
            if(!client_socket){
                client_socket = accept_client(server_socket);
                connectionUsed = true;
            } 
            /** @post a client is connected */

            //   -> ping (once in a while)
            uint16_t tick = 1;
            if(tick % 50 == 0){
                ping();

                if(tick > 60'000) tick = 1;
            }

            //   -> if received request from GUI -> answer request and/or push orders to the controlTask
            int nbOctet = read_message(client_socket, nullptr, 0);
            if(nbOctet > 0){
                handleRequest("");
            }

            if(connectionUsed){ // if we're still connected
                //   -> send model update to GUI
                sendModel(); // whatever that does
            }
            
            tick++;
            // let other thread do work cuz next message will likely take time to get there
            std::this_thread::sleep_for(10ms);
        }
    }

    ~TcpServer(){
        if(connectionUsed){
            shutdown(client_socket, SHUT_RDWR);
            close(client_socket);
        }
        if(serverOpen){
            close(server_socket);
        }
    }

private:
    void ping(){

    }

    void handleRequest(std::string request){
        //fr
    }

    void sendModel(){

    }

    int client_socket;
    int server_socket;

    bool connectionUsed = false;
    bool serverOpen = false;
};


// temporary just to silence the errors
namespace libcamera{
    class CameraManager{};
    class FrameBufferAllocator{};
    class Stream;
    class Request;
};

class RpiCamera{
public:
    RpiCamera();
    ~RpiCamera();
    int getImage();
    static void requestComplete(libcamera::Request* request);

private:
    std::unique_ptr<libcamera::CameraManager> cm = std::make_unique<libcamera::CameraManager>();
    std::unique_ptr<libcamera::FrameBufferAllocator> allocator;
    libcamera::Stream* stream = nullptr;
    std::vector<std::unique_ptr<libcamera::Request>> requests;

    bool isValid = false;
};

class Lidar {
public: 
    Lidar(){
        lidar_start();
    }

    ~Lidar(){
        lidar_close();
    }

    //TODO : encapsulate the lidar_update_scan
    int scan();
};

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



void sensorTask(){
    // start the sensors
    RpiCamera camera;
    Lidar rpLidar;
    
    //init the map
    WorldModel model;

    // pull data
    camera.getImage();
    rpLidar.scan();

    // image will get automatically pushed to a buffer by the camera thread
    // probably need a class for the image processing which update the WorlMap
    
    // update the world model
    PolarCoordinate* raw;
    model.map.update(raw, 10);
}
void networkTask(){
    // start server
    TcpServer net(PORT);
    net.listen();
}
void controlTask(BiChannel<int, int>& netChan, BiChannel<int, int>& sensorChan){
    // connect to arduino
    ArduinoController arduino;
    Pilot pilot {ManualPilot(arduino)};

    while(true){
        pilot.readModel(sensorChan);
        pilot.readOrders(netChan);
        pilot.step();
    }
}

int main() {
    BiChannel<int, int> networkChan;
    BiChannel<int, int> sensorChan;

    auto networkProcess = std::thread(&networkTask, networkChan, sensorChan);
    auto sensorProcess = std::thread(&sensorTask, sensorChan);
    auto controlProcess = std::thread(&controlTask, networkChan, sensorChan);

    networkProcess.join();
    sensorProcess.join();
    controlProcess.join();

    return 0;
}

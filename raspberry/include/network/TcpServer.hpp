#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <chrono>
#include <thread>
#include <string>

#include "communication/tcp_server.h"

extern bool programStop;

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

            if(programStop){
                if(connectionUsed){
                    shutdown(client_socket, SHUT_RDWR);
                    close(client_socket);
                    connectionUsed = false;
                }
                if(serverOpen){
                    close(server_socket);
                    serverOpen = false;
                }
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

#endif // TCP_SERVER_HPP

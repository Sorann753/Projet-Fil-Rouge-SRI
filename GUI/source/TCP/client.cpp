#include"TCP/client.hpp"
//#include "TCP/tcp_server.hpp"

Client::Client(){

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return;
    }
}

Client::~Client(){
}

void Client::connection(int sock){
        // Step 2: Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        fl_alert("connection failed or already connected");
        return;
    }
    const char* message = "Hello, server!";
    send(sock, message, strlen(message), 0);
    fl_message("Connected");

}

auto Client::readData(){
    if (listen(0, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
    
}
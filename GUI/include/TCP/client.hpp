#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <FL/fl_ask.H>

class Client
{
private:
    int port = 5000;
    sockaddr_in addr;
    struct sockaddr_in serv_addr;
public:
    int sock = 0;
    Client();
    ~Client();
    void connection(int sock);
    auto readData();

};





#endif
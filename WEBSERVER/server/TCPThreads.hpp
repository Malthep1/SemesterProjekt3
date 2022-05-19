#pragma once

#include "osapi/linux/ThreadFunctor.hpp"
#include "osapi/linux/Thread.hpp"
#include "string.h"  // For memset !??
#include "unistd.h"  // read/write 
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "Exceptions.hpp"
#include "ServTCP.hpp"


using osapi::ThreadFunctor;

class ServTCP;

struct ConnData
{
    struct sockaddr_in outAddress;
    struct sockaddr_in thisAddress;
};


class ListenerThread : public ThreadFunctor
{
public:
    ListenerThread(int port, const char* servIP, ServTCP* s);

private:
    void run();
    ServTCP* server;
    int setupListener();
    ConnData cData;
    int socketId = 0;
};

class ConnectionThread : public ThreadFunctor
{
public:
    ConnectionThread(int tid, ServTCP* s);
private:
    void run();
    std::string receiveMsg();
    int handleMessage(std::string msg);
    int sendMsg(std::string msg);

    int id;
    ServTCP* server;
    char buffer[256];
};


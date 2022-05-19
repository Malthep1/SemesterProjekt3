#pragma once

/*
* Marcin Szymanek (marcinwszymanek@gmail.com)
*
* Implementation of TCP connection threads in FoodgiverService
*
* 5/13/2022 
* v. 0.1
*
* Initial commit
*
* 5/20/2022
* v 0.2
* 
* Implemented parsing the message received from client
* TODO: Send correct messages to server after a request has been received and parsed in appropriate handler
*
*/

#include "osapi/linux/ThreadFunctor.hpp"
#include "osapi/linux/Thread.hpp"
#include "osapi/MsgQueue.hpp"
#include "string.h"  // For memset !??
#include <cstring>   // c_str()
#include <iostream> 
#include "unistd.h"  // read/write from file/socket 
#include "netinet/in.h" 
#include "sys/socket.h"
#include "arpa/inet.h"
#include "Exceptions.hpp"
#include "ServTCP.hpp"

#define AUTH "auth"
#define TREAT "treat"
#define INFO "info"

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
    ListenerThread(int port, ServTCP* s);

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
    osapi::MsgQueue* getMsgQueue(){return &connMQueue;}
private:
    void run();
    std::string receiveMsg();
    int handleMessage(std::string msg);
    int handleAuthorization();
    int handleTreatRequest(int id);
    int handleInfoSendRequest(int id);
    int sendMsg(std::string msg);

    int id;
    ServTCP* server;
    osapi::MsgQueue connMQueue;
    const std::string auth = AUTH;
    const std::string treat = TREAT;
    const std::string info = INFO;
    char buffer[256];
};


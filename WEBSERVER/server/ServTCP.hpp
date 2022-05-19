#pragma once

#include "TCPThreads.hpp"
#include "osapi/MsgQueue.hpp"
#include <sys/socket.h>
#include <stdio.h>
#include <vector>

using namespace osapi;
using std::vector;

#define PORT 9000
#define IP "10.0.0.10"


struct MsgNewCon : osapi::Message{
    int fd;
};

class ServTCP : public ThreadFunctor{

    public:
        ServTCP() : tcpMq(10){
            printf("Allocating vectors...\n");
            threadPool = new vector<Thread*>(10);
            openSockets = new vector<int>(10);
        };
        ~ServTCP(){
            delete threadPool;
            delete openSockets;
        }
        enum{
            MID_TCP_SOCK_NEW,
            MID_TCP_SOCK_MSG,
            MID_WS_MSG
        };
        MsgQueue* getMsgQueue();
    private:
        void run();
        int handleMsg(unsigned long id, Message* msg);
        int createListener();
        int handleConnection(int fd);
        int startPort = PORT;
    
        vector<Thread*> *threadPool;
        
        MsgQueue tcpMq;
        vector<int> *openSockets;

};

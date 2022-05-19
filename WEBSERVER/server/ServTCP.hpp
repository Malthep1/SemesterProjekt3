#pragma once

/*
* Marcin Szymanek (marcinwszymanek@gmail.com)
*
* Implementation of TCP server in FoodgiverService
* Handles creating new connections, managing threads, user authentication and communication with
* WebSocket side of the server (mobile phone)
*
* 5/10/2022 
* v. 0.1
*
* Initial commit
*
* 5/20/2022
* v 0.2
* 
* Added prototype handlers and prototype Authenticator class. Cleaned up the code a bit.
* Fixed a bug related to VM IP
* TODO:
* - Add message handling when appropriate message has been received (just debug print msgs to start with)
* - Garbage allocation (delete threads when finished)
*
*/

#include "TCPThreads.hpp"
#include "osapi/MsgQueue.hpp"
#include <sys/socket.h>
#include <stdio.h>
#include <vector>
#include <map>

using namespace osapi;
using std::vector;

#define PORT 9000
#define AUTH_FILENAME "auth.txt"

// Forward declaration
class ConnectionThread;

struct MsgNewCon : osapi::Message{
    int fd;
};

struct MsgConAuth : osapi::Message{
    ConnectionThread* cthread;
};

struct MsgConTreat : osapi::Message{
    int userId;
    ConnectionThread* cthread;
};

struct MsgConInfo : osapi::Message{
    int userId;
};

struct MsgConFinished : osapi::Message{
    ConnectionThread* cthread;
};

class ServTCP : public ThreadFunctor{

    public:
        //class Authenticator;

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
            MID_TCP_CONN_TREAT,
            MID_TCP_CONN_INFO,
            MID_WS_MSG
        };
        MsgQueue* getMsgQueue();
        // Authenticator m_auth;

    private:
        class Authenticator{
        public:
            Authenticator();
            bool addNewUser(int id);
            int findUser(int id);
        private:
            std::string authFileName = AUTH_FILENAME;
            bool readFile();
            bool appendToFile();
        };

        void run();
        int handleMsg(unsigned long id, Message* msg);
        int createListener();
        int handleConnection(int fd);
        int handleAuthRequest();
        int handleTreatRequest();
        int handleSendInfoReq();
        int startPort = PORT;
    
        vector<Thread*> *threadPool;
        
        MsgQueue tcpMq;
        vector<int> *openSockets;

};

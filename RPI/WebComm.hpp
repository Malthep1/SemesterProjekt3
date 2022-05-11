#pragma once

/* v.01
*
* Added class function definitions
*
*/

#include <vector>
#include <osapi/linux/Thread.hpp>
#include <osapi/linux/ThreadFunctor.hpp>
#include <osapi/MsgQueue.hpp>
#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<stdlib.h>	//malloc
#include<sys/socket.h>	//you know what this is for
#include<arpa/inet.h>	//inet_addr , inet_ntoa , ntohs etc
#include<netinet/in.h>
#include<unistd.h>	//getpid
#include <netdb.h>
#include <errno.h>
//#include "findFgDNS.h"

#define SERVER_NAME "foodgiver.hopto.org"

using osapi::Thread;
using osapi::MsgQueue;
using std::vector;


class WebComm{
public:
    WebComm(bool auth = false) : connected(false) {
        authorized = auth;
    }
    // Garbage allocation. Must be called from SysCtl after a response has been received
    void shutdownConnection(){
        delete openConnection;
        openConnection = nullptr;
    }

     Thread* create_new_connection(MsgQueue* scQueue, int req_type);

private:

    // This is the implementation of the connection thread
    class connThread : public osapi::ThreadFunctor{
    public:
        connThread(MsgQueue* scQueue, int req_type) : scQueue_(scQueue){
            //serv_IP = ngethostbyname(serverName, T_A);
            // serv_IP = gethostbyname(SERVER_NAME);
            serv_IP = " ";
        }
        // Two types of requests can be sent, chosen at run()
        enum REQ_TYPE{
            REQ_TREAT,
            REQ_SEND_INFO
        };
    private:
        void run();
        void send_req_info();
        void send_req_treat();
        int setup_connection();

        // Used to find server ip - we use DDNS so the ip is not static, but the domain name is
        int get_fgs_ip();
        int socket_fd;
        char* serv_IP;
        
        unsigned char serverName[100] = SERVER_NAME;
        // Used to send the reply from server back to SystemController thread
        MsgQueue* scQueue_;
    };

    bool connected;
    // Tells whether the device has been authorized by the server
    bool authorized;
    // Creates a new thread to connect to server

    // Keep track of an open connection
    // Multiple connections possible but unnecessary?
    Thread* openConnection = nullptr;
    
    
};

#pragma once

/* v.01
*
* Added class function definitions
* 
* v.02
* 
* Implemented primary control interface
* Implemented automatically getting IP to "foodgiver.hopto.org"
* Implemented sending a tcp message 
* Facilitates sending Msg via MsgQueue, but that will be implemented after SystemController has a mq
* Facilitates implementing different request types (send treat, send info, + auth?)
* Commented all functions
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

#define SERVER_NAME "foodgiver.hopto.org"
#define S_RESP_ACK "ack"
#define S_RESP_NACK "nack"

using osapi::Thread;
using osapi::MsgQueue;
using std::vector;


class WebComm{
public:
    enum REQ_TYPE{
            REQ_AUTH,
            REQ_TREAT,
            REQ_SEND_INFO
    };
    WebComm(bool auth = false) : connected(false) {
        authorized = auth;
    }

    Thread* create_new_connection(MsgQueue* scQueue, REQ_TYPE req_type);
    // Garbage allocation. Must be called from SysCtl after a response has been received
    void shutdownConnection(){
        delete openConnection;
        openConnection = nullptr;
    }

private:

    // This is the implementation of the connection thread
    class connThread : public osapi::ThreadFunctor{
    public:
        // Three types of requests can be sent, chosen at run()
        connThread(MsgQueue* scQueue, REQ_TYPE req_type) : scQueue_(scQueue), type_(req_type){}
        
    private:
        void run();
        int send_auth();
        int send_req_info();
        int send_req_treat();
        int setup_connection();

        // Used to find server ip - we use DDNS so the ip is not static, but the domain name is
        int get_fgs_ip();
        // Socket file descriptor
        int socket_fd;
        char serv_IP[20];
        const std::string m_ack = S_RESP_ACK;
        const std::string m_nack = S_RESP_NACK;
        REQ_TYPE type_;
        // Used to send the reply from server back to SystemController thread
        MsgQueue* scQueue_;
    };

    // This is probably unnecessary
    bool connected;
    // Tells whether the device has been authorized by the server
    bool authorized;

    // Keep track of an open connection
    // Multiple connections possible but unnecessary?
    Thread* openConnection = nullptr;    
};

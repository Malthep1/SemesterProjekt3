#include "TCPThreads.hpp"


ListenerThread::ListenerThread(int port, const char* servIP, ServTCP* s){
    server = s;
    
    cData.outAddress.sin_port = htons(port);
    cData.outAddress.sin_family = AF_INET;
    cData.outAddress.sin_addr.s_addr = INADDR_ANY;
    cData.thisAddress.sin_port = htons(port);
    cData.thisAddress.sin_family = AF_INET;
    // inet_aton(servIP, (in_addr*) &cData.thisAddress.sin_addr.s_addr);
    cData.thisAddress.sin_addr.s_addr = INADDR_ANY;
}

void ListenerThread::run(){
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    if(socketId == -1){
        throw SocketError();
    }
    printf("Socketid ok...\n");
    int addrLen = sizeof(cData.thisAddress);

    if(bind(socketId, (sockaddr*) &cData.thisAddress, (socklen_t) addrLen) < 0){
        printf("uh - oh ...\n");
        throw SocketError();
    }
    printf("bind ok...\n");

    if(listen(socketId, 10) < 0){
        throw SocketError();
    }

    int inSocket;
    addrLen = sizeof(cData.outAddress);
    for(;;){
        inSocket = accept(socketId, (struct sockaddr *)&cData.outAddress, (socklen_t*)&addrLen);
        printf("Something here...\n");
        MsgNewCon* msg = new MsgNewCon;
        msg->fd = inSocket;
        printf("Sending msg\n");
        server->getMsgQueue()->send(ServTCP::MID_TCP_SOCK_NEW, msg);
    }
}

ConnectionThread::ConnectionThread(int tid, ServTCP* s){
    id = tid;
    server = s;
}

void ConnectionThread::run(){
    printf("Connection no %i up and running!\n", id);
    std::string msg = receiveMsg();
    int err = handleMessage(msg);
}

std::string ConnectionThread::receiveMsg(){
    memset(buffer, 0, sizeof(buffer));
    read(id, buffer, sizeof(buffer));
    std::string message(buffer);
    std::cout << "Message: " << message << "\n";
    return message;
}

int ConnectionThread::handleMessage(std::string msg){
    if(auth.compare(msg) == 0){
        printf("Device requested authorisation\n");
        std::string ack("ack");
        sendMsg(ack);
        std::string unique_id("42");
        sendMsg(unique_id);
    }
    else if(treat.compare(msg) == 0){
        printf("Pet asked for a treat\n");
    }
    else if(info.compare(msg) == 0){
        printf("Device requests send info\n");
    }
    else{
        printf("Unrecognized message");
        std::string nack("nack");
        sendMsg(nack);
        printf("\n Message compared with auth = %d \n", auth.compare(msg));
    }

    return 0;
}

int ConnectionThread::sendMsg(std::string msg){
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, msg.c_str(), msg.length());
    printf("%s in send buffer\n", buffer);
    send(id, buffer, msg.length() + 1, 0);
    return 0;
}
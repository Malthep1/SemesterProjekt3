#include "TCPThreads.hpp"


ListenerThread::ListenerThread(int port, const char* servIP, ServTCP* s){
    server = s;
    
    cData.outAddress.sin_port = htons(port);
    cData.outAddress.sin_family = AF_INET;
    cData.outAddress.sin_addr.s_addr = INADDR_ANY;
    cData.thisAddress.sin_port = htons(port);
    cData.thisAddress.sin_family = AF_INET;
    inet_aton(servIP, (in_addr*) &cData.thisAddress.sin_addr.s_addr);
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
    read(id, buffer, 100);
    std::string message = "";
    for(int i = 0; i < sizeof(buffer); i++){
        message += buffer[i];
        printf("%c", buffer[i]);
    }
    printf("The message is, %s\n", message);
    return message;
}

int ConnectionThread::handleMessage(std::string msg){
    if(msg == "auth"){
        printf("Device requested authorisation\n");
    }
    else if(msg == "treat"){
        printf("Pet asked for a treat\n");
    }
    else if(msg == "info"){
        printf("Device requests send info\n");
    }

    return 0;
}

int ConnectionThread::sendMsg(std::string msg){
    return 0;
}
#include "ServTCP.hpp"

void ServTCP::run(){
    printf("running servtcp\n");
    createListener();
    unsigned long id;
    Message* msg;

    for(;;){
        msg = tcpMq.receive(id);
        handleMsg(id, msg);
        delete msg;
    }
}

int ServTCP::createListener(){
    printf("creating listender\n");
    ListenerThread* lt = new ListenerThread(startPort, IP, this);
    osapi::Thread* lthread = new Thread(lt, osapi::Thread::PRIORITY_ABOVE_NORMAL, "listener");
    threadPool->push_back(lthread);
    printf("listener created. starting\n");
    lthread->start();
    return 0;
}

int ServTCP::handleMsg(unsigned long id, Message* msg){
    printf("Received msg in q\n");
    switch(id){
        case MID_TCP_SOCK_NEW: {
            printf("Recognizeed message\n");
            MsgNewCon* msg_ = static_cast<MsgNewCon*>(msg);
            int fd = msg_->fd;
            handleConnection(fd);
            break;
        }
        default: {
            printf("Unrecognized message...\n");
            break;
        }
    }
    return 0;
}

int ServTCP::handleConnection(int fd){
    openSockets->push_back(fd);
    printf("Server received new connection\n");
    printf("creating connector\n");
    ConnectionThread* ct = new ConnectionThread(fd, this);
    osapi::Thread* cthread = new Thread(ct);
    threadPool->push_back(cthread);
    printf("listener created. starting\n");
    cthread->start();
    
    return 0;
}

MsgQueue* ServTCP::getMsgQueue(){
    return &tcpMq;
}
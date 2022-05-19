#include "TCPThreads.hpp"


ListenerThread::ListenerThread(int port, ServTCP* s){
    server = s;
    
    cData.outAddress.sin_port = htons(port);
    cData.outAddress.sin_family = AF_INET;
    cData.outAddress.sin_addr.s_addr = INADDR_ANY;
    cData.thisAddress.sin_port = htons(port);
    cData.thisAddress.sin_family = AF_INET;
    cData.thisAddress.sin_addr.s_addr = INADDR_ANY;
}

/* Creates a thread that listens to*/
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

ConnectionThread::ConnectionThread(int tid, ServTCP* s) : connMQueue(2), id(tid), server(s){}

// OneShot Thread: no infinite loop. Dies after message has been handled
// TODO: Garbage Collection (Destructor?) Send msg to server that it can delete the allocated resource
void ConnectionThread::run(){
    printf("Connection no %i up and running!\n", id);
    std::string msg = receiveMsg();
    int err = handleMessage(msg);
}

// Read tcp message, convert it to string type and return it
std::string ConnectionThread::receiveMsg(){
    memset(buffer, 0, sizeof(buffer));
    read(id, buffer, sizeof(buffer));
    std::string message(buffer);
    std::cout << "Message: " << message << "\n";
    return message;
}
/* The handler now parses the string and checks for protocol keywords + user id
*  Then it passes the user id to the correct handle
*  Uses string.find() to check for the correct keywords
*  TODO: Handles should send a correct msg back to ServTCP class via its MsgQueue
*  TODO: Authorization feature (might need to wait for WS part to do this)
*/
int ConnectionThread::handleMessage(std::string msg){
    
    if(msg.find(auth) == 0){
        printf("Device requested authorisation\n");
        std::string ack("ack");
        sendMsg(ack);
        handleAuthorization();
    }
    else if((msg.find(treat)) != std::string::npos){
        printf("Pet asked for a treat\n");
        char char_id[4];
        int c = 0;
        for(int i = 5; i < msg.length(); i++){
            if(msg[i] >= '0' && msg[i] <= '9'){
                char_id[c] = msg[i];
                c++;
            }
        }
        char_id[3] = '\0';
        int user_id = atoi(char_id);
        printf("id == %d\n", user_id);
        // Insert check user ID code here

        std::string ack("ack");
        sendMsg(ack);
        
        handleTreatRequest(user_id);
    }
    else if(msg.find(info) == 0){
        printf("Device requests send info\n");
        char char_id[4];
        int c = 0;
        for(int i = 5; i < msg.length(); i++){
            if(msg[i] >= '0' && msg[i] <= '9'){
                char_id[c] = msg[i];
                c++;
            }
        }
        char_id[3] = '\0';
        int user_id = atoi(char_id);
        printf("id == %d\n", user_id);

        std::string ack("ack");
        sendMsg(ack);
        
        // Insert check user ID code here
        handleInfoSendRequest(user_id);
    }
    else{
        printf("Unrecognized message");
        std::string nack("nack");
        sendMsg(nack);
        printf("\n Message compared with auth = %d \n", auth.compare(msg));
    }

    return 0;
}

int ConnectionThread::handleAuthorization(){
    return 0;
}
int ConnectionThread::handleTreatRequest(int id){
    return 0;
}
int ConnectionThread::handleInfoSendRequest(int id){
    return 0;
}

// Copy the content of string msg and send it via tcp
int ConnectionThread::sendMsg(std::string msg){
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, msg.c_str(), msg.length());
    printf("%s in send buffer\n", buffer);
    send(id, buffer, msg.length() + 1, 0);
    return 0;
}
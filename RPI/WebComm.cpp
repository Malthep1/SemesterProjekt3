#include "WebComm.hpp"

#define PORT 9000


// Creates a new connection following a protocol specified by 2nd parameter
// First parameter is SystemController MsgQueue*
Thread* WebComm::create_new_connection(MsgQueue* scQueue, REQ_TYPE req_type){
    // Make a new thread. Implementation in connThread
    connThread nc(scQueue, req_type);
    Thread* thread_conn = new Thread(&nc);

    thread_conn->start();

    // DEBUG:
    thread_conn->join();

    // Return for garbage collection
    return thread_conn;
}

// Setup tcp connection with the server and run the correct protocol
// TODO: Different types of requests
// TODO: First time authorization
void WebComm::connThread::run(){
    setup_connection();
    switch(type_){
        case REQ_AUTH:
            send_auth();
            break;
        default:
            printf("Unrecognized request\n");
    }
}

/* The three protocols that we use are implemented here*/
int WebComm::connThread::send_auth(){
    char msg_from_serv[20];
    char msg_to_serv[5] = "auth";
    msg_to_serv[4] = '\0';

    // Send and receive msgs to server according to protocol
    int bytes = send(socket_fd, msg_to_serv, 5, 0);
    printf("Sent %i bytes\n", bytes);

    bytes = read(socket_fd, msg_from_serv, 5);
    printf("Received %i bytes\n", bytes);

    std::string reply(msg_from_serv);
    if(m_ack.compare(reply) != 0){
        printf("Server not acknowledged command\n");
        return -1;
    }
    printf("Server acknowledged command\n");
    printf("%s\n", msg_from_serv);
    //memset(msg_from_serv, 0, sizeof(msg_from_serv));

    bytes = read(socket_fd, msg_from_serv, 15);
    printf("Message : %s\n", msg_from_serv);
    return 0;
}

int WebComm::connThread::send_req_treat(){
    return 0;
}

int WebComm::connThread::send_req_info(){
    return 0;
}

// Sets up the connection by finding the correct IP address, creating a tcp socket 
// and attempting to connect to the server
int WebComm::connThread::setup_connection(){
    
    // Get server ip
    if(get_fgs_ip() != 0){
        printf("Could not find server IP\n");
        return -1;
    }

    printf("%s, %d\n", serv_IP, PORT);
    
    // Set up a new socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        // debug:
        printf("Error creating socket\n");
        return -1;
    }

     // Setup the socket data 
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;     // Ip4
    serv_addr.sin_port = htons(PORT);   // Port 9000

    
    // Convert address to ip4  
    if(inet_pton(AF_INET, serv_IP, &serv_addr.sin_addr) <= 0){
        printf("Incorrect IP address\n");
        return -1;
    }
    printf("address converted\n");
 

    int error = connect(socket_fd, (sockaddr*) &serv_addr, sizeof(serv_addr));
    if (error < 0){
        printf("Conection Failed \n");
        printf("Error number: %d", errno);
        return -1;
    }

    printf("Connection to the server established");
    
    return 0;
}

// Requests server address info from DNS and stores the IP in serv_IP
int WebComm::connThread::get_fgs_ip(){
    char ip[50];
    char hostname[50];
    addrinfo *temp;
    int err = getaddrinfo(SERVER_NAME, NULL, NULL, &temp);
    if(err != 0){
        printf("Error getting server addrinfo (DNS)\n");
        return -1;
    }
    
    // Address stored in hostname in a funky format
    err = getnameinfo(temp->ai_addr, temp->ai_addrlen, hostname, 50, NULL, 0, 0);
    if(err){
        printf("getnameinfo error\n");
        return -1;
    }
    
    // getnameinfo returns a bunch of non-IP related characters so we eliminate them here
    int counter = 0;
    for(int i = 0; i < 50; i++){
        char c = hostname[i];
        // printf("%c ", hostname[i]);
        if(c == '\0'){
            ip[counter] = '\0';
            break;
        }
        if(c >= '0' && c <= '9'){
            ip[counter] = c;
            counter++;
        }
        else if(c == '-'){
            ip[counter] = '.';
            counter++;
        }
    }
   
    strcpy(serv_IP, ip);
    printf("IP = %s\n", serv_IP); 
    return 0;
}
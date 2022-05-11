#include "WebComm.hpp"

#define PORT 9000



Thread* WebComm::create_new_connection(MsgQueue* scQueue, int req_type){
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

    char msg_from_serv[100];
    char msg_to_serv[20] = "test";
    msg_to_serv[5] = '\0';

    // Send and receive msgs to server according to protocol
    int bytes = send(socket_fd, msg_to_serv, 5, 0);
    printf("Sent %i bytes\n", bytes);

    bytes = read(socket_fd, msg_from_serv, 100);
    printf("Received %i bytes\n", bytes);

    printf("Message : %s\n", msg_from_serv);
    
}

int WebComm::connThread::setup_connection(){
    
    // Get server ip
    if(get_fgs_ip() != 0){
        printf("Could not find server IP\n");
        return -1;
    }
    
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

    printf("%s, %d\n", serv_IP, PORT);
    // Convert address to ip4  
    if(inet_pton(AF_INET, serv_IP, &serv_addr.sin_addr) <= 0){
        printf("Invalid address/Not supported \n");
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

int WebComm::connThread::get_fgs_ip(){
    char ip[50];
    char hostname[50];
    addrinfo *temp;
    int err = getaddrinfo(SERVER_NAME, NULL, NULL, &temp);
    if(err != 0){
        printf("Some error occured\n");
    }
    
   
    err = getnameinfo(temp->ai_addr, temp->ai_addrlen, hostname, 50, NULL, 0, 0);
    if(err){
        printf("getnameinfo error\n");
    }
    

    // getnameinfo returns a bunch of non-IP related characters so we eliminate them here
    int counter = 0;
    for(int i = 0; i < 50; i++){
        char c = hostname[i];
        printf("%c ", hostname[i]);
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

    
    serv_IP = ip;
    printf("IP = %s\n", serv_IP); 
    return 0;
}
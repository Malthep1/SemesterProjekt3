#include "WebComm.hpp"
//#include "findFgDNS.h"
#include <iostream>


using std::cout;

int main(int argc, char* argv[]){
    cout << "Main test online\n";
    
    WebComm::REQ_TYPE type;
    std::string param(argv[1]);
    //printf("%s\n", param.c_str());
    if(param == "auth"){
        type = WebComm::REQ_AUTH;
    }
    else if(param == "treat"){
        type = WebComm::REQ_TREAT;
    }
    else if(param == "info"){
        type = WebComm::REQ_SEND_INFO;
        printf("info\n");
    }
    else{
        printf("Incorrect parameter, aborting");
        return 0;
    }
    cout << "ok ";
    MsgQueue mQueue(2);
    WebComm webCom;
    printf("ok ");
    webCom.create_new_connection(&mQueue, type);
    printf("ok ");
    webCom.shutdownConnection();
    printf("ok ");
    return 0;
}
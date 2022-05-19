#include "WebComm.hpp"
//#include "findFgDNS.h"
#include <iostream>


using std::cout;

int main(void){
    cout << "Main test online\n";
    
    cout << "ok ";
    MsgQueue mQueue(2);
    WebComm webCom;
    printf("ok ");
    webCom.create_new_connection(&mQueue, WebComm::REQ_AUTH);
    printf("ok ");
    webCom.shutdownConnection();
    printf("ok ");
    return 0;
}
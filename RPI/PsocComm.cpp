#include "PsocComm.hpp"

PsocComm& PsocComm::getInstance(){
        static PsocComm instance;
        return instance;
}

void PsocComm::createPsocCommListenerThread(MsgQueue* msgQueue){
    PsocListenerThread lt(msgQueue);
    Thread* thread_listener = new Thread(&lt); 
    thread_listener->start();
    std::cout << "created\n";
    //thread_listener->detach();
    std::cout << "detached\n";
    sleep(1);
}
void PsocComm::PsocListenerThread::handleUart(std::string uartString){
    UartString* msg = new UartString;
    msg->response = uartString;
    if(uartString == "SACKE"){
        msgQueue_->send(ACK, msg);
    }
    else if(uartString == "SNACKE"){
        //Failed, resend latestCommand
        msgQueue_->send(NACK, msg);
    }
    else if(uartString == "SWLOWE"){
        msgQueue_->send(WLOW, msg);
    }
    else if(uartString == "SFLOWE"){
        msgQueue_->send(FLOW, msg);
    }
    else if(uartString == "STLOWE"){
        msgQueue_->send(TLOW, msg);
    }
    else if(uartString == "STRE"){
        msgQueue_->send(TLOW, msg);
    }
}

void PsocComm::PsocListenerThread::run(){
    //std:: cout << "Started\n";
    //string uartInc("SACKE");
    //osapi::sleep(500);
    //handleUart(uartInc);
    string uartInc("SACKE");
    std::cout << "PSOC INC\n";
    handleUart(uartInc);

    while(true){
        string uartInc = waitForUartComm();
        std::cout << "PSOC INC\n";
        handleUart(uartInc);
    }
}

void PsocComm::sendCommand(unsigned char* s){
    // if s = re, resending is the goal
    //std::cout << s << "\n";
    //if(s == "RE"){
    //    s = latestCommand;
    //}
    //converting to char array and ptr
    //unsigned char* ptr = &s;

    //unsigned char char_arr1[2];
    //unsigned char char_arr2[4];
    //if(s == "TD" || s == "TA"){
    //    ptr = &char_arr1[0];
    //    std::copy(s.cbegin(), s.cend(), char_arr1);
    //}
    //else {
    //    std::cout << "BEFORE POINTER CREATION\n";
    //    ptr = &char_arr2[0];
    //    std::copy(s.cbegin(), s.cend(), char_arr2);
    //}

    int error = uart_.transmitBytes(s);
    if(error != 0){
        std::cout << "Error transmitting bytes.\n";
    }
    else{
        std::cout << "Bytes transmitted.\n";
    }
    latestCommand = s;
}

string PsocComm::PsocListenerThread::waitForUartComm(){
    int commandReceived = 0;
    unsigned char* incomming;
    
    while(commandReceived == 0){
        incomming = uart_->readBytes();
        if(incomming != NULL && incomming[0] != '\0'){
            commandReceived++;
        }
        sleep(5);
    }
    const char * constPtr = (const char*) incomming;
    std::string returnstring(constPtr);
    return returnstring;
}
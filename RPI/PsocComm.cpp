#include "PsocComm.hpp"

PsocComm& PsocComm::getInstance(){
        static PsocComm instance;
        return instance;
}

Thread* PsocComm::createPsocCommListenerThread(MsgQueue* msgQueue){
    PsocListenerThread lt(msgQueue);
    Thread* thread_listener = new Thread(&lt);
    thread_listener->start();
    thread_listener->join();
    return thread_listener;
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
}

void PsocComm::PsocListenerThread::run(){
    std::cout << "PSOC LISTENER THREAD ONLINE \n";
    //while(true){
        //string uartInc = waitForUartComm();
        string uartInc("SACKE");
        handleUart(uartInc);
        uartInc = "SNACKE";
        handleUart(uartInc);
        uartInc = "SWLOWE";
        handleUart(uartInc);
        uartInc = "SFLOWE";
        handleUart(uartInc);
        uartInc = "STLOWE";
        handleUart(uartInc);
    //    osapi::sleep(20000);
    //}
}

void PsocComm::sendCommand(string s){
    // if s = re, resending is the goal
    if(s == "RE"){
        s = latestCommand;
    }
    //converting to char array and ptr
    unsigned char* ptr;
    if(s == "TD" || s == "TA"){
        unsigned char char_arr[2];
        ptr = &char_arr[0];
        std::copy(s.cbegin(), s.cend(), char_arr);
    }
    else {
        unsigned char char_arr[4];
        ptr = &char_arr[0];
        std::copy(s.cbegin(), s.cend(), char_arr);
    }
    
    int error;
    error = uart_.transmitBytes(ptr);
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
        osapi::sleep(5000);
    }
    const char * constPtr = (const char*) incomming;
    std::string returnstring(constPtr);
    return returnstring;
}
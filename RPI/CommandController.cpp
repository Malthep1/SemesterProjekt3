#include "CommandController.hpp"

MsgQueue* CommandController::getMessageQueue(){
    MsgQueue * mqptr;
    mqptr = &msgQueue_;
    return mqptr;
}

void CommandController::handleCommand(osapi::Message* msg, unsigned long id){
    UartString* cmsg = static_cast<UartString*>(msg);
    string resp = cmsg->response;
    
    if(cmsg->response == "SACKE"){
        //std::cout << "SACKE HIT\n";
        //ACK
    }   
    else if(cmsg->response == "SNACKE"){
        //std::cout << "SNACKE HIT\n";
        //dispatchUartCommand("RE");
    }
    else if(cmsg->response == "STLOWE"){
        //std::cout << "TLOW HIT\n";
        //Web notification
    }
    else if(cmsg->response == "SFLOWE"){
        //std::cout << "FLOW HIT\n";
        //Web notificiation
    }
    else if(cmsg->response == "SWLOWE"){
        //std::cout << "WLOW HIT\n";
        //Web notificiation
    }
    else if(cmsg->response == "STRE"){
        std::cout << "TR HIT\n";
        //Web notificiation 
    }
    else if(cmsg->response == "STAE"){
        std::cout << "TA HIT\n";
        //Dispense treat
        //dispatchUartCommand("T");
    }
    else if(cmsg->response == "STDE"){
        std::cout << "TD HIT\n";
        //Denied, do nothing??
    }
    else if(id == 123123){
        char * ptr = strcpy(new char[5], resp.c_str());
        dispatchUartCommand((unsigned char*) ptr);
    }
    else{
        std::cout << "ERROR, returning NACK\n";
    }

}

void CommandController::setupCommunicationModules(){
    PsocComm::getInstance().createPsocCommListenerThread(getMessageQueue());
    //WebComm::getInstance().createPsocCommListenerThread(getMessageQueue());
}

void CommandController::dispatchUartCommand(unsigned char* s){
    PsocComm::getInstance().sendCommand(s);
}

void CommandController::requestTreat(){
    web_.postTreatRequest(420);
}

void CommandController::getTreatRequestAnswer(){
    web_.createWebCommListenerThread(getMessageQueue());
}

void CommandController::dispatchCommand(){
    std::cout << "Waiting for command\n";
    Message* msg = getMessageQueue()->receive(id);
    handleCommand(msg, id);
    delete msg;
}
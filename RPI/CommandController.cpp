#include "CommandController.hpp"

MsgQueue* CommandController::getMessageQueue(){
    MsgQueue * mqptr;
    mqptr = &msgQueue_;
    return mqptr;
}

void CommandController::handleCommand(osapi::Message* msg, unsigned long id){
    std::cout << "HANDLER\n";
    UartString* cmsg = static_cast<UartString*>(msg);
    string resp = cmsg->response;
    std::cout << "Response: " << resp << "\n";
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
    else if(id == 1){
        char * ptr = strcpy(new char[5], resp.c_str());
        dispatchUartCommand((unsigned char*) ptr);
    }
    else if(id == 2){
        char * ptr = strcpy(new char[5], resp.c_str());
        dispatchUartCommand((unsigned char*) ptr);
    }
    else if(id == 3){
        char * ptr = strcpy(new char[5], resp.c_str());
        dispatchUartCommand((unsigned char*) ptr);
        if(cmsg->response == "STAE"){
            std::cout << "TA HIT\n";
            //Dispense treat
            //dispatchUartCommand("T");
        }
        else if(cmsg->response == "STDE"){
            std::cout << "TD HIT\n";
            //Denied, do nothing??
        }
    }
    else if(id == 9){
        int deleteId = std::stoi(cmsg->response);
        web_.deleteThread(deleteId);
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
        Message* msg = msgQueue_.receive(id);
        handleCommand(msg, id);
        delete msg;
    
}
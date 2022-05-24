#pragma once

/**
 * @file CommandController
 * @author Malthe Petersen
 * @version 0.3
 * added setupCommunicationModules()
 * @date 2022-05-23
 */

//#include "WebComm.hpp"   TODO WHEN IMPLEMENTED
#include "PsocComm.hpp"
#include <osapi/Thread.hpp>
#include <osapi/ThreadFunctor.hpp>
#include <osapi/MsgQueue.hpp>
#include <unistd.h>         // read() is used to read inotify events
#include <fcntl.h>
#include <string>
#include <iostream>
using osapi::Thread;
using osapi::MsgQueue;
using osapi::Message;
using std::string;

struct uartString : public osapi::Message{
    std::string* response; 
};

struct webString : public osapi::Message{
    std::string* response; 
};


class CommandController{
public:
    CommandController() : msgQueue_(10){
        std::cout << "COMMANDCONTROLLER ONLINE\n";
        setupCommunicationModules();
    };
    //Checks msqQueue, dispatches command if present
    void dispatchCommand();
    //For dispatching command directly from sys manager
    void dispatchUartCommand(string s);
    MsgQueue * getMessageQueue();
private:
    void setupCommunicationModules();
    void handleCommand(Message* msg, unsigned long id);
    unsigned long id;
    MsgQueue msgQueue_;
    //WebComm web_;
};

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include <osapi/Thread.hpp>
#include <osapi/ThreadFunctor.hpp>
#include <osapi/MsgQueue.hpp>
//EXTERNALLY SOURCED HEADER
#include "HTTPRequest.hpp"

using osapi::Thread;
using osapi::MsgQueue;
using osapi::Message;
using std::string;
using std::map;

/**
 * @file WebComm
 * @author Malthe Petersen
 * @version 0.3
 * @date 2022-06-02
 */

struct responseString : public osapi::Message{
    std::string response; 
};



class WebComm {
    
    public:
        WebComm(){};
        void postTreatRequest(int devid);
        void postNotification(string type);
        void createWebCommListenerThread(MsgQueue* msgQueue);
        void deleteThread(int id);
    private:
        class WebCommListenerThread : public osapi::ThreadFunctor{
            public:
                WebCommListenerThread(MsgQueue* msgQueue, int id_) : msgQueue_(msgQueue) {id = id_;};
                enum ids{
                    TD,
                    TA
                };
            private:
                void run();
                string getRequestAnswer();
                void notifyCommandControl();
                void handleResponse(std::string responseString);
                MsgQueue* msgQueue_;
                int id;
        };
    struct threadStruct{
        WebCommListenerThread* wchread;
        Thread* tp;
    };
    int id = 0;
    map<int,threadStruct> threadPool;
};

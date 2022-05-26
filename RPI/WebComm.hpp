#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
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

/**
 * @file WebComm
 * @author Malthe Petersen
 * @version 0.2
 * @date 2022-05-25
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
    private:
        class WebCommListenerThread : public osapi::ThreadFunctor{
            public:
                WebCommListenerThread(MsgQueue* msgQueue) : msgQueue_(msgQueue) {}
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
        };
};

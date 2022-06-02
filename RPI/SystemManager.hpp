#pragma once

/**
 * @file SystemManager.hpp
 * @author Marcin Szymanek (marcinwszymanek@gmail.com) & Malthe Petersen(0.2 & 0.3 & 0.4 & 0.5)
 * @brief System Manager Class
 * @version 0.5
 * @date 2022-05-31
 * 
 * Purpose is managing the entire system at runtime.
 * 
 */

#include "CommandController.hpp"
#include <osapi/Thread.hpp>
#include <osapi/linux/Thread.hpp>
#include <osapi/linux/ThreadFunctor.hpp>
#include <sys/inotify.h>    // inotify API
#include <unistd.h>         // read() is used to read inotify events
#include <fcntl.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

using osapi::Thread;
using osapi::ThreadFunctor;
using osapi::MsgQueue;
using osapi::Message;

struct Setting
{
    int feedingHour;
    int feedingMinute;
    int foodAmount;
    int treatLimit;
    bool treatsEnabled = false;
    bool treatRequestsEnabled = false;
};

class SystemManager{
public:
    SystemManager(){
        std::cout << "System Manager Online\n";
        runMain();
    };
    
private:
    
    void runMain();
    //Main Thread
    class MainThread : public ThreadFunctor {
    public:
        MainThread() {
            currentSetting.feedingHour = 15;
            currentSetting.feedingMinute = 53;
            currentSetting.foodAmount = 100;
            currentSetting.treatLimit = 3;
            currentSetting.treatRequestsEnabled = true;
            currentSetting.treatsEnabled = true;
            listenSettingsUpdate();
            waitFeedingTime();
        };
    private:
        void run();
        void listenSettingsUpdate();
        void waitFeedingTime();
        //Settings Struct
        Setting currentSetting;
        // CommandController Object
        CommandController cmdCtrl;
        Thread* lt_;
        Thread* tt_;

        class ListenerThread : public ThreadFunctor {
        public:
            ListenerThread(Setting * curSetting, MsgQueue * msgQueue_) : inotify_fd(NULL) {
                curSet = curSetting;
                msgQueue = msgQueue_;
            };
        private:
            void run();
            // Setup fcntl and inotify 
            int init_inot();
            int inotify_fd;
            int inotify_watch_fd;
            Setting * curSet;
            MsgQueue * msgQueue;
        };
        class TimeThread : public ThreadFunctor{
        public:
            TimeThread(Setting * curSetting, MsgQueue * msgQueue_) {
                curSet = curSetting;
                msgQueue = msgQueue_;
            };
        private:
            void run();
            void passToCommandController(std::string command);
            Setting * curSet;
            MsgQueue * msgQueue;
        };
    };
    
    // If we want more threads we make a vector<Thread*> instead (threadpool) and listenSettingsUpdate()
    // can returns pointer to put the thread inside the vector
    Thread* mt_;
};

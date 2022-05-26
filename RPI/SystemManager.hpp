#pragma once

/**
 * @file SystemManager.hpp
 * @author Marcin Szymanek (marcinwszymanek@gmail.com) & Malthe Petersen(0.2 & 0.3 & 0.4)
 * @brief Prototype inotify thread to keep tabs on /dat/settings.dat file
 * @version 0.4
 * @date 2022-05-24
 * 
 * For testing purposes the thread is ran and joined when SystemManager is created
 * The thread will print out a message every time a file has been written to and closed
 * 
 */

#include "CommandController.hpp"
#include <osapi/Thread.hpp>
#include <osapi/linux/Thread.hpp>
#include <osapi/linux/ThreadFunctor.hpp>
#include <sys/inotify.h>    // inotify API
#include <unistd.h>         // read() is used to read inotify events
#include <fcntl.h>
#include <vector>
#include <boost/algorithm/string.hpp>

using osapi::Thread;
using osapi::ThreadFunctor;

struct Setting
{
    int feedingtime;
    int foodAmount;
    int treatLimit;
    bool treatsEnabled = false;
    bool treatRequestsEnabled = false;
};

class SystemManager{
public:
    SystemManager() : lt_(nullptr), tt_(nullptr){
        std::cout << "System Manager Online\n";
        listenSettingsUpdate();
        currentSetting.feedingtime = 8;
        currentSetting.foodAmount = 100;
        currentSetting.treatLimit = 3;
        currentSetting.treatRequestsEnabled = true;
        currentSetting.treatsEnabled = true; 
    };
    void runMain();
private:
    //Settings Struct
    Setting currentSetting;

    //Command Controller Object
    CommandController cmdCtrl;

    // Implementation of listener thread goes here
    class ListenerThread : public ThreadFunctor{
    public:
        ListenerThread(Setting * curSetting) : inotify_fd(NULL) {
            curSet = curSetting;
        };
    private:
        void run();
        // Setup fcntl and inotify 
        int init_inot();
        int inotify_fd;
        int inotify_watch_fd;
        Setting * curSet;
    };
    class TimeThread : public ThreadFunctor{
    public:
        TimeThread(Setting * curSetting) {
            curSet = curSetting;
        };
    private:
        void run();
        Setting * curSet;
    };
    void runSystem();
    void listenSettingsUpdate();
    void waitFeedingTime();
    // If we want more threads we make a vector<Thread*> instead (threadpool) and listenSettingsUpdate()
    // can returns pointer to put the thread inside the vector
    Thread* lt_;
    Thread* tt_;
    
};

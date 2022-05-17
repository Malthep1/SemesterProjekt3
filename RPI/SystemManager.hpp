#pragma once

/**
 * @file SystemManager.hpp
 * @author Marcin Szymanek (marcinwszymanek@gmail.com)
 * @brief Prototype inotify thread to keep tabs on /dat/settings.dat file
 * @version 0.1
 * @date 2022-05-13
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

using osapi::Thread;
using osapi::ThreadFunctor;

struct Setting
{
    std::vector<int> feedingtimes;
    int foodAmount;
    int treatLimit;
    bool treatsEnabled = false;
    bool treatRequestsEnabled = false;
};

class SystemManager{
public:
    SystemManager() : lt_(nullptr){
        listenSettingsUpdate();
    };

private:
    //Settings Struct
    Setting currentSetting;

    //Command Controller Object
    CommandController cmdCtrl;

    // Implementation of listener thread goes here
    class ListenerThread : public ThreadFunctor{
    public:
        ListenerThread() : inotify_fd(NULL){}
    private:
        void run();
        // Setup fcntl and inotify 
        int init_inot();
        int inotify_fd;
        int inotify_watch_fd;
    };
    void runSystem();
    void listenSettingsUpdate();
    // If we want more threads we make a vector<Thread*> instead (threadpool) and listenSettingsUpdate()
    // can returns pointer to put the thread inside the vector
    Thread* lt_;
    
};

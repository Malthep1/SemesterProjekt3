#include "osapi/linux/Mutex.hpp"
#include <osapi/MutexError.hpp>
#include <iostream>

namespace osapi
{
    Mutex::Mutex(){
        mut_ = PTHREAD_MUTEX_INITIALIZER;
    }

    // Mutex must NOT be locked
    void Mutex::lock(){
        if(pthread_mutex_lock(&mut_) != 0) throw MutexError();
    }

    // Mutex must be locked
    void Mutex::unlock(){
        if(pthread_mutex_unlock(&mut_) != 0) throw MutexError();
    }

    Mutex::~Mutex(){}
}
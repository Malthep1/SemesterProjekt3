#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <pthread.h>
#include <osapi/Utility.hpp>

namespace osapi
{
  class Conditional;

  class Mutex : Notcopyable
  {
  public:
    Mutex();
    void lock();
    void unlock();
    ~Mutex();
  private:
    friend class Conditional;
    pthread_mutex_t mut_;
  };
}



#endif
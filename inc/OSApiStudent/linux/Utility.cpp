#include <unistd.h>
#include <osapi/Utility.hpp>

namespace osapi
{
  
void sleep(unsigned long msecs)
{
  ::usleep(msecs * 1000);
}

}
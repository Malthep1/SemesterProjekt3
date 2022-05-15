#include <iostream>
#include <osapi/example/LogSystem.hpp>

void LogSystem::writeToLog(LogInd* l)
{
  lf_ << l->text << std::endl;
  
}

void LogSystem::handleMsg(unsigned long id, osapi::Message* msg)
{
  switch(id)
  {
    case ID_LOG_IND:
      std::cout << "Writing to log\n"; 
      writeToLog(static_cast<LogInd*>(msg));
      std::cout << "Write complete\n";
      break;

    default:
      std::cout << "Unknown event..." << std::endl;
  }
  
}

void LogSystem::run()
{
  for(;;)
  {
    unsigned long id;
    osapi::Message* msg = mq_.receive(id);
    handleMsg(id, msg);
    delete msg;
  }
  
}


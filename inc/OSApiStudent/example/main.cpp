#include <osapi/Thread.hpp>
//#include <Console.hpp>
#include <osapi/example/LogSystem.hpp>
#include <osapi/example/KeyBoardInput.hpp>
#include <iostream>
using std::cout;

int main()
{
  cout << "Starting main\n";
  //Console c;
  LogSystem l;
  KeyBoardInput k(&l);
  cout << "KeyboardInput created\n";
  
  //osapi::Thread ct(&c);
  osapi::Thread lt(&l);
  cout << "lt created\n";
  lt.start();
  cout << "lt started\n";
  osapi::Thread kt(&k);
  kt.start();
  cout << "kt started\n";

  //ct.join();
  lt.join();
  cout << "lt joined\n";
  kt.join();
  
}

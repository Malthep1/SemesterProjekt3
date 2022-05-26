#pragma once

/**
 * @file PsocComm
 * @author Malthe Petersen
 * @version 0.3
 * Singleton pattern
 * @date 2022-05-23
 */
#include "RPIUart.hpp"
#include <osapi/Thread.hpp>
#include <osapi/ThreadFunctor.hpp>
#include <osapi/MsgQueue.hpp>
#include <iostream>
#include <unistd.h>         // read() is used to read inotify events
#include <fcntl.h>

using osapi::Thread;
using osapi::MsgQueue;
using std::string;

struct UartString : public osapi::Message{
    std::string response; 
};

class PsocComm{
public:
    static PsocComm& getInstance(); // Returns instance of PsocComm, SINGLETON PATTERN
    void createPsocCommListenerThread(MsgQueue* msgQueue);
    void sendCommand(string s);
private:
    PsocComm(){}; //Can only be instantiated from inside the class.
    PsocComm(PsocComm const&);              // Preventing copies
    void operator=(PsocComm const&);        // Preventing copies
    std::string latestCommand;
    RPIUart uart_;
    // Nested class which is to be run as a thread to listen for incomming messages on the UART.
    class PsocListenerThread : public osapi::ThreadFunctor{
        public:
            PsocListenerThread(MsgQueue* msgQueue) : msgQueue_(msgQueue), uart_(uart_){}
            enum ids{
                ACK,
                NACK,
                WLOW,
                FLOW,
                TLOW,
                TR
            };
        private:
            void run();
            string waitForUartComm();
            void notifyCommandControl();
            void handleUart(std::string uartString);
            MsgQueue* msgQueue_;
            RPIUart* uart_;
    };
};

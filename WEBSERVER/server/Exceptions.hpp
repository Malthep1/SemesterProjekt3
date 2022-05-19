#pragma once

#include "osapi/Exceptions.hpp"

class SocketError : public osapi::SystemError{
    public:
        SocketError() :
         SystemError("Socket error has occured \n")
         {}
};
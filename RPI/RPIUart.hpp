#include <stdio.h>
#include <iostream>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <time.h>
#include <string>
#include <string.h>

/**
 * @file RPIUart
 * @author Malthe Petersen
 * @version 0.1
 * @date 2022-05-24
 */

class RPIUart{
    public:
        RPIUart(){
            initializeUART();
        }
        int transmitBytes(unsigned char* command);
        unsigned char* readBytes();
        void delay();
    private:
        void initializeUART();
        struct termios options;
};

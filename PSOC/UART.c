/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#include "project.h"
#include "UART.h"
#include "weight.h"
#include "dispense.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

char uartBuffer[256];   //Opret en charbuffer

void handleByteReceived(char* arrayPointer)
{   
    switch(arrayPointer[0])
    {
        case 'W' :
        {
            if(isdigit(arrayPointer[1]) && isdigit(arrayPointer[2]) && isdigit(arrayPointer[3]))
            {
                //Gem som char-array
                char number[4];
                number[0] = arrayPointer[1];
                number[1] = arrayPointer[2];
                number[2] = arrayPointer[3];
                number[3] = '\0';
                
                //Konverter fra char til int
                int amount = atoi(number);  
                UART_1_PutString(uartBuffer);
                
                UART_1_PutString("SACKE"); //Send ACK tilbage, da kommando er forstået
                dispenseWater(amount);
            }
            else
            {
                UART_1_PutString("SNACKE"); //Send NACK tilbage, da der er fejl
            }
        }
        break;
        
        case 'F' :
        {
            if(isdigit(arrayPointer[1]) && isdigit(arrayPointer[2]) && isdigit(arrayPointer[3]))
            {
                //Gem som char-array
                char number[4];
                number[0] = arrayPointer[1];
                number[1] = arrayPointer[2];
                number[2] = arrayPointer[3];
                number[3] = '\0';
                
                //Konverter fra char til int
                int amount = atoi(number);
                UART_1_PutString(uartBuffer);
                
                UART_1_PutString("SACKE"); //Send ACK tilbage, da kommando er forstået
                dispenseFood(amount);
            }
            else
            {
                UART_1_PutString("SNACKE"); //Send NACK tilbage, da der er fejl
            }
        }
        break;
        
        case 'T' :
        {
            UART_1_PutString("SACKE"); //Send ACK tilbage, da kommando er forstået
            void dispenseTreat();
        }
        break;
        
        default :
        {
            UART_1_PutString("SNACKE"); //Send NACK tilbage, da der er fejl 
        }
        
    }
}
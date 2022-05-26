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
#include "project.h"
#include "weight.h"
#include <stdio.h>
#include <cypins.h>

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(ISR_sensor_handler);
void handleByteReceived(char* byteReceived);

//Opsætning af interuptflag
int sensorFlag = 0;
int UARTFlag = 0;

int main(void)
{
    CyGlobalIntEnable;      // Enabler global interrupts

    // Initializering
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_sensor_StartEx(ISR_sensor_handler);
    UART_1_Start();
    ADC_SAR_Water_Start();
    ADC_SAR_Water_StartConvert();
    ADC_SAR_Food_Start();
    ADC_SAR_Food_StartConvert();

    for(;;)
    {
        if (UARTFlag == 1)
        {            
            char RPIcommand[6]; //Opret char-array til at håndtere info fra RPI
            uint8_t bytesToRead = 5;
    
            if (UART_1_ReadRxData() == 'S')  //Hvis startbit bliver læst, fortsæt
            {
                for (uint8_t i = 0; i < bytesToRead; i++)
                {
                    while (UART_1_GetRxBufferSize() == 0)   //Sikrer at den næste char er klar
                    {}
                    char c = UART_1_ReadRxData();
                    if (c == 'E')  //Hvis stopbit bliver læst, break
                    {
                        UARTFlag = 0;
                        break;   
                    }
                    RPIcommand[i] = c;
                }
                RPIcommand[5] = '\0';
                UART_1_PutString(RPIcommand);
                
                char *arrayPointer = RPIcommand;   //Pointer til char array
                handleByteReceived(arrayPointer);
            }
            else
            {
            UART_1_PutString("SNACKE"); //Send NACK tilbage, da der er fejl
            UARTFlag = 0;
            }
        }   
    }
}
CY_ISR(ISR_UART_rx_handler)
{
   UARTFlag = 1;    //Sætter interuptflag
}

CY_ISR(ISR_sensor_handler)
{
   sensorFlag = 1;  //Sætter interuptflag
}

/* [] END OF FILE */

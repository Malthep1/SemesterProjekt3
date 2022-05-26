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
#include <stdio.h>
#include "project.h"
#include "dispense.h"
#include "weight.h"
extern int sensorFlag;

void dispenseWater(int amount)
{
    setWeightWater();
    int currentWater = getWeightWater();
    PWM_Water_Start();
    
    for (int i = 0; i < 100; i++)
    {
        if (amount > currentWater)
        {
            if (sensorFlag == 1)
            {
                UART_1_PutString("SWAITE"); //Send WAIT til RPI, for at bede om kommando igen senere
                sensorFlag = 0;
                return;
            }
            CyDelay(100);
            
            setWeightWater();
            int currentWater2 = getWeightWater();
        
            if (currentWater2 > currentWater)
            {
                i--;
                currentWater = currentWater2;
            }
        }
        else
        {
            //Stop motor hvis ønsket amount er nået
            PWM_Water_Stop();
            return;
        }
    }
    //Stop motor hvis vandbeholder er løbet tør
    PWM_Water_Stop();    
    UART_1_PutString("SWLOWE"); //Send WLOW til RPI
    return;
}

void dispenseFood(int amount)
{
    setWeightFood();
    int currentFood = getWeightFood();
    
    for (int i = 0; i < 150; i++)
    {
        if (amount > currentFood)
        {
            Pin_A_Write(1);
            Pin_B_Write(1);
            Pin_A2_Write(0);
            Pin_B2_Write(0);
            CyDelay(10);
            
            Pin_A_Write(0);
            Pin_B_Write(1);
            Pin_A2_Write(1);
            Pin_B2_Write(0);
            CyDelay(10);
            
            Pin_A_Write(0);
            Pin_B_Write(0);
            Pin_A2_Write(1);
            Pin_B2_Write(1);
            CyDelay(10);
            
            Pin_A_Write(1);
            Pin_B_Write(0);
            Pin_A2_Write(0);
            Pin_B2_Write(1);
            CyDelay(10);
            
            setWeightFood();
            int currentFood2 = getWeightFood();
            
            if (currentFood2 > currentFood)
            {
                i--;
                currentFood = currentFood2;
            }
            
            if (sensorFlag == 1)
                {
                    UART_1_PutString("SWAITE"); //Send WAIT til RPI, for at bede om kommando igen senere
                    sensorFlag = 0;
                    return;
                }
        }
        else
        {
            //Stop motor hvis ønsket amount er nået
            Pin_A_Write(0);
            Pin_B_Write(0);
            Pin_A2_Write(0);
            Pin_B2_Write(0);
            return;
        }
    }
    //Stop motor hvis madbeholder er løbet tør
    Pin_A_Write(0);
    Pin_B_Write(0);
    Pin_A2_Write(0);
    Pin_B2_Write(0);
    UART_1_PutString("SFLOWE"); //Send FLOW til RPI
    return;
}

void dispenseTreat()
{
    PWM_Treat_Start();
    CyDelay(4000);
    PWM_Treat_Stop();
}
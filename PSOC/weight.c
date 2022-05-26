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
#include "weight.h"

//Opsætning af globale variabler
volatile uint16_t result = 0;
volatile double offset = 923.0;
volatile double gain = 0.29455;
uint16_t weightWater = 0;
uint16_t weightFood = 0;

void setWeightWater()
{
    if (ADC_SAR_Water_IsEndConversion(ADC_SAR_Water_WAIT_FOR_RESULT))
    {
        uint16_t result = ADC_SAR_Water_GetResult16(); //Måler ADC værdi
        float newResult = gain*(result - offset); //Beregning af vægt
        weightWater = newResult;
    }
}

uint16_t getWeightWater()
{
    return weightWater; 
}

void setWeightFood()
{
    if (ADC_SAR_Food_IsEndConversion(ADC_SAR_Food_WAIT_FOR_RESULT))
    {
        uint16_t result = ADC_SAR_Food_GetResult16(); //Måler ADC værdi
        float newResult = gain*(result - offset); //Beregning af vægt
        weightFood = newResult;
    }
}

uint16_t getWeightFood()
{
    return weightFood; 
}
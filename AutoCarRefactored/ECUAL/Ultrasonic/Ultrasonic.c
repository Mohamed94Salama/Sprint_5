/*****************************************************************************
* Task: Autonomous Car
* File Name: Ultrasonic.c
* Description: File for Ultrasonic driver functions
* Author: Amr Mohamed
* Date: 9/8/2021
******************************************************************************/

/*Includes -----------------------------------------------------------------*/
#include "Ultrasonic.h"
#include <util/delay.h>

/*MACROS -------------------------------------------------------------------*/
//duration of the trigger pulse
#define  TRIGGER_DURATION_US              10u
//Factor to be used when converting reading time into distance in cm
#define  US_CM_FACTOR                     58u

/*ENUMS -----------------------------------------------------------------*/
typedef enum{GETTING_RISING_EDGE,GETTING_FALLING_EDGE}enuReadingState_t;
typedef enum{Reading_Not_Complete,Reading_Complete}enuReadingFlagState_t;

/*GLOBAL VARIABLES -----------------------------------------------------------------*/
uint32_t Reading1=0,Reading2=0;
volatile enuReadingFlagState_t Reading_Flag=Reading_Not_Complete;
uint8_t gu8GroupId;

/*API Implementation ---------------------------------------------------*/

/************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return value    : void
* Description     : A function to record ICU readings - Callback function
************************************************************************************/
static void Get_Readings(void)
{
   //create a state flag
   static enuReadingState_t u8Flag=GETTING_RISING_EDGE;
   //check on the flag condition
   switch (u8Flag)
   {
      //if the flag is set to detecting a rising edge
      case GETTING_RISING_EDGE:
      //get the rising edge timing
      SW_ICU_GetReading(astrUltrasonicConfigParameters[gu8GroupId].u8UltrasonicSWICUGroupId, &Reading1);
      //switch SW ICU to detect a falling edge
      SW_ICU_SetEdge(astrUltrasonicConfigParameters[gu8GroupId].u8UltrasonicSWICUGroupId, SW_ICU_FALLING_EDGE);
      //switch the flag state
      u8Flag=GETTING_FALLING_EDGE;
      break;
      
      //if the flag is set to detecting a rising edge
      case GETTING_FALLING_EDGE:
      SW_ICU_GetReading(astrUltrasonicConfigParameters[gu8GroupId].u8UltrasonicSWICUGroupId, &Reading2);
      SW_ICU_SetEdge(astrUltrasonicConfigParameters[gu8GroupId].u8UltrasonicSWICUGroupId, SW_ICU_RISING_EDGE);
      //switch the flag state
      u8Flag=GETTING_RISING_EDGE;
      //switch the Reading Flag state to indecate reading completion
      Reading_Flag=Reading_Complete;
      break;
   }
}
 

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to initialize the Ultrasonic Module
************************************************************************************/
enuErrorStatus_t Ultrasonic_Init(uint8_t u8GroupId)
{
   //initialize Trigger pin
	DIO_Init(&astrDIOConfigParameters[astrUltrasonicConfigParameters[u8GroupId].u8TrigPinGroupId]);
   //initialize Echo pin
	DIO_Init(&astrDIOConfigParameters[astrUltrasonicConfigParameters[u8GroupId].u8EchoPinGroupId]);
   //initialize software ICU
   SW_ICU_Init(astrUltrasonicConfigParameters[u8GroupId].u8UltrasonicSWICUGroupId);
   //set callback function to be executed in case of ICU event
   SW_ICU_SetCallback(astrUltrasonicConfigParameters[u8GroupId].u8UltrasonicSWICUGroupId, Get_Readings);
   //return OK status
   return E_OK;
}

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to send a trigger pulse to the Ultrasonic Module
************************************************************************************/
enuErrorStatus_t Ultrasonic_Trigger(uint8_t u8GroupId)
 {
      //reset the reading completion flag
      Reading_Flag=Reading_Not_Complete;
      //set the ICU to detect a rising edge
      SW_ICU_SetEdge(astrUltrasonicConfigParameters[u8GroupId].u8UltrasonicSWICUGroupId, SW_ICU_RISING_EDGE);
      //generate the trigger pulse
   	DIO_Write(astrUltrasonicConfigParameters[u8GroupId].u8TrigPinGroupId, HIGH);
      _delay_us(TRIGGER_DURATION_US);
   	DIO_Write(astrUltrasonicConfigParameters[u8GroupId].u8TrigPinGroupId, LOW);
      //reset the ICU unit
      SW_ICU_Reset_Reading(astrUltrasonicConfigParameters[u8GroupId].u8UltrasonicSWICUGroupId);
      //return an OK status
      return E_OK;
 }
 
/************************************************************************************
 * Parameters (in) : u8GroupId
 * Parameters (out): uint16_t *Distance, enuErrorStatus_t
 * Return value    : 1=E_OK - 0=E_ERROR
 * Description     : A function to check if ultrasonic reading is complete and return distance
 ************************************************************************************/
enuErrorStatus_t Ultrasonic_GetDistance(uint8_t u8GroupId, uint16_t *Distance)
{
	gu8GroupId = u8GroupId;
	//check for reading completion condition
	if (Reading_Flag)
	{
		//when echo signal has been received, calculate the distance
		*Distance=((Reading2-Reading1))/US_CM_FACTOR;
		//return an OK status
		return E_OK;
	}
	//return a NOK status
	return E_ERROR;
}

/************************************************************************************
 * Parameters (in) : u8GroupId
 * Parameters (out): uint16_t *Distance, enuErrorStatus_t
 * Return value    : 1=E_OK - 0=E_ERROR
 * Description     : A function to get ultrasonic reading using polling
 ************************************************************************************/
enuErrorStatus_t Ultrasonic_GetReading_Polling(uint8_t u8GroupId, uint32_t *Distance)
{
	gu8GroupId = u8GroupId;
	//reset the reading completion flag
   Reading_Flag=Reading_Not_Complete;
   //set the ICU to detect a rising edge
   SW_ICU_SetEdge(astrUltrasonicConfigParameters[u8GroupId].u8UltrasonicSWICUGroupId, SW_ICU_RISING_EDGE);
   //generate the trigger pulse
	DIO_Write(astrUltrasonicConfigParameters[u8GroupId].u8TrigPinGroupId, HIGH);
   _delay_us(TRIGGER_DURATION_US);
	DIO_Write(astrUltrasonicConfigParameters[u8GroupId].u8TrigPinGroupId, LOW);
   //wait for echo detection to complete
   while (!Reading_Flag);
   //calculate the distance
   *Distance=((Reading2-Reading1))/US_CM_FACTOR;
   //return an OK state
   return E_OK;
}

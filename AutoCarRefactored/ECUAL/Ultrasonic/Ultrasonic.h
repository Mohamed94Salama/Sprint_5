/*****************************************************************************
* Task: Autonomous Car
* File Name: Ultrasonic.h
* Description: Header file for Ultrasonic driver
* Author: Amr Mohamed
* Date: 9/8/2021
******************************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

/*Includes -----------------------------------------------------------------*/
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "SW_ICU.h"
#include "Ultrasonic_Cfg.h"

typedef struct
{
	/* Selecting the software ICU group id used */
	uint8_t u8UltrasonicSWICUGroupId;
	/* Selecting the ultrasonic trigger pin group id used */
	uint8_t u8TrigPinGroupId;
	/* Selecting the ultrasonic echo pin group id used */
	uint8_t u8EchoPinGroupId;
} strUltrasonicConfigType_t;

extern strUltrasonicConfigType_t astrUltrasonicConfigParameters[ULTRASONIC_GROUPS_NO];

/*API Prototypes -----------------------------------------------------------------*/
/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to initialize the Ultrasonic Module
************************************************************************************/
enuErrorStatus_t Ultrasonic_Init(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to send a trigger pulse to the Ultrasonic Module
************************************************************************************/
enuErrorStatus_t Ultrasonic_Trigger(uint8_t u8GroupId);

 /************************************************************************************
 * Parameters (in) : u8GroupId
 * Parameters (out): uint16_t *Distance, enuErrorStatus_t
 * Return value    : 1=E_OK - 0=E_ERROR
 * Description     : A function to check if ultrasonic reading is complete and return distance
 ************************************************************************************/
enuErrorStatus_t Ultrasonic_GetDistance(uint8_t u8GroupId, uint16_t *Distance);

/************************************************************************************
 * Parameters (in) : u8GroupId
 * Parameters (out): uint16_t *Distance, enuErrorStatus_t
 * Return value    : 1=E_OK - 0=E_ERROR
 * Description     : A function to get ultrasonic reading using polling
 ************************************************************************************/
enuErrorStatus_t Ultrasonic_GetReading_Polling(uint8_t u8GroupId, uint32_t *Distance);


#endif /* ULTRASONIC_H_ */

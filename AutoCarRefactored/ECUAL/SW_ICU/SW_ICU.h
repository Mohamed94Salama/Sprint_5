/*****************************************************************************
* Task: Autonomous Car
* File Name: SW_ICU.h
* Description: Header file for Software ICU driver
* Author: Amr Mohamed
* Date: 12/8/2021
******************************************************************************/


#ifndef SW_ICU_H_
#define SW_ICU_H_

/*Includes -----------------------------------------------------------------*/
#include "EXTINT.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "Register.h"
#include "DIO.h"
#include "Timer.h"
#include "SW_ICU_Cfg.h"

/*ENUMS -----------------------------------------------------------------*/
typedef enum {SW_ICU_FALLING_EDGE=2, SW_ICU_RISING_EDGE=3} enuSWICUTriggerMode_t;

typedef struct
{
	/* Selecting the software ICU trigger mode used */
	enuSWICUTriggerMode_t enuSWICUTriggerMode;
	/* Selecting the external interrupt group id used */
	uint8_t u8SWICUEXTINTGroupId;
	/* Selecting the timer group id used */
	uint8_t u8SWICUTimerGroupId;
	/* Selecting the maximum timer value */
	uint16_t u16TimerMax;
	/* Selecting the timer frequency used in KHz */
	uint8_t u8TimerFreqKHz;
	/* Assigning the address of the software ICU callback function */
	void (*pfSWICUCallback)(void);
} strSWICUConfigType_t;

extern strSWICUConfigType_t astrSWICUConfigParameters[SWICU_GROUPS_NO];

/*API PROTOTYPES ---------------------------------------------------*/
/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to initialize the Software ICU
************************************************************************************/
enuErrorStatus_t SW_ICU_Init(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in) : u8GroupId, enuSWICUTriggerMode
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to set the detection edge of the SW_ICU
************************************************************************************/
enuErrorStatus_t SW_ICU_SetEdge(uint8_t u8GroupId, enuSWICUTriggerMode_t enuSWICUTriggerMode);

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): *u32Buffer, enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to get the SW_ICU register reading
************************************************************************************/
enuErrorStatus_t SW_ICU_GetReading(uint8_t u8GroupId, uint32_t *u32Buffer);

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to reset the SW_ICU logic
************************************************************************************/
enuErrorStatus_t SW_ICU_Reset_Reading(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in) : u8GroupId, void(*pfSWICUCallback)(void)
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to set callback function to be called in case of ICU event
************************************************************************************/
enuErrorStatus_t SW_ICU_SetCallback(uint8_t u8GroupId, void(*pfSWICUCallback)(void));

#endif /* SW_ICU_H_ */

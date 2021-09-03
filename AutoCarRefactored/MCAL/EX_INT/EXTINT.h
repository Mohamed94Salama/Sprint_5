/*****************************************************************************
* Task       : External Interrupt driver
* File Name  : EXT_INT.h
* Description: header file for External Interrupt driver
* Author     : Kariman Karam
* Date       : 3/8/2021
******************************************************************************/

#ifndef EXT_INT_H_
#define EXT_INT_H_

/*- INCLUDES --------------------------------------------------*/
#include "EXTINT_Cfg.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "Register.h"

/*- ENUMS --------------------------------------------------*/
typedef enum
{
	EXT_LOW_MODE =0,
	EXT_CHANGE_MODE,
	EXT_FAILLING_MODE,
	EXT_RAISING_MODE

}enuEXTINTSenseControl_t;

typedef enum
{
	EXTINT_INT0=0,
	EXTINT_INT1,
	EXTINT_INT2
}enuEXTINTNum_t;

typedef struct
{
	/* Selecting the interrupt channel used */
	enuEXTINTNum_t enuEXTINTNum;
	/* Selecting the sense mode control of the interrupt channel used */
	enuEXTINTSenseControl_t enuEXTINTSenseControl;
	/* Assigning the address of the callback function */
	void(*pfEXTINTCallBack)(void);
} strEXTINTConfigType_t;

extern strEXTINTConfigType_t astrEXTINTConfigParameters[EXTINT_GROUPS_NO];

/*- FUNCTION PROTOTYPES ----------------------------------*/

/***********************************************************************************************
* Parameters (in) : strEXTINTConfigType
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize External Interrupt
************************************************************************************************/
enuErrorStatus_t EXTINT_Init(strEXTINTConfigType_t* strEXTINTConfigType);

/***********************************************************************************************
* Parameters (in) : u8GroupId, enuEXTINTSenseControl
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to set the sense mode of the External interrupt
************************************************************************************************/
enuErrorStatus_t EXTINT_SetSenseMode(uint8_t u8GroupId, enuEXTINTSenseControl_t enuEXTINTSenseControl);

/***********************************************************************************************
* Parameters (in) : u8GroupId, pfEXTINTCallBack
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to set the callback function of the External interrupt
************************************************************************************************/
enuErrorStatus_t EXTINT_SetCallBack(uint8_t u8GroupId, void(*pfEXTINTCallBack)(void));

#endif

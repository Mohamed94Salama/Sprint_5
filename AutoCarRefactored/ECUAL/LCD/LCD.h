
/*****************************************************************************
* Module     : LCD
* File Name  : LCD.h
* Description: Header file for the LCD driver
* Author     : Mohamed Ehab
* Date       : 24/7/2021
******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/*- INCLUDES -----------------------------------------------*/
#include "LCD_Cfg.h"
#include "DIO.h"
#include "TDelay.h"
#include "BIT_MANIPULATE.h"
#include "STD_TYPES.h"
#include <stdlib.h>


typedef enum
{
	LCD_FOUR_BIT_UPPER_PORT_PINS_DATA_MODE,
	LCD_FOUR_BIT_LOWER_PORT_PINS_DATA_MODE,
	LCD_EIGHT_BIT_DATA_MODE
} enuLCDDataMode_t;

typedef struct
{
	/* Selecting the LCD data mode type */
	enuLCDDataMode_t enuLCDDataMode;
	/* Selecting the group id number that corresponds RS pin */
	uint8_t u8RSPinDIOGroupId;
	/* Selecting the group id number that corresponds RW pin */
	uint8_t u8RWPinDIOGroupId;
	/* Selecting the group id number that corresponds E pin */
	uint8_t u8EPinDIOGroupId;
	/* Selecting the group id numbers that corresponds DATA pins {D0, D1, D2, D3, D4, D5, D6, D7} */
	uint8_t u8DataPinsDIOGroupIds[8];
	/* Selecting the group id number that corresponds TDelay module */
	uint8_t u8TDelayGroupId;
} strLCDConfigType_t;

extern strLCDConfigType_t astrLCDConfigParameters[LCD_GROUPS_NO];


/*- FUNCTION DECLARATIONS ----------------------------------*/
/*************************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to initialize the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_Init(uint8_t u8GroupId);

/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Data
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to display a character on the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_displayCharacter(uint8_t u8GroupId, uint8_t u8Data);

/*************************************************************************************************
* Parameters (in) : u8GroupId, *pu8Str
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to display a string on the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_displayString(uint8_t u8GroupId, const uint8_t *pu8Str);

/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Row, u8Col
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to set the cursor in a particular row and column
*************************************************************************************************/
enuErrorStatus_t LCD_goToRowColumn(uint8_t u8GroupId, uint8_t u8Row, uint8_t u8Col);

/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Row, u8Col, *pu8Str
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to display a string with particular row and column
*************************************************************************************************/
enuErrorStatus_t LCD_displayStringRowColumn(uint8_t u8GroupId, uint8_t u8Row, uint8_t u8Col,const uint8_t *pu8Str);

/*************************************************************************************************
* Parameters (in) : u8GroupId, u16Data
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to display integer
*************************************************************************************************/
enuErrorStatus_t LCD_intgerToString(uint8_t u8GroupId, uint16_t u16Data);

/*************************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to clear the screen of the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_clearScreen(uint8_t u8GroupId);

#endif /* LCD_H_ */

/*****************************************************************************
* Module     : LCD
* File Name  : LCD.h
* Description: Source file for the LCD driver
* Author     : Mohamed Ehab
* Date       : 24/7/2021
******************************************************************************/

/*- INCLUDES -----------------------------------------------*/
#include "LCD.h"

/*- PREPROCESSOR MACROS ------------------------------------*/
#define LCD_D0_GROUP_ID_INDEX					(0u)
#define LCD_D1_GROUP_ID_INDEX					(1u)
#define LCD_D2_GROUP_ID_INDEX					(2u)
#define LCD_D3_GROUP_ID_INDEX					(3u)
#define LCD_D4_GROUP_ID_INDEX					(4u)
#define LCD_D5_GROUP_ID_INDEX					(5u)
#define LCD_D6_GROUP_ID_INDEX					(6u)
#define LCD_D7_GROUP_ID_INDEX					(7u)

#define LCD_FOUR_BIT_MODE						(4)
#define LCD_HIGHEST_FOUR_PINS_OUTPUT		(0xF0)
#define LCD_LOWEST_FOUR_PINS_OUTPUT			(0x0F)
#define LCD_SHIFTING_VALUE						(4)
#define LCD_ROW0									(0)
#define LCD_ROW1									(1)
#define LCD_ROW2									(2)
#define LCD_ROW3									(3)

/* LCD Commands */
#define CLEAR_COMMAND                  	(0x01)
#define FOUR_BITS_DATA_MODE            	(0x02)
#define TWO_LINE_LCD_Four_BIT_MODE     	(0x28)
#define TWO_LINE_LCD_Eight_BIT_MODE    	(0x38)
#define CURSOR_OFF                     	(0x0C)
#define CURSOR_ON                      	(0x0E)
#define CURSOR_INCREMENT               	(0x06)
#define SET_CURSOR_LOCATION            	(0x80)

/*Flags*/
#define LCD_INIT_FLAG							(1)
#define LCD_SEND_COMMAND_FLAG					(2)
#define LCD_DISPLAY_CHARACTER_FLAG			(3)
#define	LCD_CLEAR_SCREEN_FLAG				(4)

/*Delays*/
#define DELAY_50_MS								(50)
#define DELAY_39_US								(39)
#define DELAY_1530_US							(1530)
#define DELAY_1_MS								(1)

/*Addresses*/
#define ADDRESS_ROW_1							(0x40)
#define ADDRESS_ROW_2							(0x10)
#define ADDRESS_ROW_3							(0x50)

/****/
#define SCREEN_WIDTH 							(16)
#define DECIMAL_BASE 							(10)

/*- ENUMS AND GLOBAL VARIABLES --------------------------------------------------*/
/* Enums defining the states of the LCD_Init function*/
typedef enum{START_INIT, INIT_DELAY, FB_MODE_C1_SENT, FB_MODE_C2_SENT, CURSOR_CMD_SENT, CURSOR_INC_CMD_SENT, LCD_CLEARED} LCDInitState_t;

/* Initializing 2 variables for the current state and the previous state of the LCD_Init*/
LCDInitState_t  enuInitPreviousState = LCD_CLEARED, enuInitCurrentState = START_INIT;

/*********************************************************************************************/

/* Enums defining the states of the LCD_sendCommand function*/
typedef enum{COMMAND_START, COMMAND_INIT, COMMAND1_CFG, COMMAND1_SENT, COMMAND2_CFG, COMMAND2_SENT} LCDSendCommand_t;

/* Initializing 2 variables for the current state and the previous state of the LCD_sendCommand*/
LCDSendCommand_t enuCommandCurrentState = COMMAND_START, enuCommandPreviousState = COMMAND2_SENT;

/******************************************************************************************/

/* Enums defining the states of the LCD_displayCharacter function*/
typedef enum{DATA_START, DATA_INIT, DATA1_CFG, DATA1_SENT, DATA2_CFG, DATA2_SENT} LCDSendData_t;

/* Initializing 2 variables for the current state and the previous state of the LCD_displayCharacter*/
LCDSendData_t enuDataCurrentState = DATA_START, enuDataPreviousState = DATA2_SENT;

/*****************************************************************************************/

/* Enums defining the states of the LCD_clearScreen function*/
typedef enum{CLEAR_UNDONE, CLEAR_PROGRESS, CLEAR_DONE} LCDClear_t;

/* Initializing 2 variables for the current state and the previous state of the LCD_clearScreen*/	
LCDClear_t enuClearState = CLEAR_UNDONE;

/*****************************************************************************************/
/* Flag for the callback function after having the timer finished counting */
static volatile uint8_t u8LCDFlag = 0;


/*- PRIVATE FUNCTIONS DECLARATIONS -------------------------*/
/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Command
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to send command to LCD
*************************************************************************************************/
static enuErrorStatus_t LCD_sendCommand(uint8_t u8GroupId, uint8_t u8Command);
/* The callback function after having the timer finished counting */
static void LCD_Callback(void);


/*- FUNCTIONS DEFINITIONS ----------------------------------*/
/*************************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to initialize the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_Init(uint8_t u8GroupId)
{
	/* Error handler initialization */
	static enuErrorStatus_t enuRetVar = E_ERROR;
	
	/* Making sure that the function won't proceed as long as the previous state is the current state */
	if(enuInitPreviousState != enuInitCurrentState)
	{
		switch(enuInitCurrentState) //Switching case on the current state 
		{
			/* The first state , Initializing the DIO pins */
			case START_INIT:
				/* Configure the control pins(E,RS,RW) as output pins */
				DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8RSPinDIOGroupId]);
				DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8RWPinDIOGroupId]);
				DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId]);

				/* Configure the highest 4 bits of the data port as output pins */
				if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_UPPER_PORT_PINS_DATA_MODE)
				{
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D4_GROUP_ID_INDEX]]);
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D5_GROUP_ID_INDEX]]);
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D6_GROUP_ID_INDEX]]);
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D7_GROUP_ID_INDEX]]);
				}
				/* Configure the lowest 4 bits of the data port as output pins */
				else if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_LOWER_PORT_PINS_DATA_MODE)
				{
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D0_GROUP_ID_INDEX]]);
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D1_GROUP_ID_INDEX]]);
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D2_GROUP_ID_INDEX]]);
					DIO_Init(&astrDIOConfigParameters[astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D3_GROUP_ID_INDEX]]);
				}
				else
				{
				}
				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuInitPreviousState = enuInitCurrentState;
				/* Making the flag equal to 1 to make it updating the LCD_Init state in the callback function*/
				u8LCDFlag = LCD_INIT_FLAG;
				/* Making a delay of 50ms using a timer */
				TDelay_ms(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_50_MS, LCD_Callback);
				break;
			
			/* The second state , Enabling the 4-bit mode */
			case INIT_DELAY:
				//4 bit mode command 1
				if(LCD_sendCommand(u8GroupId, FOUR_BITS_DATA_MODE))
				{
					/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
					enuInitPreviousState = enuInitCurrentState;

					/* Making the flag equal to 1 to make it updating the LCD_Init state in the callback function*/
					u8LCDFlag = LCD_INIT_FLAG;

					/* Making a delay of 39us using a timer */
					TDelay_us(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_39_US, LCD_Callback);
				}
				else
				{
				}
				break;
			
			/*The third state , configuring the number of lines and the font */
			case FB_MODE_C1_SENT:
				//4 bit mode command 2
				if(LCD_sendCommand(u8GroupId, TWO_LINE_LCD_Four_BIT_MODE)) /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
				{
					/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
					enuInitPreviousState = enuInitCurrentState;

					/* Making the flag equal to 1 to make it updating the LCD_Init state in the callback function*/
					u8LCDFlag = LCD_INIT_FLAG;

					/* Making a delay of 39us using a timer */
					TDelay_us(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_39_US, LCD_Callback);
				}
				else
				{
				}
				break;
			
			/* The third state , Adjusting the cursor appearance */
			case FB_MODE_C2_SENT:
				if(LCD_sendCommand(u8GroupId, CURSOR_OFF))
				{
					/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
					enuInitPreviousState = enuInitCurrentState;

					/* Making the flag equal to 1 to make it updating the LCD_Init state in the callback function*/
					u8LCDFlag = LCD_INIT_FLAG;

					/* Making a delay of 39us using a timer */
					TDelay_us(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_39_US, LCD_Callback);
				}
				else
				{
				}
				break;
			
			/* The forth state , updating the cursor move */
			case CURSOR_CMD_SENT:
				if(LCD_sendCommand(u8GroupId, CURSOR_INCREMENT))
				{
					/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
					enuInitPreviousState = enuInitCurrentState;

					/* Making the flag equal to 1 to make it updating the LCD_Init state in the callback function*/
					u8LCDFlag = LCD_INIT_FLAG;

					/* Making a delay of 39us using a timer */
					TDelay_us(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_39_US, LCD_Callback);
				}
				else
				{
				}
				break;
         
			/* The fifth state , indicating that the cursor options configured successfully , then sending the clear command */
         case CURSOR_INC_CMD_SENT:
				if(LCD_sendCommand(u8GroupId, CLEAR_COMMAND))
				{
					/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
					enuInitPreviousState = enuInitCurrentState;

					/* Making the flag equal to 1 to make it updating the LCD_Init state in the callback function*/
					u8LCDFlag = LCD_INIT_FLAG;

					/* Making a delay of 1530us using a timer */
					TDelay_us(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_1530_US, LCD_Callback);
				}
				else
				{
				}
				break;
			
			/* The sixth state , indicating that the clear command is sent successfully */
			case LCD_CLEARED:
				/* Returning the handler E_ON , indicating that the function done successfully */
				enuRetVar = E_OK;

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuInitPreviousState = enuInitCurrentState;
				break;
		}
	}
	else
	{		
		//do nothing when the previous state is the current state
	}
	return enuRetVar; //return the Error handler
	/***********************************************/
}//end LCD_init

/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Data
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to display a character on the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_displayCharacter(uint8_t u8GroupId, uint8_t u8Data)
{
	/* Error handler initialization */
	static enuErrorStatus_t enuRetVar = E_ERROR;
	
	/* Making sure that the function won't proceed as long as the previous state is the current state */
	if(enuDataPreviousState != enuDataCurrentState)
	{
		switch(enuDataCurrentState)		//Switching case on the current state 
		{
			/*The first state , Initializing the DIO pins*/
			case DATA_START:
				enuRetVar=E_ERROR;
				/* Data Mode RS=1 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8RSPinDIOGroupId, HIGH);

				/* write data to LCD so RW=0 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8RWPinDIOGroupId, LOW);

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuDataPreviousState = enuDataCurrentState;

				/* Moving to the next state */
				enuDataCurrentState++;
				break;
			
			/* The second state , sending the high 4 bits */
			case DATA_INIT:
				/* out the highest 4 bits of the required data to the data bus D4 --> D7 */
				if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_UPPER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D4_GROUP_ID_INDEX], (u8Data & 0x10) >> 4);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D5_GROUP_ID_INDEX], (u8Data & 0x20) >> 5);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D6_GROUP_ID_INDEX], (u8Data & 0x40) >> 6);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D7_GROUP_ID_INDEX], (u8Data & 0x80) >> 7);
				}
				/* out the highest 4 bits of the required data to the data bus D0 --> D3 */
				else if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_LOWER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D0_GROUP_ID_INDEX], (u8Data & 0x10) >> 4);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D1_GROUP_ID_INDEX], (u8Data & 0x20) >> 5);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D2_GROUP_ID_INDEX], (u8Data & 0x40) >> 6);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D3_GROUP_ID_INDEX], (u8Data & 0x80) >> 7);
				}
				else
				{
				}

				/* Enable LCD E=1 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, HIGH);

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuDataPreviousState = enuDataCurrentState;
				/* For updating the state */
				u8LCDFlag = LCD_DISPLAY_CHARACTER_FLAG;
				TDelay_ms(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_1_MS, LCD_Callback);
				break;
			
			/* The third state */
			case DATA1_CFG:
				/* Clear the E-pin */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, LOW);
			
				/* Moving to the next state*/
				enuDataCurrentState++;
				break;

			/* The forth state , sending the lower 4 bits */
			case DATA1_SENT:
				/* out the lowest 4 bits of the required data to the data bus D4 --> D7 */
				if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_UPPER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D4_GROUP_ID_INDEX], (u8Data & 0x01) >> 0);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D5_GROUP_ID_INDEX], (u8Data & 0x02) >> 1);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D6_GROUP_ID_INDEX], (u8Data & 0x04) >> 2);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D7_GROUP_ID_INDEX], (u8Data & 0x08) >> 3);
				}
				/* out the lowest 4 bits of the required data to the data bus D0 --> D3 */
				else if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_LOWER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D0_GROUP_ID_INDEX], (u8Data & 0x01) >> 0);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D1_GROUP_ID_INDEX], (u8Data & 0x02) >> 1);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D2_GROUP_ID_INDEX], (u8Data & 0x04) >> 2);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D3_GROUP_ID_INDEX], (u8Data & 0x08) >> 3);
				}
				else
				{
				}

				/* Enable LCD E=1 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, HIGH);

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuDataPreviousState = enuDataCurrentState;

				/* For updating the state */
				u8LCDFlag = LCD_DISPLAY_CHARACTER_FLAG;
				TDelay_ms(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_1_MS, LCD_Callback);
				break;

			/* The fifth state*/
			case DATA2_CFG:
				/* Clear the E-pin */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, LOW);

				/* Moving to the next state */
				enuDataCurrentState++;
				break;
			
			/* The sixth state*/
			case DATA2_SENT:
				/* Returning E_OK*/
				enuRetVar = E_OK;

				/* Re-initialize the current state and the previous state to start again from the same point *//* Re-initialize the */
				enuDataCurrentState = DATA_START;
				enuDataPreviousState = DATA2_SENT;
				break;
		}
	}
	else
	{
		// do nothing 	
	}
	return enuRetVar;
	
	/**************************/

}//end LCD_displayCharacter

/*************************************************************************************************
* Parameters (in) : u8GroupId, *pu8Str
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to display a string on the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_displayString(uint8_t u8GroupId, const uint8_t *pu8Str)
{
	/* Initialize the index*/
	static uint8_t u8I = 0;

   if (pu8Str == NULLPTR)
   {
      return E_ERROR;
   }
   
   /*Displaying a character as long as it is not a NULL*/
   if (pu8Str[u8I] != '\0')
   {
      if (LCD_displayCharacter(u8GroupId, pu8Str[u8I]))
      {
		  /* updating the index value */
         u8I++;
      }
   }
   else
   {
	   /* re-initialize the index value */
      u8I = 0;
      return E_OK;
   }
   return E_ERROR;
}//end LCD_displayString

/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Row, u8Col
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to set the cursor in a particular row and column
*************************************************************************************************/
enuErrorStatus_t LCD_goToRowColumn(uint8_t u8GroupId, uint8_t u8Row, uint8_t u8Col)
{
	/* Initializing the address and the flag */
	static uint8_t u8Address = 0;
	static uint8_t Flag = 0;
   
   if(!Flag)
   {
      /* first of all calculate the required address */
      switch(u8Row)
      {
			/* Row 0 */
         case LCD_ROW0:
				u8Address = u8Col;
				break;
			/* Row 1 */
         case LCD_ROW1:
				u8Address = u8Col+ADDRESS_ROW_1;
				break;
			/* Row 2 */
         case LCD_ROW2:
				u8Address = u8Col+ADDRESS_ROW_2;
				break;
			/* Row 3 */
         case LCD_ROW3:
				u8Address = u8Col+ADDRESS_ROW_3;
				break;
      }
      /* Adding the cursor location to the address*/
      u8Address |= SET_CURSOR_LOCATION;
      Flag++; //Incrementing the flag to go out of the block indicating that the address has been calculated
   }
			
	/* to write to a specific address in the LCD 
	 * we need to apply the corresponding command 0b10000000+Address */
   if (LCD_sendCommand(u8GroupId, u8Address))
   {
	   /* re-initialize the flag*/
      Flag = 0;
      return E_OK;
   }
	return E_ERROR;
}//end LCD_goToRowColumn

/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Row, u8Col, *pu8Str
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to display a string with particular row and column
*************************************************************************************************/
enuErrorStatus_t LCD_displayStringRowColumn(uint8_t u8GroupId, uint8_t u8Row, uint8_t u8Col,const uint8_t *pu8Str)
{
	/* go to to the required LCD position */
	LCD_goToRowColumn(u8GroupId, u8Row, u8Col);
	/* display the string */
	LCD_displayString(u8GroupId, pu8Str);
	return E_OK;
}//end LCD_displayStringRowColumn


/*************************************************************************************************
* Parameters (in) : u8GroupId, u16Data
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to display integer
*************************************************************************************************/
enuErrorStatus_t LCD_intgerToString(uint8_t u8GroupId, uint16_t u16Data)
{
	/* String to hold the ascii result */
   uint8_t strBuff[SCREEN_WIDTH];
   itoa(u16Data, (char*)strBuff, DECIMAL_BASE); /* 10 for decimal */
   return LCD_displayString(u8GroupId, strBuff);
}//end LCD_intgerToString

/*************************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to clear the screen of the LCD
*************************************************************************************************/
enuErrorStatus_t LCD_clearScreen(uint8_t u8GroupId)
{
	/* Error handler initialization */
	static enuErrorStatus_t enuRetVar = E_ERROR;
	/*For updating the state*/
	u8LCDFlag = LCD_CLEAR_SCREEN_FLAG;
	switch(enuClearState)
	{
		/* The first state */
		case CLEAR_UNDONE:
			LCD_sendCommand(u8GroupId, CLEAR_COMMAND);
			TDelay_us(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_1530_US, LCD_Callback);
			enuClearState = CLEAR_PROGRESS;
			break;

		/*The second state*/
		case CLEAR_PROGRESS:
			break;

		/* The thirs state*/
		case CLEAR_DONE:
			enuRetVar = E_OK;
	}
	
	return enuRetVar;
}//end LCD_clearScreen


/*************************************************************************************************
* Parameters (in) : u8GroupId, u8Command
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR , E_OK
* Description     : A function to send command to LCD
*************************************************************************************************/
static enuErrorStatus_t LCD_sendCommand(uint8_t u8GroupId, uint8_t u8Command)
{
	/* Error handler initialization */
	static enuErrorStatus_t enuRetVar=E_ERROR;

	/* Making sure that the function won't proceed as long as the previous state is the current state */
	if(enuCommandPreviousState != enuCommandCurrentState)
	{
		switch(enuCommandCurrentState)  //Switching case on the current state
		{
			/* The first state , starting the function and initializing the DIO pins */
			case COMMAND_START:
				enuRetVar=E_ERROR;
				/* Instruction Mode RS=0 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8RSPinDIOGroupId, LOW);
				/* write data to LCD so RW=0 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8RWPinDIOGroupId, LOW);

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuCommandPreviousState = enuCommandCurrentState;

				/* Moving to the next state */
				enuCommandCurrentState++;
				break;

			/* The second state , sending the 4 high bits of the command on the data bus D4 --> D7*/
			case COMMAND_INIT:
				//4 bit mode command 1
				/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
				if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_UPPER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D4_GROUP_ID_INDEX], (u8Command & 0x10) >> 4);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D5_GROUP_ID_INDEX], (u8Command & 0x20) >> 5);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D6_GROUP_ID_INDEX], (u8Command & 0x40) >> 6);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D7_GROUP_ID_INDEX], (u8Command & 0x80) >> 7);
				}
				/* out the highest 4 bits of the required command to the data bus D0 --> D3 */
				else if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_LOWER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D0_GROUP_ID_INDEX], (u8Command & 0x10) >> 4);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D1_GROUP_ID_INDEX], (u8Command & 0x20) >> 5);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D2_GROUP_ID_INDEX], (u8Command & 0x40) >> 6);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D3_GROUP_ID_INDEX], (u8Command & 0x80) >> 7);
				}
				else
				{
				}

				/* Enable LCD E=1 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, HIGH);

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuCommandPreviousState = enuCommandCurrentState;
				u8LCDFlag = LCD_SEND_COMMAND_FLAG;
				TDelay_ms(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_1_MS, LCD_Callback);
				break;

			/* The third state , Indicating that the previous state is done successfully , then clearing the E pin*/
			case COMMAND1_CFG:
				/* clear the E-pin */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, LOW);

				/* Moving to the next state */
				enuCommandCurrentState++;
				break;

			/* The forth state , indicating the the high 4 bits are sent successfully */
			case COMMAND1_SENT:
				/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
				if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_UPPER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D4_GROUP_ID_INDEX], (u8Command & 0x01) >> 0);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D5_GROUP_ID_INDEX], (u8Command & 0x02) >> 1);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D6_GROUP_ID_INDEX], (u8Command & 0x04) >> 2);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D7_GROUP_ID_INDEX], (u8Command & 0x08) >> 3);
				}
				/* out the lowest 4 bits of the required command to the data bus D0 --> D3 */
				else if(astrLCDConfigParameters[u8GroupId].enuLCDDataMode == LCD_FOUR_BIT_LOWER_PORT_PINS_DATA_MODE)
				{
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D0_GROUP_ID_INDEX], (u8Command & 0x01) >> 0);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D1_GROUP_ID_INDEX], (u8Command & 0x02) >> 1);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D2_GROUP_ID_INDEX], (u8Command & 0x04) >> 2);
					DIO_Write(astrLCDConfigParameters[u8GroupId].u8DataPinsDIOGroupIds[LCD_D3_GROUP_ID_INDEX], (u8Command & 0x08) >> 3);
				}
				else
				{
				}

				/* Enable LCD E=1 */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, HIGH);

				/* Making the current state equal to the previous state to make sure that the function won't proceed till the timer finishes counting */
				enuCommandPreviousState = enuCommandCurrentState;
				u8LCDFlag = LCD_SEND_COMMAND_FLAG;
				TDelay_ms(astrLCDConfigParameters[u8GroupId].u8TDelayGroupId, DELAY_1_MS, LCD_Callback);
				break;

			/* The fifth state */
			case COMMAND2_CFG:
				/* clear the E-pin */
				DIO_Write(astrLCDConfigParameters[u8GroupId].u8EPinDIOGroupId, LOW);

				/*Moving to the next state*/
				enuCommandCurrentState++;
				break;

			/*The sixth state */
			case COMMAND2_SENT:
				/*Returning E_OK*/
				enuRetVar = E_OK;

				/* Re-initialize the current state and the previous state to start again from the same point */
				enuCommandCurrentState = COMMAND_START;
				enuCommandPreviousState = COMMAND2_SENT;
				break;
		}
	}
	else
	{
		// do nothing
	}
	return enuRetVar;
	/**************************/
}//end LCD_sendCommand

/* The callback function after having the timer finished counting */
static void LCD_Callback(void)
{
	if(u8LCDFlag == LCD_INIT_FLAG) //init
	{
		enuInitCurrentState++; //Moving to the next state for the LCD_Init function
	}
   else if(u8LCDFlag == LCD_SEND_COMMAND_FLAG)//send command
	{
		enuCommandCurrentState++;	//Moving to the next state for the LCD_sendCommand function
	}
   else if(u8LCDFlag == LCD_DISPLAY_CHARACTER_FLAG)///send data
	{
		enuDataCurrentState++;  //Moving to the next state for the LCD_displayCharacter function
	}
   else if(u8LCDFlag == LCD_CLEAR_SCREEN_FLAG)//clear
	{
		enuClearState++;  //Moving to the next state for the LCD_clearScreen function
	}

	TDelay_Stop(astrLCDConfigParameters[0].u8TDelayGroupId); //Resetting the timer to count again
}

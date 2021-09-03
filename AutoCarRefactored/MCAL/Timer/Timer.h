/*****************************************************************************
* Task: TIMER_DRIVER
* File Name: Timer.h
* Description: File containing function prototypes for Timer.c
* Author: Mohamed Ehab
* Date: 10/7/2021
******************************************************************************/

#ifndef __TIMER__
#define __TIMER__

#include "Timer_Cfg.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "Register.h"

typedef enum
{
	TIMER0,
	TIMER1,
	TIMER2
}enuTimerModule_t;

typedef enum
{
	TIMER_NORMAL_MODE,
	TIMER_PHASECORRECT_MODE,
	TIMER_CTC_MODE,
	TIMER_FASTPWM_MODE
}enuTimerMode_t;

typedef enum
{
	TIMER_STOP,
	TIMER_SCALER_1,
	TIMER_SCALER_8,
	TIMER_SCALER_64,
	TIMER_SCALER_256,
	TIMER_SCALER_1024,
	EXTERNAL_FALLING,
	EXTERNAL_RISING
}enuTimerScaler_t;

typedef enum
{
	OC_DISCONNECTED,
	OC_TOGGLE,
	OC_NON_INVERTING,
	OC_INVERTING
}enuOCMode_t;

typedef enum
{
	TIMER1_NOT_SELECTED,
	TIMER1_OCA,
	TIMER1_OCB
}enuTimer1OCSelect_t;

typedef struct
{
	/* Selecting the timer module */
	enuTimerModule_t enuTimerModule;
	/* Selecting the timer mode */
	enuTimerMode_t enuTimerMode;
	/* Selecting the OC channel (for timer 1 only) */
	enuTimer1OCSelect_t enuTimer1OCSelect;
	/* Selecting the timer prescaler */
	enuTimerScaler_t enuTimerScaler;
} strTimerConfigType_t;

extern strTimerConfigType_t astrTimerConfigParameters[TIMER_GROUPS_NO];

//Initialization functions

/************************************************************************************
* Parameters (in): strTimerConfigType
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to initialize timer module according to the sent parameters
************************************************************************************/
enuErrorStatus_t Timer_Init(strTimerConfigType_t* strTimerConfigType);

/************************************************************************************
* Parameters (in): u8GroupId, enuOCMode
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to initialize the OCR pin for use in PWM generation
************************************************************************************/
enuErrorStatus_t Timer_OCMode(uint8_t u8GroupId, enuOCMode_t enuOCMode);
 
//Interrupt functions
/************************************************************************************
* Parameters (in): u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to enable overflow interrupt
************************************************************************************/
enuErrorStatus_t Timer_OVInterruptEnable(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in): u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to disable overflow interrupt
************************************************************************************/
enuErrorStatus_t Timer_OVInterruptDisable(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in): u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to enable output compare interrupt
************************************************************************************/
enuErrorStatus_t Timer_OCInterruptEnable(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in): u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to disable output compare interrupt
************************************************************************************/
enuErrorStatus_t Timer_OCInterruptDisable(uint8_t u8GroupId);

//Timer Ctrl Functions
/************************************************************************************
* Parameters (in): u8GroupId, u64TimerValue, void(*pfCallback)(void)
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to start the timer and set a callback function to be called when time runs up
************************************************************************************/
enuErrorStatus_t Timer_Start(uint8_t u8GroupId, uint64_t u64TimerValue, void(*pfCallback)(void));

/************************************************************************************
* Parameters (in): u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to stop the timer if running
************************************************************************************/
enuErrorStatus_t Timer_Stop(uint8_t u8GroupId);

/************************************************************************************
* Parameters (in): u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value: 1=time's up or 0=timer is still running
* Description: A function to check if a previously set up timer is still running or not
************************************************************************************/
enuErrorStatus_t Timer_GetStatus(uint8_t u8GroupId);



/******************************************************************************************/

/********************************** Timer 1 Functions *************************************/

typedef enum{
   TIMER1_STOP=0,
   TIMER1_SCALER_1,
   TIMER1_SCALER_8,
   TIMER1_SCALER_64,
   TIMER1_SCALER_256,
   TIMER1_SCALER_1024,
   EXTERNAL0_FALLING,
   EXTERNAL0_RISING
}enuTimer1Scaler_t;

typedef enum
{
   TIMER1_NORMAL_MODE=0,
   TIMER1_PWM_PHASE_8BIT_MODE,
   TIMER1_PWM_PHASE_9BIT_MODE,
   TIMER1_PWM_PHASE_10BIT_MODE,
   TIMER1_CTC_OCRA_TOP_MODE,
   TIMER1_FASTPWM_8BIT_MODE,
   TIMER1_FASTPWM_9BIT_MODE,
   TIMER1_FASTPWM_10BIT_MODE,
   TIMER1_PWM_PHASE_FREQ_ICR_TOP_MODE,
   TIMER1_PWM_PHASE_FREQ_OCRA_TOP_MODE,
   TIMER1_PWM_PHASE_ICR_TOP_MODE,
   TIMER1_PWM_PHASE_OCRA_TOP_MODE,
   TIMER1_CTC_ICR_TOP_MODE,
   /* 13 IS RESERVED */
   TIMER1_FASTPWM_ICR_TOP_MODE=14,
   TIMER1_FASTPWM_OCRA_TOP_MODE
}enuTimer1Mode_t;


/************************************************************************************
* Parameters (in): enuTimer1Mode_t enuMode,enuTimer1Scaler_t enuScaler
* Parameters (out): enuErrorStatus_t
* Return value: 1=SUCCESS or 0=FAIL
* Description: A function to initialize timer 1 according to the sent parameters
************************************************************************************/
enuErrorStatus_t Timer1_Init( enuTimer1Mode_t enuMode,enuTimer1Scaler_t enuScaler);

enuErrorStatus_t Timer1_OVF_SetCallback(void (*local_Fptr)(void));

enuErrorStatus_t Timer1_GetOVFCount(uint32_t *buffer);
enuErrorStatus_t Timer1_ResetOVFCount(void);


#endif /* __TIMER__ */

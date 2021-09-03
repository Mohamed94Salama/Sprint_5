/*****************************************************************************
* Task       : TIMER_DRIVER
* File Name  : T_Delay.h
* Description: Source file for timer delay
* Author     : Kariman Karam
* Date       : 8/12/2021
******************************************************************************/

/*Includes ---------------------------------------------------*/
#include "TDelay.h"

/*Preprocessor Macros ---------------------------------------------------*/
#define TDELAY_THOUSAND_MULTIPLIER										(1000)

/*Functions Declarations ---------------------------------------------------*/
#if 0
/************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to initialize the delay function
************************************************************************************/
enuErrorStatus_t TDelay_Init(void)
{
	/*initialize timer0 to work on normal mode with prescaller 8, tick time = 1 us*/
	T0_Init(TIMER0_NORMAL_MODE, TIMER0_SCALER_8);
	/*enable OVF interrupt*/
	T0_OV_InterruptEnable();
	return E_OK;
}//end TDelay_Init
#endif
/************************************************************************************
* Parameters (in) : u8TimerGroupId, u64TimerValue, void(*pfCallback)(void)
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to make delay with ms
************************************************************************************/
enuErrorStatus_t TDelay_ms(uint8_t u8TimerGroupId, uint64_t u64Time_ms, void(*pfCallback)(void))
{
	enuErrorStatus_t enuStatus = E_ERROR;
	/*Convert ms to us*/
	u64Time_ms *= TDELAY_THOUSAND_MULTIPLIER;

	//turn on timer interrupts
	Timer_OVInterruptEnable(u8TimerGroupId);
	/*start timer to count in ms, send callBack function*/
	Timer_Start(u8TimerGroupId, u64Time_ms, pfCallback);
	enuStatus = Timer_GetStatus(u8TimerGroupId);

	return enuStatus;
}//end TDelay_ms

/************************************************************************************
* Parameters (in) : u8TimerGroupId, u64TimerValue, void(*pfCallback)(void)
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to make delay with us
************************************************************************************/
enuErrorStatus_t TDelay_us(uint8_t u8TimerGroupId, uint64_t u64Time_us, void(*pfCallback)(void))
{
	enuErrorStatus_t enuStatus = E_ERROR;

	//turn on timer interrupts
	Timer_OVInterruptEnable(u8TimerGroupId);
	/*start timer to count in ms, send callBack function*/
	Timer_Start(u8TimerGroupId, u64Time_us, pfCallback);
	enuStatus = Timer_GetStatus(u8TimerGroupId);

	return enuStatus;
}//end TDelay_us

/************************************************************************************
* Parameters (in) : u8TimerGroupId
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to stop the timer delay
************************************************************************************/
enuErrorStatus_t TDelay_Stop(uint8_t u8TimerGroupId)
{
	enuErrorStatus_t enuStatus = E_ERROR;

	enuStatus = Timer_Stop(u8TimerGroupId);

	return enuStatus;
}

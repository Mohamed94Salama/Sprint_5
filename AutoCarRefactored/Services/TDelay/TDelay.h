/*****************************************************************************
* Task       : TIMER_DRIVER
* File Name  : T_Delay.h
* Description: Header file for timer delay
* Author     : Kariman Karam
* Date       : 8/12/2021
******************************************************************************/

#ifndef T_DELAY_H_
#define T_DELAY_H_

#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "Timer.h"
#include "Register.h"


/************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to initialize the delay function
************************************************************************************/
enuErrorStatus_t TDelay_Init(void);

/************************************************************************************
* Parameters (in) : u8TimerGroupId, u64TimerValue, void(*pfCallback)(void)
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to make delay with ms
************************************************************************************/
enuErrorStatus_t TDelay_ms(uint8_t u8TimerGroupId, uint64_t u64Time_ms, void(*pfCallback)(void));

/************************************************************************************
* Parameters (in) : u8TimerGroupId, u64TimerValue, void(*pfCallback)(void)
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to make delay with us
************************************************************************************/
enuErrorStatus_t TDelay_us(uint8_t u8TimerGroupId, uint64_t u64Time_us, void(*pfCallback)(void));

/************************************************************************************
* Parameters (in) : u8TimerGroupId
* Parameters (out): None
* Return value    : enuErrorStatus_t
* Description     : A function to stop the timer delay
************************************************************************************/
enuErrorStatus_t TDelay_Stop(uint8_t u8TimerGroupId);

#endif /* T_DELAY_H_ */

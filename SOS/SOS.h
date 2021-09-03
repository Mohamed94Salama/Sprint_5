/*
 * SOS.h
 *
 * Created: 8/30/2021 1:36:17 PM
 *  Author: Mohamed Samy
 */ 
#ifndef SOS_H_
#define SOS_H_

/*- INCLUDES -----------------------------------------------*/
#include "DIO.h"
#include "TM.h"
#include "SOS_Cfg.h"
#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"


/*- STRUCTS AND UNIONS -------------------------------------*/
typedef struct
{
	/* Selecting the periodicity of the selected task */
	uint8_t u8Periodicity;
	/* Assigning the pointer to the task function */
	void (*pfTask)(void);
} strSOSConfigTask_t;


/*- APIS DECLARATIONS --------------------------------------*/
/*************************************************************************************************
* Parameters (in) : u8TimerGroupId, u64TimerDelayValue
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to initialize the timer used in SOS 
*************************************************************************************************/
enuErrorStatus_t SOS_Init(uint8_t u8TimerGroupId, uint64_t u64TimerDelayValue);

/*************************************************************************************************
* Parameters (in) : u8Priority, strSOSConfigTask
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to create a certain task
*************************************************************************************************/
enuErrorStatus_t SOS_CreateTask(uint8_t u8Priority, strSOSConfigTask_t* strSOSConfigTask);

/*************************************************************************************************
* Parameters (in) : u8Priority
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to delete a certain task
*************************************************************************************************/
enuErrorStatus_t SOS_DeleteTask(uint8_t u8Priority);

/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): void
* Return Value    : void
* Description     : A function to run task(s) using the SOS
*************************************************************************************************/
void SOS_Run(void);

#endif /* SOS_H_ */
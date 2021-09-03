/*
 * SOS.c
 *
 * Created: 8/30/2021 1:36:06 PM
 *  Author: Mohamed Samy
 */ 

/*- INCLUDES -----------------------------------------------*/
#include "SOS.h"


/*- LOCAL FUNCTIONS PROTOTYPES -----------------------------*/
/*************************************************************************************************
* Parameters (in) : strSOSConfigTask
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to check if the input struct is empty or not
*************************************************************************************************/
static enuErrorStatus_t SOS_IsStructEmpty(strSOSConfigTask_t* strSOSConfigTask);

/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): void
* Return Value    : void
* Description     : A function to be called after an ISR has occured (Callback Function)
*************************************************************************************************/
static void SOS_TimerCallback(void);


/*- GLOBAL STATIC VARIABLES --------------------------------*/
static strSOSConfigTask_t gastrSOSConfigTaskParameters[SOS_NO_OF_TASKS];
static strSOSConfigTask_t gstrSOSEmptyTask = {0, NULLPTR};
static uint64_t gu64TickCounts = 0;
static uint8_t gu8Flag = LOW;

/*- LOCAL FUNCTIONS IMPLEMENTATION -------------------------*/
/*************************************************************************************************
* Parameters (in) : strSOSConfigTask
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to check if the input struct is empty or not
*************************************************************************************************/
static enuErrorStatus_t SOS_IsStructEmpty(strSOSConfigTask_t* strSOSConfigTask)
{
	enuErrorStatus_t enuErrorStatus;
	
	/* Checking if the attributes of the input struct are null */
	if((strSOSConfigTask->u8Periodicity == 0) && (strSOSConfigTask->pfTask == NULLPTR))
	{
		enuErrorStatus = E_OK;
	}
	else
	{
		enuErrorStatus = E_ERROR;
	}
	return enuErrorStatus;
}

/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): void
* Return Value    : void
* Description     : A function to be called after an ISR has occured (Callback Function)
*************************************************************************************************/
static void SOS_TimerCallback(void)
{
	/* Incrementing the value of the tick count */
	gu64TickCounts++;
	/* Raising the flag to acknowledge the scheduler that an ISR has occured */
	gu8Flag = HIGH;
}


/*- APIS IMPLEMENTATION ------------------------------------*/
/*************************************************************************************************
* Parameters (in) : u8TimerGroupId, u64TimerDelayValue
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to initialize the timer used in SOS
*************************************************************************************************/
enuErrorStatus_t SOS_Init(uint8_t u8TimerGroupId, uint64_t u64TimerDelayValue)
{
	enuErrorStatus_t enuRetVar;
	
	/* Initializing the timer module */
	enuRetVar = Timer_Init();
	if(enuRetVar == E_OK)
	{
		/* Starting the selected timer with the selected tick time value */
		enuRetVar = Timer_Start(u8TimerGroupId, u64TimerDelayValue, SOS_TimerCallback);		
	}
	else
	{
	}
	return enuRetVar; 
}

/*************************************************************************************************
* Parameters (in) : u8Priority, strSOSConfigTask
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to create a certain task
*************************************************************************************************/
enuErrorStatus_t SOS_CreateTask(uint8_t u8Priority, strSOSConfigTask_t* strSOSConfigTask)
{
	enuErrorStatus_t enuRetVar;
	
	/* Checking if the selected task struct is empty, the task priority is less than the maximum number of tasks,
		and the input struct is not empty */
	if((SOS_IsStructEmpty(&gastrSOSConfigTaskParameters[u8Priority]) == E_OK) && 
		(u8Priority < SOS_NO_OF_TASKS) && (strSOSConfigTask != NULLPTR))
	{
		/* Assigning the input struct parameters into the selected priority struct location inside the tasks array */
		gastrSOSConfigTaskParameters[u8Priority] = *strSOSConfigTask;			
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters (in) : u8Priority
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to delete a certain task
*************************************************************************************************/
enuErrorStatus_t SOS_DeleteTask(uint8_t u8Priority)
{
	enuErrorStatus_t enuRetVar;

	/* Checking if the selected task struct is not empty and the task priority is less than the maximum number of tasks */
	if((SOS_IsStructEmpty(&gastrSOSConfigTaskParameters[u8Priority]) != E_OK) && (u8Priority < SOS_NO_OF_TASKS))
	{
		/* Clearing the task parameters from the task array to delete it */ 
		gastrSOSConfigTaskParameters[u8Priority] = gstrSOSEmptyTask;
		enuRetVar = E_OK;
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): void
* Return Value    : void
* Description     : A function to run task(s) using the SOS
*************************************************************************************************/
void SOS_Run(void)
{
	uint8_t u8TaskId;
	
	while(1)
	{
		/* Checking if the ISR flag is raised */
		if(gu8Flag == HIGH)
		{
			/* Looping on each task to implement them based on priority */
			for(u8TaskId = 0; u8TaskId < SOS_NO_OF_TASKS; u8TaskId++)
			{
				/* Checking if the selected task struct is not empty and the task's periodicity has come */
				if((SOS_IsStructEmpty(&gastrSOSConfigTaskParameters[u8TaskId]) != E_OK) && 
					((gu64TickCounts % gastrSOSConfigTaskParameters[u8TaskId].u8Periodicity) == 0))
				{
					/* Calling the task's function */ 
					gastrSOSConfigTaskParameters[u8TaskId].pfTask();
				}
				else
				{
				}
			}	
			/* Lowering the ISR flag to detect the next tick time */
			gu8Flag = LOW;	
		}
		else
		{	
		}
	}
}



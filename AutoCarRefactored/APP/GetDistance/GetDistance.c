/*- INCLUDES -----------------------------------------------*/
#include "GetDistance.h"

#define GETDISTANCE_ULTRASONIC_GROUP_ID							(0u)

/*- FUNCTION DEFINITIONS -----------------------------------*/
/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to initialize all APIs responsible for getting distance
*************************************************************************************************/
enuErrorStatus_t GetDistance_Init(void)
{
	enuErrorStatus_t enuRetVar;

	/* Initializing the ultrasonic sensor module to start */
	enuRetVar = Ultrasonic_Init(GETDISTANCE_ULTRASONIC_GROUP_ID);
	gu8UltrasonicFlag = LOW;
	return enuRetVar;
}

/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to get the current distance of the robot from any obstacle
*************************************************************************************************/
enuErrorStatus_t GetDistance_Update(void)
{
	enuErrorStatus_t enuRetVar;

	if(gu8UltrasonicFlag == HIGH)
	{
		/* Getting the distance read from the ultrasonic sensor */
		enuRetVar = Ultrasonic_GetDistance(GETDISTANCE_ULTRASONIC_GROUP_ID, &gu16Distance);
		if(enuRetVar == E_OK)
		{
			gu8UltrasonicFlag = LOW;
		}
		else
		{
		}
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}

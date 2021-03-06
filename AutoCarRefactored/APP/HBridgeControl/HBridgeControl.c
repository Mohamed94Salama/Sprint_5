/*- INCLUDES -----------------------------------------------*/
#include "HBridgeControl.h"

/*- PREPROCESSOR MACROS ------------------------------------*/
#define HBRIDGECONTROL_GROUP_ID								(0u)
#define HBRIDGECONTROL_SPEED									(30u)
#define HBRIDGECONTROL_MAX_DISTANCE							(55u)
#define HBRIDGECONTROL_MIN_DISTANCE							(45u)

/*- FUNCTION DEFINITIONS -----------------------------------*/
/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to initialize all APIs responsible for robot control
*************************************************************************************************/
enuErrorStatus_t HBridgeControl_Init(void)
{
	enuErrorStatus_t enuRetVar;

	/* Initializing the H-bridge module according to the configuration parameters */
	enuRetVar = HBridge_Init(HBRIDGECONTROL_GROUP_ID);
	if(enuRetVar == E_OK)
	{
		/* Setting the speed of the H-bridge to 40% of its speed */
		enuRetVar = HBridge_SetSpeed(HBRIDGECONTROL_GROUP_ID, HBRIDGECONTROL_SPEED);
		if(enuRetVar == E_OK)
		{
			/* Triggering the H-bridge's motors to start movement */
			enuRetVar = HBridge_Start(HBRIDGECONTROL_GROUP_ID);
		}
		else
		{
		}
	}
	else
	{
	}
	return enuRetVar;
}

/*************************************************************************************************
* Parameters (in) : void
* Parameters (out): enuErrorStatus_t
* Return Value    : E_ERROR, E_OK
* Description     : A function to control the movement of the robot
*************************************************************************************************/
enuErrorStatus_t HBridgeControl_Update(void)
{
	enuErrorStatus_t enuRetVar;

	if((gu16Distance >= HBRIDGECONTROL_MIN_DISTANCE) && (gu16Distance <= HBRIDGECONTROL_MAX_DISTANCE))
	{
		/* Setting the direction of H-bridge's motors to turn right */
		enuRetVar = HBridge_SetDirection(HBRIDGECONTROL_GROUP_ID, TURN_RIGHT);
	}
	else if(gu16Distance < HBRIDGECONTROL_MIN_DISTANCE)
	{
		/* Setting the direction of H-bridge's motors to move backward */
		enuRetVar = HBridge_SetDirection(HBRIDGECONTROL_GROUP_ID, MOVE_BACKWARD);
	}
	else if(gu16Distance > HBRIDGECONTROL_MAX_DISTANCE)
	{
		/* Setting the direction of H-bridge's motors to move forward */
		enuRetVar = HBridge_SetDirection(HBRIDGECONTROL_GROUP_ID, MOVE_FORWARD);
	}
	else
	{
		enuRetVar = E_ERROR;
	}
	return enuRetVar;
}


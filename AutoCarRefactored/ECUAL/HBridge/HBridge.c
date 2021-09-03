#include "HBridge.h"


/* Internal Macros */
#define HBRIDGE_MODULES_MAX_NO																				(10u)
#define HBRIDGE_CLOCK_MIN_VALUE																				(-1l)
#define HBRIDGE_CLOCK_MAX_VALUE																				(8u)
#define HBRIDGE_MOTOR_SPEED_MIN_VALUE																		(0u)
#define HBRIDGE_MOTOR_SPEED_MAX_VALUE																		(100u)
#define HBRIDGE_MOTOR_DIRECTION_MIN_VALUE																	(-1l)
#define HBRIDGE_MOTOR_DIRECTION_MAX_VALUE																	(4u)

typedef enum {HBRIDGE_INIT_UNDONE, HBRIDGE_INIT_DONE} enuHBridgeState_t;
static enuHBridgeState_t aenuHBridgeState[HBRIDGE_GROUPS_NO] = {HBRIDGE_INIT_UNDONE};

static uint8_t gu8HBridgeMotorSpeed = 0;


/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Initialize the H Bridge module according to the configuration
*************************************************************************************************/
enuErrorStatus_t HBridge_Init(uint8_t u8GroupId)
{
   enuErrorStatus_t enuRetVar;

   if((astrHBridgeConfigParameters[u8GroupId].enuHBridgeEnablePinsControl == DIO_ON_ENABLE_PINS) ||
   	(astrHBridgeConfigParameters[u8GroupId].enuHBridgeEnablePinsControl == PWM_ON_ENABLE_PINS))
   {
      if(astrHBridgeConfigParameters[u8GroupId].enuHBridgeEnablePinsControl == DIO_ON_ENABLE_PINS)
      {
   		/* Initializing the DIO pin for motor 1 enable pin */
   		DIO_Init(&astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor1EnablePinGroupId]);
   		/* Initializing the DIO pin for motor 2 enable pin */
   		DIO_Init(&astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor2EnablePinGroupId]);
      }
      else
      {
   		/* Initializing the PWM module for motor 1 enable pin */
   		PWM_Init(&astrPWMConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor1EnablePinGroupId]);
   		/* Initializing the PWM module for motor 2 enable pin */
   		PWM_Init(&astrPWMConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor2EnablePinGroupId]);
      }
   	/* Initializing the DIO pin for motor 1A direction pin */
   	DIO_Init(&astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor1ADirectionPinGroupId]);
   	/* Initializing the DIO pin for motor 1B direction pin */
   	DIO_Init(&astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor1BDirectionPinGroupId]);
   	/* Initializing the DIO pin for motor 2A direction pin */
   	DIO_Init(&astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor2ADirectionPinGroupId]);
   	/* Initializing the DIO pin for motor 2B direction pin */
   	DIO_Init(&astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor2BDirectionPinGroupId]);
   	aenuHBridgeState[u8GroupId] = HBRIDGE_INIT_DONE;
   	enuRetVar = E_OK;
   }
   else
   {
	   enuRetVar = E_ERROR;
   }
   return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: u8GroupId, u8HBridgeMotorSpeed
* Return Value	: enuErrorStatus_t
* Description  : Set the speed of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_SetSpeed(uint8_t u8GroupId, uint8_t u8HBridgeMotorSpeed)
{
   enuErrorStatus_t enuRetVar;
   
   if((aenuHBridgeState[u8GroupId] == HBRIDGE_INIT_DONE) 		&& (astrHBridgeConfigParameters[u8GroupId].enuHBridgeEnablePinsControl == PWM_ON_ENABLE_PINS) &&
   	(u8HBridgeMotorSpeed >= HBRIDGE_MOTOR_SPEED_MIN_VALUE) 	&& (u8HBridgeMotorSpeed <= HBRIDGE_MOTOR_SPEED_MAX_VALUE))
   {
		gu8HBridgeMotorSpeed = u8HBridgeMotorSpeed;
	   enuRetVar = E_OK;
   }
   else
   {
	   enuRetVar = E_ERROR;
   }
   return enuRetVar;
}

/*************************************************************************************************
* Parameters	: u8GroupId, enuHBridgeMotorDirection
* Return Value	: enuErrorStatus_t
* Description  : Set the direction of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_SetDirection(uint8_t u8GroupId, enuHBridgeMotorDirection_t enuHBridgeMotorDirection)
{
   enuErrorStatus_t enuRetVar;
   
   if((aenuHBridgeState[u8GroupId] == HBRIDGE_INIT_DONE) &&
   	(enuHBridgeMotorDirection > HBRIDGE_MOTOR_DIRECTION_MIN_VALUE) && (enuHBridgeMotorDirection < HBRIDGE_MOTOR_DIRECTION_MAX_VALUE))
   {
	   /* Checking if the motor is moving forward or backward */
	   if(enuHBridgeMotorDirection == MOVE_FORWARD)
	   {
		   /* Setting the value of 1A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1ADirectionPinGroupId, HIGH);
		   /* Clearing the value of 1B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1BDirectionPinGroupId, LOW);
		   /* Setting the value of 2A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2ADirectionPinGroupId, HIGH);
		   /* Clearing the value of 2B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2BDirectionPinGroupId, LOW);
	   }
	   else if(enuHBridgeMotorDirection == MOVE_BACKWARD)
	   {
	   	/* Clearing the value of 1A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1ADirectionPinGroupId, LOW);
		   /* Setting the value of 1B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1BDirectionPinGroupId, HIGH);
		   /* Clearing the value of 2A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2ADirectionPinGroupId, LOW);
		   /* Setting the value of 2B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2BDirectionPinGroupId, HIGH);
	   }
	   else if(enuHBridgeMotorDirection == TURN_LEFT)
	   {
	   	/* Clearing the value of 1A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1ADirectionPinGroupId, LOW);
		   /* Setting the value of 1B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1BDirectionPinGroupId, HIGH);
		   /* Setting the value of 2A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2ADirectionPinGroupId, HIGH);
		   /* Clearing the value of 2B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2BDirectionPinGroupId, LOW);
	   }
	   else
	   {
	   	/* Setting the value of 1A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1ADirectionPinGroupId, HIGH);
		   /* Clearing the value of 1B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor1BDirectionPinGroupId, LOW);
		   /* Clearing the value of 2A direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2ADirectionPinGroupId, LOW);
		   /* Setting the value of 2B direction pin */
	   	DIO_Write(astrHBridgeConfigParameters[u8GroupId].u8Motor2BDirectionPinGroupId, HIGH);
	   }
	   enuRetVar = E_OK;
   }
   else
   {
	   enuRetVar = E_ERROR;
   }
   return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Start the rotation of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_Start(uint8_t u8GroupId)
{
   enuErrorStatus_t enuRetVar;
   
   if(aenuHBridgeState[u8GroupId] == HBRIDGE_INIT_DONE)
   {
      if(astrHBridgeConfigParameters[u8GroupId].enuHBridgeEnablePinsControl == DIO_ON_ENABLE_PINS)
      {
   		/* Starting motor 1 to move */
	   	DIO_Write(astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor1EnablePinGroupId].enuDIOPinNo, HIGH);
   		/* Starting motor 2 to move */
	   	DIO_Write(astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor2EnablePinGroupId].enuDIOPinNo, HIGH);
      }
      else
      {
   	   /* Starting motor 1 to move */
   	   PWM_Start(astrHBridgeConfigParameters[u8GroupId].u8Motor1EnablePinGroupId, gu8HBridgeMotorSpeed);
   	   /* Starting motor 2 to move */
   	   PWM_Start(astrHBridgeConfigParameters[u8GroupId].u8Motor2EnablePinGroupId, gu8HBridgeMotorSpeed);
      }
	   enuRetVar = E_OK;
   }
   else
   {
	   enuRetVar = E_ERROR;
   }
   return enuRetVar;	
}

/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Stop the rotation of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_Stop(uint8_t u8GroupId)
{
   enuErrorStatus_t enuRetVar;
   
   if(aenuHBridgeState[u8GroupId] == HBRIDGE_INIT_DONE)
   {
      if(astrHBridgeConfigParameters[u8GroupId].enuHBridgeEnablePinsControl == DIO_ON_ENABLE_PINS)
      {
   	   /* Stopping motor 1 */
	   	DIO_Write(astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor1EnablePinGroupId].enuDIOPinNo, LOW);
		   /* Stopping motor 2 */
	   	DIO_Write(astrDIOConfigParameters[astrHBridgeConfigParameters[u8GroupId].u8Motor2EnablePinGroupId].enuDIOPinNo, LOW);
      }
      else
      {
   	   /* Stopping motor 1 */
   	   PWM_Stop(astrHBridgeConfigParameters[u8GroupId].u8Motor1EnablePinGroupId);
   	   /* Stopping motor 2 */
   	   PWM_Stop(astrHBridgeConfigParameters[u8GroupId].u8Motor2EnablePinGroupId);
      }	   enuRetVar = E_OK;
   }
   else
   {
	   enuRetVar = E_ERROR;
   }
   return enuRetVar;	
}

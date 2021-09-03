#ifndef HBRIDGE_H_
#define HBRIDGE_H_

#include "HBridge_Cfg.h"
#include "DIO.h"
#include "PWM.h"
#include "STD_TYPES.h"

typedef enum {DIO_ON_ENABLE_PINS, PWM_ON_ENABLE_PINS} enuHBridgeEnablePinsControl_t;
typedef enum {MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT} enuHBridgeMotorDirection_t;

typedef struct
{
	/* Selecting whether the enable pins are driven by DIO or PWM */
	enuHBridgeEnablePinsControl_t enuHBridgeEnablePinsControl;
	/* Selecting the group id number that corresponds motor 1 enable pin */
	uint8_t u8Motor1EnablePinGroupId;
	/* Selecting the group id number that corresponds motor 2 enable pin */
	uint8_t u8Motor2EnablePinGroupId;
	/* Selecting the group id number that corresponds motor 1A direction pin */
	uint8_t u8Motor1ADirectionPinGroupId;
	/* Selecting the group id number that corresponds motor 1B direction pin */
	uint8_t u8Motor1BDirectionPinGroupId;
	/* Selecting the group id number that corresponds motor 2A direction pin */
	uint8_t u8Motor2ADirectionPinGroupId;
	/* Selecting the group id number that corresponds motor 2B direction pin */
	uint8_t u8Motor2BDirectionPinGroupId;
} strHBridgeConfigType_t;

extern strHBridgeConfigType_t astrHBridgeConfigParameters[HBRIDGE_GROUPS_NO];

/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Initialize the H Bridge module according to the configuration
*************************************************************************************************/
enuErrorStatus_t HBridge_Init(uint8_t u8GroupId);
/*************************************************************************************************
* Parameters	: u8GroupId, u8HBridgeMotorSpeed
* Return Value	: enuErrorStatus_t
* Description  : Set the speed of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_SetSpeed(uint8_t u8GroupId, uint8_t u8HBridgeMotorSpeed);
/*************************************************************************************************
* Parameters	: u8GroupId, enuHBridgeMotorDirection
* Return Value	: enuErrorStatus_t
* Description  : Set the direction of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_SetDirection(uint8_t u8GroupId, enuHBridgeMotorDirection_t enuHBridgeMotorDirection);
/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Start the rotation of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_Start(uint8_t u8GroupId);
/*************************************************************************************************
* Parameters	: u8GroupId
* Return Value	: enuErrorStatus_t
* Description  : Stop the rotation of motors
*************************************************************************************************/
enuErrorStatus_t HBridge_Stop(uint8_t u8GroupId);

#endif /* HBRIDGE_H_ */

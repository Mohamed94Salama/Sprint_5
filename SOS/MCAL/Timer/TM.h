#ifndef TM_H_
#define TM_H_

#include "STD_TYPES.h"
#include "BIT_MANIPULATE.h"
#include "TM_Cfg.h"
#include "DIO.h"
#include "DIO_Cfg.h"
#include "Register.h"


typedef enum {TIMER0, TIMER1, TIMER2} enuTimerModule_t;
typedef enum {NORMAL, CTC, FAST_PWM, PC_PWM} enuTimerMode_t;
typedef enum {NO_CLOCK, NO_PRESCALER, PRESCALER_8, PRESCALER_64, PRESCALER_256, PRESCALER_1024, 
			  EXTERNAL_CLOCK_FALL_EDGE, EXTERNAL_CLOCK_RISE_EDGE, PRESCALER_32, PRESCALER_128} enuTimerClock_t;
typedef enum {TIMER_INT_DIS, TIMER_INT_EN} enuTimerIntEn_t;
typedef enum {OC0_Disconnected, Toggle_OC0, Clear_OC0, Set_OC0}enuCTCModeTimer0_t;
typedef enum {OC1_Disconnected, Toggle_OC1, Clear_OC1, Set_OC1}enuCTCModeTimer1_t;
typedef enum {OC2_Disconnected, Toggle_OC2, Clear_OC2, Set_OC2}enuCTCModeTimer2_t;
	
typedef struct
{
	/* Selecting the used timer module */
	enuTimerModule_t enuTimerModule;
	/* Selecting the timer module mode */
	enuTimerMode_t enuTimerMode;
	/* Selecting the clock source to be used for the selected module							*
	 * N.B: 1) TIMER0 and TIMER1 don't support PRESCALER_32 and PRESCALER_128 modes				*
	 *		2) TIMER2 don't support EXTERNAL_CLOCK_FALL_EDGE and EXTERNAL_CLOCK_FALL_EDGE modes */
	enuTimerClock_t enuTimerClock;
	/* Selecting whether to enable or disable timer module interrupts */ 
	enuTimerIntEn_t enuTimerIntEn;		
	/* Selecting the CTC mode */
	enuCTCModeTimer0_t enuCTCMode0;
	enuCTCModeTimer1_t enuCTCMode1;
	enuCTCModeTimer2_t enuCTCMode2;
} strTimerConfigType_t;

extern const strTimerConfigType_t astrTimerConfigParameters[TM_GROUPS_NO];

/* A function to initialize all the TM groups according to the configuration */
enuErrorStatus_t Timer_Init(void);
/* A function to initiate a timer delay on the selected TM group */
enuErrorStatus_t Timer_Start(uint8_t u8GroupId, uint64_t u32TimerValueUS, void(*pfCallback)(void));
/* A function to stop a timer */
enuErrorStatus_t Timer_Stop(uint8_t u8GroupId);
/* A function to check periodically for the timer flag */
enuErrorStatus_t Timer_GetStatus(uint8_t u8GroupId);

#endif /* TM_H_ */
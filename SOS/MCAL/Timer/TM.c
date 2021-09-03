/*
 * TM.c
 *
 * Created: 7/9/2021 4:04:10 PM
 *  Author: DELLL
 */ 
#include "TM.h"


void (*F_PTR)(void)=NULL;
uint32_t count;
uint32_t prescaler;
uint8_t Flag_0=0;
uint8_t Flag_1=0;
uint8_t Flag_2=0;
f32_t ARR2;
uint32_t T_des;
enuErrorStatus_t Timer_Init(void)
{
	uint8_t i;
	enuErrorStatus_t Error=E_ERROR;
	for(i=0;i<TM_GROUPS_NO;i++)
	{
		if(((astrTimerConfigParameters[i].enuTimerModule==TIMER0)||(astrTimerConfigParameters[i].enuTimerModule==TIMER1)||(astrTimerConfigParameters[i].enuTimerModule==TIMER2))&&
		((astrTimerConfigParameters[i].enuTimerMode==NORMAL)||(astrTimerConfigParameters[i].enuTimerMode==CTC)||(astrTimerConfigParameters[i].enuTimerMode==FAST_PWM)||(astrTimerConfigParameters[i].enuTimerMode==PC_PWM))&&
		((astrTimerConfigParameters[i].enuTimerClock==NO_CLOCK)||(astrTimerConfigParameters[i].enuTimerClock==NO_PRESCALER)||(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_8)||(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_64)||(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_256)||
		(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_1024)||(astrTimerConfigParameters[i].enuTimerClock==32)||(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_128)||(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)||(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE))&&
		((astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)||(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_EN))&&
		((astrTimerConfigParameters[i].enuCTCMode0==OC0_Disconnected)||(astrTimerConfigParameters[i].enuCTCMode0==Toggle_OC0)||(astrTimerConfigParameters[i].enuCTCMode0==Clear_OC0)||(astrTimerConfigParameters[i].enuCTCMode0==Set_OC0))&&
		((astrTimerConfigParameters[i].enuCTCMode1==OC1_Disconnected)||(astrTimerConfigParameters[i].enuCTCMode1==Toggle_OC1)||(astrTimerConfigParameters[i].enuCTCMode1==Clear_OC1)||(astrTimerConfigParameters[i].enuCTCMode1==Set_OC1))&&
		((astrTimerConfigParameters[i].enuCTCMode2==OC2_Disconnected)||(astrTimerConfigParameters[i].enuCTCMode2==Toggle_OC2)||(astrTimerConfigParameters[i].enuCTCMode2==Clear_OC2)||(astrTimerConfigParameters[i].enuCTCMode2==Set_OC2)))
		{
			Error=E_OK;
		if(astrTimerConfigParameters[i].enuTimerModule==TIMER0)
		{
			if(astrTimerConfigParameters[i].enuTimerMode==NORMAL)
			{
				CLEAR_BIT(TCCR0_R,WGM00_B);
				CLEAR_BIT(TCCR0_R,WGM01_B);					
					if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)
					{
						CLEAR_BIT(TIMSK_R,TOIE0_B);
					}else
					{
						sei();
						SET_BIT(TIMSK_R,TOIE0_B);
					}
			}else if(astrTimerConfigParameters[i].enuTimerMode==CTC)
			{
				CLEAR_BIT(TCCR0_R,WGM00_B);
				SET_BIT(TCCR0_R,WGM01_B);	
				if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)
				{
					CLEAR_BIT(TIMSK_R,OCIE0_B);
				}else
				{
					sei();
					SET_BIT(TIMSK_R,OCIE0_B);
				}
				if(astrTimerConfigParameters[i].enuCTCMode0==OC0_Disconnected)
				{
					CLEAR_BIT(TCCR0_R,COM01_B);
					CLEAR_BIT(TCCR0_R,COM00_B);
				}else if(astrTimerConfigParameters[i].enuCTCMode0==Toggle_OC0)
				{
					CLEAR_BIT(TCCR0_R,COM01_B);
					SET_BIT(TCCR0_R,COM00_B);		
					//DIO_Init();			
				}else if(astrTimerConfigParameters[i].enuCTCMode0==Clear_OC0)
				{
					SET_BIT(TCCR0_R,COM01_B);
					CLEAR_BIT(TCCR0_R,COM00_B);
					//DIO_Init();					
				}else if(astrTimerConfigParameters[i].enuCTCMode0==Set_OC0)
				{
					SET_BIT(TCCR0_R,COM01_B);
					SET_BIT(TCCR0_R,COM00_B);
					//DIO_Init();					
				}
		}else if(astrTimerConfigParameters[i].enuTimerMode==FAST_PWM)
		{
			SET_BIT(TCCR0_R,WGM00_B);
			SET_BIT(TCCR0_R,WGM01_B);
		}else if(astrTimerConfigParameters[i].enuTimerMode==PC_PWM)
		{
			SET_BIT(TCCR0_R,WGM00_B);
			CLEAR_BIT(TCCR0_R,WGM01_B);
		}
		if(astrTimerConfigParameters[i].enuTimerClock==NO_CLOCK)
		{
			CLEAR_BIT(TCCR0_R,CS02_B);
			CLEAR_BIT(TCCR0_R,CS01_B);
			CLEAR_BIT(TCCR0_R,CS00_B);
		}else if(astrTimerConfigParameters[i].enuTimerClock==NO_PRESCALER)
		{
			CLEAR_BIT(TCCR0_R,CS02_B);
			CLEAR_BIT(TCCR0_R,CS01_B);
			SET_BIT(TCCR0_R,CS00_B);
			prescaler=1;
		}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_8)
		{
			CLEAR_BIT(TCCR0_R,CS02_B);
			SET_BIT(TCCR0_R,CS01_B);
			CLEAR_BIT(TCCR0_R,CS00_B);
			prescaler=8;
		}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_64)
		{
			CLEAR_BIT(TCCR0_R,CS02_B);
			SET_BIT(TCCR0_R,CS01_B);
			SET_BIT(TCCR0_R,CS00_B);
			prescaler=64;
		}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_256)
		{
			SET_BIT(TCCR0_R,CS02_B);
			CLEAR_BIT(TCCR0_R,CS01_B);
			CLEAR_BIT(TCCR0_R,CS00_B);
			prescaler=256;
		}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_1024)
		{
			SET_BIT(TCCR0_R,CS02_B);
			CLEAR_BIT(TCCR0_R,CS01_B);
			SET_BIT(TCCR0_R,CS00_B);
			prescaler=1024;
		}else if(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)
		{
			SET_BIT(TCCR0_R,CS02_B);
			SET_BIT(TCCR0_R,CS01_B);
			CLEAR_BIT(TCCR0_R,CS00_B);
		}else if(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE)
		{
			SET_BIT(TCCR0_R,CS02_B);
			SET_BIT(TCCR0_R,CS01_B);
			SET_BIT(TCCR0_R,CS00_B);
		}
	}else if(astrTimerConfigParameters[i].enuTimerModule==TIMER1)
	{
		if(astrTimerConfigParameters[i].enuTimerMode==NORMAL)
		{
			
			if(astrTimerConfigParameters[i].enuTimerClock==NO_CLOCK)
			{
				CLEAR_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
			}else if(astrTimerConfigParameters[i].enuTimerClock==NO_PRESCALER)
			{
				CLEAR_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
				prescaler=1;				
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_8)
			{
				CLEAR_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
				prescaler=8;
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_64)
			{
				CLEAR_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
				prescaler=64;	
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_256)
			{
				SET_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
				prescaler=256;
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_1024)
			{
				SET_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
				prescaler=1024;				
			}else if(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)
			{
				SET_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);				
			}else if(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE)
			{
				SET_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
			}
			
			if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)
			{
				CLEAR_BIT(TIMSK_R,TOIE1_B);
			}else
			{
				sei();
				SET_BIT(TIMSK_R,TOIE1_B);
			}

	}else if(astrTimerConfigParameters[i].enuTimerMode==CTC)
	{
		if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)
		{
			CLEAR_BIT(TIMSK_R,OCIE1A_B);
		}else if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_EN)
		{
			sei();
			SET_BIT(TIMSK_R,OCIE1A_B);
		}
		
			if(astrTimerConfigParameters[i].enuTimerClock==NO_CLOCK)
			{
				CLEAR_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
			}else if(astrTimerConfigParameters[i].enuTimerClock==NO_PRESCALER)
			{
				CLEAR_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
				prescaler=1;
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_8)
			{
				CLEAR_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
				prescaler=8;
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_64)
			{
				CLEAR_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
				prescaler=64;
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_256)
			{
				SET_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
				prescaler=256;
			}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_1024)
			{
				SET_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
				prescaler=1024;
			}else if(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)
			{
				SET_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
			}else if(astrTimerConfigParameters[i].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE)
			{
				SET_BIT(TCCR1B_R,2);
				SET_BIT(TCCR1B_R,1);
				SET_BIT(TCCR1B_R,0);
			}
			
			if(astrTimerConfigParameters[i].enuCTCMode1==OC1_Disconnected)
			{
				CLEAR_BIT(TCCR1A_R,7);
				CLEAR_BIT(TCCR1A_R,6);
				CLEAR_BIT(TCCR1A_R,5);
				CLEAR_BIT(TCCR1A_R,4);	
			}else if(astrTimerConfigParameters[i].enuCTCMode1==Toggle_OC1)
			{
				CLEAR_BIT(TCCR1A_R,7);
				SET_BIT(TCCR1A_R,6);
				CLEAR_BIT(TCCR1A_R,5);
				SET_BIT(TCCR1A_R,4);
				//DIO_Init();
			}else if(astrTimerConfigParameters[i].enuCTCMode1==Clear_OC1)
			{
				SET_BIT(TCCR1A_R,7);
				CLEAR_BIT(TCCR1A_R,6);
				SET_BIT(TCCR1A_R,5);
				CLEAR_BIT(TCCR1A_R,4);
				//DIO_Init();				
			}else if(astrTimerConfigParameters[i].enuCTCMode1==Set_OC1)
			{
				SET_BIT(TCCR1A_R,7);
				SET_BIT(TCCR1A_R,6);
				SET_BIT(TCCR1A_R,5);
				SET_BIT(TCCR1A_R,4);
				//DIO_Init();
			}
			SET_BIT(TCCR1B_R,3);
	}
	}else if(astrTimerConfigParameters[i].enuTimerModule==TIMER2)
	{
					if(astrTimerConfigParameters[i].enuTimerClock==NO_CLOCK)
					{
						CLEAR_BIT(TCCR2_R,2);
						CLEAR_BIT(TCCR2_R,1);
						CLEAR_BIT(TCCR2_R,0);
					}else if(astrTimerConfigParameters[i].enuTimerClock==NO_PRESCALER)
					{
						CLEAR_BIT(TCCR2_R,2);
						CLEAR_BIT(TCCR2_R,1);
						SET_BIT(TCCR2_R,0);
						prescaler=1;
					}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_8)
					{
						CLEAR_BIT(TCCR2_R,2);
						SET_BIT(TCCR2_R,1);
						CLEAR_BIT(TCCR2_R,0);
						prescaler=8;
					}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_32)
					{
						CLEAR_BIT(TCCR2_R,2);
						SET_BIT(TCCR2_R,1);
						SET_BIT(TCCR2_R,0);
						prescaler=32;
					}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_64)
					{
						SET_BIT(TCCR2_R,2);
						CLEAR_BIT(TCCR2_R,1);
						CLEAR_BIT(TCCR2_R,0);
						prescaler=64;
					}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_128)
					{
						SET_BIT(TCCR2_R,2);
						CLEAR_BIT(TCCR2_R,1);
						SET_BIT(TCCR2_R,0);
						prescaler=128;
					}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_256)
					{
						SET_BIT(TCCR2_R,2);
						SET_BIT(TCCR2_R,1);
						CLEAR_BIT(TCCR2_R,0);
						prescaler=256;
					}else if(astrTimerConfigParameters[i].enuTimerClock==PRESCALER_1024)
					{
						SET_BIT(TCCR2_R,2);
						SET_BIT(TCCR2_R,1);
						SET_BIT(TCCR2_R,0);
						prescaler=1024;
					}

			if(astrTimerConfigParameters[i].enuTimerMode==NORMAL)
			{
				
				if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)
				{
					CLEAR_BIT(TIMSK_R,6);
				}else
				{
					sei();
					SET_BIT(TIMSK_R,6);
				}
			}else if(astrTimerConfigParameters[i].enuTimerMode==CTC)
			{
				SET_BIT(TCCR2_R,3);
				
				if(astrTimerConfigParameters[i].enuTimerIntEn==TIMER_INT_DIS)
				{
					CLEAR_BIT(TIMSK_R,7);
				}else
				{
					sei();
					SET_BIT(TIMSK_R,7);
				}	
				
				if(astrTimerConfigParameters[i].enuCTCMode2==OC2_Disconnected)
				{
					CLEAR_BIT(TCCR2_R,5);
					CLEAR_BIT(TCCR2_R,4);
				}else if(astrTimerConfigParameters[i].enuCTCMode2==Toggle_OC2)
				{
					CLEAR_BIT(TCCR2_R,5);
					SET_BIT(TCCR2_R,4);
					//DIO_Init();
				}else if(astrTimerConfigParameters[i].enuCTCMode2==Clear_OC2)
				{
					SET_BIT(TCCR2_R,5);
					CLEAR_BIT(TCCR2_R,4);
					//DIO_Init();
				}else if(astrTimerConfigParameters[i].enuCTCMode2==Set_OC2)
				{
					SET_BIT(TCCR2_R,5);
					SET_BIT(TCCR2_R,4);
					//DIO_Init();
				}				
			}
				}
				}
				}
	return Error;
}

enuErrorStatus_t Timer_Start(uint8_t u8GroupId, uint64_t u32TimerValueUS, void(*pfCallback)(void))
{
	enuErrorStatus_t Error = E_ERROR;
			if(((astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0)||(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)||(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==FAST_PWM)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==PC_PWM))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerClock==NO_CLOCK)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==NO_PRESCALER)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_8)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_64)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_256)||
			(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_1024)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==32)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_128)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_DIS)||(astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_EN))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode0==OC0_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Toggle_OC0)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Clear_OC0)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Set_OC0))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode1==OC1_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Toggle_OC1)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Clear_OC1)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Set_OC1))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode2==OC2_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Toggle_OC2)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Clear_OC2)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Set_OC2)))
			{Error=E_OK;
	f32_t Counts_needed;
	f32_t T_overflow;
	T_des=u32TimerValueUS;
	f32_t ARR=(F_CPU*u32TimerValueUS)/(prescaler*1000000);
	ARR2=ARR;
	if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0)
	{
		if(astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)
		{
			if(ARR2>=256)
			{
				count=ARR2/256;
			}else
			{
				T_overflow=(prescaler/F_CPU)*256;
				Counts_needed=(u32TimerValueUS/(T_overflow*1000000));
				TCNT0_R=255-Counts_needed;
				Flag_0=1;
			}
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)
		{
			OCR0_R=(u32TimerValueUS/(F_CPU/(prescaler*1000000)));
			TCNT0_R=0;
		}
				
	}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)
	{
		if(astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)
		{
			if(ARR2>=65536)
			{
				count=ARR2/65536;
			}else
			{
				
				TCNT1_R=65535-ARR2;
				Flag_1=1;
			}			
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)
		{
			OCR1A_R=ARR2;
			TCNT1_R=0;
		}
	}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2)
	{
		if(astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)
		{
			if(ARR2>=256)
			{
				count=ARR2/256;
			}else
			{
				T_overflow=(prescaler/F_CPU)*256;
				Counts_needed=(u32TimerValueUS/(T_overflow*1000000));
				TCNT2_R=255-Counts_needed;
				Flag_2=1;
			}
			
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)
		{
			OCR2_R=(u32TimerValueUS/(F_CPU/(prescaler*1000000)));
			TCNT2_R=0;
		}
	
	}
	F_PTR=pfCallback;
		}
	return Error;
	}

	
enuErrorStatus_t Timer_GetStatus(uint8_t u8GroupId)
{
	enuErrorStatus_t Error = E_ERROR;
			if(((astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0)||(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)||(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==FAST_PWM)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==PC_PWM))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerClock==NO_CLOCK)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==NO_PRESCALER)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_8)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_64)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_256)||
			(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_1024)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==32)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_128)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_DIS)||(astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_EN))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode0==OC0_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Toggle_OC0)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Clear_OC0)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Set_OC0))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode1==OC1_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Toggle_OC1)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Clear_OC1)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Set_OC1))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode2==OC2_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Toggle_OC2)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Clear_OC2)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Set_OC2)))
			{Error = E_OK;	
	if(astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_DIS)
	{
	static uint32_t overflow=0;
	
	if(astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)
	{
				if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0){
				if(Flag_0==1)
				{
				while(!GET_BIT(TIFR_R,0));
				F_PTR();
				TCNT0_R=256-(256-(ARR2*256));
				SET_BIT(TIFR_R,0);
				}else
				{
				while(!GET_BIT(TIFR_R,0));
				TCNT0_R=0;
				SET_BIT(TIFR_R,0);
				overflow++;
				if(overflow==count)
				{
					F_PTR();
					overflow=0;
				}
			
		}
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)
		{
			if(Flag_1==1)
			{
				while(!GET_BIT(TIFR_R,2));
				F_PTR();
				TCNT1_R=65536-(65535-(ARR2*65535));
				SET_BIT(TIFR_R,2);
			}else
			{
				while(!GET_BIT(TIFR_R,2));
				TCNT1_R=0;
				SET_BIT(TIFR_R,2);
				overflow++;
				if(overflow==count)
				{
					F_PTR();
					overflow=0;
				}	
			}			
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2)
		{
			if(Flag_2==1)
			{
				while(!GET_BIT(TIFR_R,0));
				F_PTR();
				TCNT1_R=256-(256-(ARR2*256));
				SET_BIT(TIFR_R,0);	
			}else
			{
				while(!GET_BIT(TIFR_R,6));
				TCNT2_R=0;
				SET_BIT(TIFR_R,6);
				overflow++;
				if(overflow==count)
				{
					F_PTR();
					overflow=0;
				}
			}			
		}
	}else if(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)
	{
		if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0)
		{
			while(!GET_BIT(TIFR_R,1));
			F_PTR();
			TCNT0_R=0;
			SET_BIT(TIFR_R,1);
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)
		{
			while(!GET_BIT(TIFR_R,4));
			F_PTR();
			TCNT1_R=0;
			SET_BIT(TIFR_R,4);			
		}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2)
		{
			while(!GET_BIT(TIFR_R,7));
			F_PTR();
			TCNT2_R=0;
			SET_BIT(TIFR_R,7);			
		}
	}
		}else
		{
			
		}
		}
	return Error;
}

ISR(TIMER0_OVF_vect)
{
	if(F_PTR != NULL)
	{
		if(Flag_0==0)
		{
			static uint32_t overflow_0=0;
			if(overflow_0==count)
			{
				overflow_0=0;
				TCNT0_R=0;
				F_PTR();
			}else
			{
				overflow_0++;
			}
	}else
	{
		F_PTR();
			TCNT0_R=(256-(ARR2*256));
			//SET_BIT(TIFR_R,0);
	}
}
	}
	
ISR(TIMER0_OC_vect)
{
	if(F_PTR != NULL)
	{
			F_PTR();
			TCNT0_R=0;
	}
}

ISR(TIMER1_OVF_vect)
{
	if(F_PTR != NULL)
	{
		if(Flag_1==0)
		{
			static uint32_t overflow_1=0;
			if(overflow_1==count)
			{
				overflow_1=0;
				TCNT1_R=0;
				F_PTR();
			}else
			{
				overflow_1++;
			}
		}else
		{
			F_PTR();
			TCNT1_R=(65535-(ARR2));
		}
	}
}

ISR(TIMER1_OCA_vect)
{
	if(F_PTR != NULL)
	{
		F_PTR();
		TCNT1_R=0;
	}
}

ISR(TIMER2_OVF_vect)
{
	if(F_PTR != NULL)
	{
		if(Flag_2==0)
		{
			static uint32_t overflow_2=0;
			if(overflow_2==count)
			{
				overflow_2=0;
				TCNT2_R=0;
				F_PTR();
			}else
			{
				overflow_2++;
			}
		}else
		{
			F_PTR();
			TCNT2_R=(256-(ARR2*256));
			//SET_BIT(TIFR_R,0);
		}
	}	
}

ISR(TIMER2_COMP_vect)
{
	if(F_PTR != NULL)
	{
		F_PTR();
		TCNT2_R=0;
	}	
}

enuErrorStatus_t Timer_Stop(uint8_t u8GroupId)
{
	enuErrorStatus_t Error = E_ERROR;
			if(((astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0)||(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)||(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerMode==NORMAL)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==CTC)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==FAST_PWM)||(astrTimerConfigParameters[u8GroupId].enuTimerMode==PC_PWM))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerClock==NO_CLOCK)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==NO_PRESCALER)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_8)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_64)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_256)||
			(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_1024)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==32)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==PRESCALER_128)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==EXTERNAL_CLOCK_FALL_EDGE)||(astrTimerConfigParameters[u8GroupId].enuTimerClock==EXTERNAL_CLOCK_RISE_EDGE))&&
			((astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_DIS)||(astrTimerConfigParameters[u8GroupId].enuTimerIntEn==TIMER_INT_EN))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode0==OC0_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Toggle_OC0)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Clear_OC0)||(astrTimerConfigParameters[u8GroupId].enuCTCMode0==Set_OC0))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode1==OC1_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Toggle_OC1)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Clear_OC1)||(astrTimerConfigParameters[u8GroupId].enuCTCMode1==Set_OC1))&&
			((astrTimerConfigParameters[u8GroupId].enuCTCMode2==OC2_Disconnected)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Toggle_OC2)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Clear_OC2)||(astrTimerConfigParameters[u8GroupId].enuCTCMode2==Set_OC2)))
			{ Error = E_OK;
	if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER0)
	{
			CLEAR_BIT(TCCR0_R,CS02_B);
			CLEAR_BIT(TCCR0_R,CS01_B);
			CLEAR_BIT(TCCR0_R,CS00_B);
	}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER1)
	{
				CLEAR_BIT(TCCR1B_R,2);
				CLEAR_BIT(TCCR1B_R,1);
				CLEAR_BIT(TCCR1B_R,0);
	}else if(astrTimerConfigParameters[u8GroupId].enuTimerModule==TIMER2)
	{
				CLEAR_BIT(TCCR2_R,2);
				CLEAR_BIT(TCCR2_R,1);
				CLEAR_BIT(TCCR2_R,0);		
	}
	}
	return Error;
}
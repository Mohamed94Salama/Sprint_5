#include "PWM.h"


strPWMConfigType_t astrPWMConfigParameters[PWM_GROUPS_NO] =
{
	{ PWM1A, FAST_PWM_ICR1, NON_INVERTED_PWM, PWM_PRESCALER_8 },
	{ PWM1B, FAST_PWM_ICR1, NON_INVERTED_PWM, PWM_PRESCALER_8 }
};

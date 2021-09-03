#include "DIO.h"


strDIOConfigType_t astrDIOConfigParameters[DIO_GROUPS_NO] =
{
	/* H-bridge */
	{ PD2, OUTPUT, NO_CONNECTION },
	{ PD3, OUTPUT, NO_CONNECTION },
	{ PD6, OUTPUT, NO_CONNECTION },
	{ PD7, OUTPUT, NO_CONNECTION },
	/* Ultrasonic */
	{ PC0, OUTPUT, NO_CONNECTION}, /* TRIG */
	{ PB2, INPUT, NO_CONNECTION},  /* ECHO */
	/* LCD */
	{ PA1, OUTPUT, NO_CONNECTION }, /* RS */
	{ PA2, OUTPUT, NO_CONNECTION }, /* RW */
	{ PA3, OUTPUT, NO_CONNECTION }, /* E  */
	{ PA4, OUTPUT, NO_CONNECTION }, /* D4 */
	{ PA5, OUTPUT, NO_CONNECTION }, /* D5 */
	{ PA6, OUTPUT, NO_CONNECTION }, /* D6 */
	{ PA7, OUTPUT, NO_CONNECTION }  /* D7 */
};

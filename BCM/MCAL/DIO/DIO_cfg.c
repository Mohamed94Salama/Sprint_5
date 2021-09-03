/*****************************************************************************
* Task: Car Project
* File Name: DIO_cfg.c
* Description: configuration file for used dio pins
* Author: TEAM2
* Date: 9/7/2021
******************************************************************************/

/*- INCLUDES----------------------------------------------*/
#include "DIO_init.h"


/*- GLOBAL VARIABLES-------------------------------------*/
const strDIOConfigType_t astrDIOConfigParameters[DIO_GROUPS_NO] =
{
	{PA0, OUTPUT, NO_CONNECTION}
};

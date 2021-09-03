#include "SOS.h"



/*implementation of tasks*/
void Task0(void)
{
	DIO_Toggle(0);
}
void Task1(void)
{
	DIO_Toggle(1);
}
void Task2(void)
{
	DIO_Toggle(2);
}



int main()
{
	strSOSConfigTask_t strSOSConfigTask0 = {4, Task0};
	strSOSConfigTask_t strSOSConfigTask1 = {2, Task1};
	strSOSConfigTask_t strSOSConfigTask2 = {1, Task2};

	DIO_Init(&astrDIOConfigParameters[0]);
	DIO_Init(&astrDIOConfigParameters[1]);
	DIO_Init(&astrDIOConfigParameters[2]);
	SOS_CreateTask(0, &strSOSConfigTask0);
	SOS_CreateTask(1, &strSOSConfigTask1);
	SOS_CreateTask(2, &strSOSConfigTask2);
	SOS_Init(0, 10000);
	SOS_Run();
	while(1)
	{
	}
}





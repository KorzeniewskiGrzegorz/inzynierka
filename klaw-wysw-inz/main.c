//*****************************************************************************
//
// Codigo de partida Practica 1.
// Autores: Eva Gonzalez, Ignacio Herrero, Jose Manuel Cano
//
//*****************************************************************************

#include<stdbool.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "utils/cpu_usage.h"

#include "drivers/rgb.h"



///// LCD
//#include "IFTSPI2_2LCD.h"
#include "LCD_task.h"
#include "IFT_LCD_PenColor.h"
unsigned int BACK_COLOR, POINT_COLOR;
/////


#define LED1TASKPRIO 1
#define LED1TASKSTACKSIZE 128

//Globales

uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;
 SensorIB sens[10];
 Remote remote_ble[10];


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}

#endif

//*****************************************************************************
//
// Aqui incluimos los "ganchos" a los diferentes eventos del FreeRTOS
//
//*****************************************************************************

//Esto es lo que se ejecuta cuando el sistema detecta un desbordamiento de pila
//
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
	//
	// This function can not return, so loop forever.  Interrupts are disabled
	// on entry to this function, so no processor interrupts will interrupt
	// this loop.
	//
	while(1)
	{
	}
}

//Esto se ejecuta cada Tick del sistema. LLeva la estadistica de uso de la CPU (tiempo que la CPU ha estado funcionando)
void vApplicationTickHook( void )
{
	static unsigned char count = 0;

	if (++count == 10)
	{
		g_ui32CPUUsage = CPUUsageTick();
		count = 0;
	}
	//return;
}

//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationIdleHook (void)
{
	SysCtlSleep();
}


//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationMallocFailedHook (void)
{
	while(1);
}


void TEMPTask(void){

	while(1){
		xSemaphoreTake(  bt_tx_sem, portMAX_DELAY) ;
		UARTprintf("AT+DISI?");
		vTaskDelay(10*configTICK_RATE_HZ);
		xSemaphoreGive(  bt_tx_sem) ;
		vTaskDelay(20*configTICK_RATE_HZ);

	}
}


void TxTask(void){

	while(1){
		Switch x;



		xQueueReceive(tx_queue,&x,portMAX_DELAY);


		xSemaphoreTake(  bt_tx_sem, portMAX_DELAY) ;
		UARTprintf("AT");
		vTaskDelay(0.5*configTICK_RATE_HZ);
		UARTprintf("AT+COND43639DC4156");
		//UARTprintf(x.addr);



		vTaskDelay(0.5*configTICK_RATE_HZ);

		if(x.action){

			UARTprintf("LEDON");
		}else UARTprintf("LEDOFF");

		vTaskDelay(configTICK_RATE_HZ);
		UARTprintf("AT");
		vTaskDelay(2*configTICK_RATE_HZ);
		xSemaphoreGive(  bt_tx_sem) ;



	}
}



/*
void ParserTask(void)
{
	while(1){
		Response str;
		xQueueReceive(response_queue,str.s,portMAX_DELAY);

		UARTCharPut(UART0_BASE,'|');
		UARTCharPut(UART0_BASE,'|');
		UARTCharPut(UART0_BASE,'|');
		UARTCharPut(UART0_BASE,'|');
		UARTCharPut(UART0_BASE,'|');
		UARTCharPut(UART0_BASE,'|');
		UARTCharPut(UART0_BASE,'\n');

		unsigned char * pcBuffer=str.s;

		unsigned char * wynik, * wynik2, * wynik3;

					wynik=strstr(pcBuffer, "OK+DISIS");

					if(wynik!=NULL){
						UUID_parsing(wynik);
					}

					wynik2=strstr(pcBuffer, "OK+DISCS");
					if(wynik2!=NULL ){

						address_parsing(wynik2);
					}



					/*wynik3=strstr(pcBuffer, "OK+DISCE");
					if(strstr(wynik3, "OK+DISIS")==NULL && strstr(wynik3, "OK+DISC")!=NULL){ // w [rzypadku gdy po skanie beacon jest jeszcze cos w buferze
						address_parsing(wynik3);
					}




	}

}
*/

//*****************************************************************************
//
// Funcion main(), Inicializa los perifericos, crea las tareas, etc... y arranca el bucle del sistema
//
//*****************************************************************************
int main(void){

	//
	// Set the clocking to run at 40 MHz from the PLL.
	//
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
			SYSCTL_OSC_MAIN);	//Ponermos el reloj principal a 40 MHz (200 Mhz del Pll dividido por 5)


	// Get the system clock speed.
	g_ulSystemClock = SysCtlClockGet();


	//Habilita el clock gating de los perifericos durante el bajo consumo --> perifericos que se desee activos en modo Sleep
	//                                                                        deben habilitarse con SysCtlPeripheralSleepEnable
	ROM_SysCtlPeripheralClockGating(true);

	// Inicializa el subsistema de medida del uso de CPU (mide el tiempo que la CPU no esta dormida)
	// Para eso utiliza un timer, que aqui hemos puesto que sea el TIMER3 (ultimo parametro que se pasa a la funcion)
	// (y por tanto este no se deberia utilizar para otra cosa).
	CPUUsageInit(g_ulSystemClock, configTICK_RATE_HZ/10, 3);


	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
		ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
		ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		 UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
		                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
		                         UART_CONFIG_PAR_NONE));

		ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);	//La UART tiene que seguir funcionando aunque el micro este dormido
		ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);	//La UART tiene que seguir funcionando aunque el micro este dormido

		UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
			IntEnable(INT_UART0);

			//
			// Enable the UART operation.
			//
			UARTEnable(UART0_BASE);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
		ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
		ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		UARTStdioConfig(1,9600,SysCtlClockGet());

		ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART1);	//La UART tiene que seguir funcionando aunque el micro este dormido
			ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOB);

			//UARTLoopbackEnable(UART0_BASE);



	int i;
	for(i=0;i<10;i++){
		sens[i].id=0xFFFF;
		sens[i].pomiarC=0xFFFF;
		sens[i].pomiarU=0xFFFF;
		sens[i].typ=0xFFFF;

		strcpy(remote_ble[i].address,"BRAK");
		remote_ble[i].con_state=0xff;

	}




	ButtonFlags   =     xEventGroupCreate();
			if(ButtonFlags == NULL)
				while(1);

	Joystick_init();

	TivaLCDInit();
	Lcd_Init();

	semaphore_lcd=  xSemaphoreCreateMutex();
	if(semaphore_lcd==NULL)
	{
		while(1);
	}

	bt_tx_sem=  xSemaphoreCreateMutex();
	if(bt_tx_sem==NULL)
	{
		while(1);
	}

	if((xTaskCreate(LCDTask, (portCHAR *)"LCD", 1024,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
		while(1);
	}



	tx_queue=xQueueCreate(1,sizeof(Switch));
					if(NULL==tx_queue)
							while(1);


	response_queue=xQueueCreate(1,sizeof(Response));
			if(NULL==response_queue)
					while(1);


	sens_queue=xQueueCreate(5,sizeof(SensorIB));
		if(NULL==sens_queue)
				while(1);

		sens_n_queue=xQueueCreate(1,sizeof(uint8_t));
				if(NULL==sens_n_queue)
						while(1);

		address_n_queue=xQueueCreate(1,sizeof(uint8_t)); // do przesylania ilosci adresow
						if(NULL==address_n_queue)
								while(1);

		address_queue=xQueueCreate(10,sizeof(Address));// do przesylania znalezionych adresow
				if(NULL==address_queue)
						while(1);

	if((xTaskCreate(UARTTask, (portCHAR *)"UARTbt", 1024,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
		while(1);
	}

	/*if((xTaskCreate(ParserTask, (portCHAR *)"Parser", 1024,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
		{
			while(1);
		}*/

	if((xTaskCreate(TEMPTask, (portCHAR *)"temp", 128,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
		while(1);
	}

	if((xTaskCreate(TxTask, (portCHAR *)"Tx", 128,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
		{
			while(1);
		}

	//
	// Arranca el  scheduler.  Pasamos a ejecutar las tareas que se hayan activado.
	//
	vTaskStartScheduler();	//el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas

	//De la funcion vTaskStartScheduler no se sale nunca... a partir de aqui pasan a ejecutarse las tareas.
	while(1)
	{
		//Si llego aqui es que algo raro ha pasado
	}
}




/*
 * configADC.h
 *
 *  Created on: 22/4/2016
 *      Author: jcgar
 */

#ifndef CONFIGADC_H_
#define CONFIGADC_H_

#include<stdint.h>





#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "configADC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "semphr.h"
#include "utils/cpu_usage.h"
#include "event_groups.h"

#define UP_BUTTON    	0x1
#define DOWN_BUTTON   	0x2
#define LEFT_BUTTON   	0x8
#define RIGHT_BUTTON   	0x4
#define OK_BUTTON 		0x10

#define UARTbt_FLAG 0x0100
#define TEMPDONE_FLAG 0x0200
//#define TEMPSCAN_FLAG 0x0400
#define ADD_PARS_FLAG 0x0800

#define ALL_BUTTON 0x1f
#define ALL_SENSOR 0xff00

 EventGroupHandle_t ButtonFlags;
 QueueHandle_t  response_queue;
 QueueHandle_t  address_queue;
 QueueHandle_t  address_n_queue;
 QueueHandle_t  sens_queue;
 QueueHandle_t  sens_n_queue;

 xSemaphoreHandle semaphore_scan;

 typedef struct {
	uint16_t typ;
 	uint16_t id;
 	uint16_t pomiarC;
 	uint16_t pomiarU;


 }SensorIB;

 typedef struct {
	uint8_t con_state;
	unsigned char address[12];

 }Remote;

 typedef struct {
	unsigned char address[12];

 }Address;


void Joystick_init(void);


void Alarma_ISR(void);
void AlarmaDig_ISR(void);



#endif /* CONFIGADC_H_ */

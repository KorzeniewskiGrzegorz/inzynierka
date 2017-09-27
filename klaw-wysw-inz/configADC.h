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
#define TEMPSCAN_FLAG 0x0200
#define TEMPDONE_FLAG 0x0400

#define ALL_BUTTON 0x1f
#define ALL_SENSOR 0xff00

 EventGroupHandle_t ButtonFlags;
 QueueHandle_t  temp_queue;
 QueueHandle_t  sens_queue;
 typedef struct {
 	uint16_t id;
 	uint16_t pomiarC;
 	uint16_t pomiarU;


 }odczyt;



void Joystick_init(void);


void Alarma_ISR(void);
void AlarmaDig_ISR(void);



#endif /* CONFIGADC_H_ */

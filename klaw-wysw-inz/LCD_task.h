/*
 * LCD_task.h
 *
 *  Created on: 19 wrz 2017
 *      Author: Marcus
 */

#ifndef LCD_TASK_H_
#define LCD_TASK_H_


#include<stdint.h>

#include "semphr.h"
#include "utils/cpu_usage.h"
#include "event_groups.h"

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
#include "IFT_LCD_PenColor.h"
#include "utils/uartstdio.h"
#include "IFTSPI2_2LCD.h"


void LCDTask(void);

#endif /* LCD_TASK_H_ */

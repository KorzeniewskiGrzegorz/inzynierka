#include<stdint.h>
#include<stdbool.h>


#include "configADC.h"



 //SEMANA2: Este fichero implementa la configuracion del ADC y la ISR asociada. Las tareas pueden llamar a la funcion configADC_LeeADC (bloqueante) para leer datos del ADC
//La funcion configADC_DisparaADC(...) (no bloqueante) realiza el disparo software del ADC
//La funcion configADC_IniciaADC realiza la configuración del ADC: Los Pines E0 a E3 se ponen como entradas analógicas (AIN3 a AIN0 respectivamente). Ademas crea la cola de mensajes necesaria para que la funcion configADC_LeeADC sea bloqueante




void Joystick_init(void)
{

	/*if(ButtonFlags ==NULL)
	{
		ButtonFlags   =     xEventGroupCreate();
		if(ButtonFlags == NULL)
			while(1);

	}*/


	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);




	  GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
	  GPIOPadConfigSet(GPIO_PORTB_BASE ,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	  GPIOIntTypeSet(GPIO_PORTB_BASE, 0xf,GPIO_RISING_EDGE);
	  IntPrioritySet(INT_GPIOB,configMAX_SYSCALL_INTERRUPT_PRIORITY);
	  	GPIOIntEnable(GPIO_PORTB_BASE,0xf);
	  	IntEnable(INT_GPIOB);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_ADC0);

	//HABILITAMOS EL GPIOE
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
	// Enable pin PE3 for ADC AIN0|AIN1|AIN2|AIN3
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);

	ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_HALF, 1);
	//
	// Enable the first sample sequencer to capture the value of channel 1 when
	// the processor trigger occurs.
	// We are using three samples, so we select sequencer 1 which handles 4 ch.
	// We set this to lower priority than the main service timer function
	ADCIntDisable(ADC0_BASE, 0); // Disable ADC ints
	ADCIntDisable(ADC0_BASE, 1);
	ADCIntDisable(ADC0_BASE, 2);
	ADCIntDisable(ADC0_BASE, 3);


	ADCComparatorIntDisable(ADC0_BASE, 0);
	ADCComparatorIntDisable(ADC0_BASE, 1);
	ADCComparatorIntDisable(ADC0_BASE, 2);
	ADCComparatorIntDisable(ADC0_BASE, 3);

	// Make sure the sequencers are off.
	ADCSequenceDisable( ADC0_BASE, 0 );
	ADCSequenceDisable( ADC0_BASE, 1 );
	ADCSequenceDisable( ADC0_BASE, 2 );
	ADCSequenceDisable( ADC0_BASE, 3 );
	ADCSequenceDisable( ADC0_BASE, 4 );


	ADCSequenceStepConfigure(ADC0_BASE,0,0, ADC_CTL_CH0);
		ADCSequenceStepConfigure(ADC0_BASE,0,1,ADC_CTL_CH0 | ADC_CTL_CMP0);
		ADCSequenceStepConfigure(ADC0_BASE,0,2, ADC_CTL_CH1 |ADC_CTL_CMP1);
		ADCSequenceStepConfigure(ADC0_BASE,0,3,ADC_CTL_CH1 |ADC_CTL_CMP2 );
		ADCSequenceStepConfigure(ADC0_BASE,0,4, ADC_CTL_CMP3  | ADC_CTL_END);




	ADCComparatorRegionSet( ADC0_BASE, 0, 3000, 4000);
	ADCComparatorConfigure( ADC0_BASE, 0, ADC_COMP_INT_HIGH_HONCE);
	ADCComparatorRegionSet( ADC0_BASE, 1, 1000, 2000 );
	ADCComparatorConfigure( ADC0_BASE, 1, ADC_COMP_INT_LOW_HONCE );
	ADCComparatorRegionSet( ADC0_BASE, 2, 3000, 4000);
	ADCComparatorConfigure( ADC0_BASE, 2, ADC_COMP_INT_HIGH_HONCE);
	ADCComparatorRegionSet( ADC0_BASE, 3, 1000, 2000 );
	ADCComparatorConfigure( ADC0_BASE, 3, ADC_COMP_INT_LOW_HONCE );


	// Clear stale data
	ADCComparatorReset( ADC0_BASE, 0, 1, 1 );
	ADCComparatorReset( ADC0_BASE, 1, 1, 1 );
	ADCComparatorReset( ADC0_BASE, 2, 1, 1 );
	ADCComparatorReset( ADC0_BASE, 3, 1, 1 );

	// Set up interrupts

	ADCIntClear(ADC0_BASE, 0); // Clear any interrupts

	IntPrioritySet( INT_ADC0SS0,configMAX_SYSCALL_INTERRUPT_PRIORITY );

	ADCIntEnable(ADC0_BASE, 0); // Enable LV fault interrupt

	ADCComparatorIntEnable( ADC0_BASE, 0);

	IntEnable( INT_ADC0SS0_TM4C123 ); // Turn on in NVIC


	// Start up the sequence to loop forever.

	ADCSequenceEnable(ADC0_BASE, 0); // Start.. Auto-repeat forever
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_ALWAYS, 0);






}






void Alarma_ISR(void) // rutina de interrupcion de los comparadores
{
	portBASE_TYPE higherPriorityTaskWoken=pdFALSE;

	uint32_t s= ADCComparatorIntStatus(ADC0_BASE);


	if(  s& UP_BUTTON ){

		xEventGroupSetBitsFromISR(ButtonFlags, UP_BUTTON , &higherPriorityTaskWoken );

	}else if(  s& DOWN_BUTTON ){

		xEventGroupSetBitsFromISR(ButtonFlags, DOWN_BUTTON , &higherPriorityTaskWoken );

	}else if(  s& LEFT_BUTTON ){

		xEventGroupSetBitsFromISR(ButtonFlags, LEFT_BUTTON , &higherPriorityTaskWoken );

	}else if(  s& RIGHT_BUTTON ){

		xEventGroupSetBitsFromISR(ButtonFlags, RIGHT_BUTTON , &higherPriorityTaskWoken );

	}

	ADCComparatorIntClear(ADC0_BASE,0xffff);


	portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
void AlarmaDig_ISR(void)// rutina de interrupcion de las entradas digitales
{

	portBASE_TYPE higherPriorityTaskWoken=pdFALSE;
	uint8_t value = ~GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);

	xEventGroupSetBitsFromISR(ButtonFlags, OK_BUTTON , &higherPriorityTaskWoken );



	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);
	portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

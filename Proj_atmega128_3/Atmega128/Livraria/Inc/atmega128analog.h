/************************************************************************
	ATMEGA128ANALOG
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     07/01/2024
************************************************************************/
#ifndef _ATMEGA128ANALOG_H_
	#define _ATMEGA128ANALOG_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant & Macro ***/
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
	// if using differential channels this value has to be greater than one
	#define MAX_CHANNEL 32
	// ADC_NUMBER_SAMPLE^2 gives number of samples, note values can only range from 0 to 4.
	#define ADC_NUMBER_SAMPLE 2
	#define MUX_MASK 31
#else
	#error "Not Atmega 128"
#endif

/*** Parameter ***/
typedef struct{
	uint8_t VREFF;
	uint8_t DIVISION_FACTOR;
}ADC0_Parameter;

/*** Handler ***/
typedef struct{
	ADC0_Parameter par;
	
	// V-table
	int (*read)(int selection);
}ADC0_Handler;

void adc_enable( uint8_t Vreff, uint8_t Divfactor, uint8_t n_channel, ... );
ADC0_Handler* adc(void);

#endif
/*** EOF ***/


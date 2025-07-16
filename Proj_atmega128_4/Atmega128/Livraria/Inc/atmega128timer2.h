/************************************************************************
	ATMEGA128TIMER2
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
************************************************************************/
#ifndef _ATMEGA128TIMER2_H_
	#define _ATMEGA128TIMER2_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant and  Macro ***/
#ifndef ATMEGA_128_TIMER_COUNTER
	#define ATMEGA_128_TIMER_COUNTER
#endif

#if !defined(__AVR_ATmega64__) && !defined(__AVR_ATmega128__)
	#error "Not Atmega 128"
#endif

/*** Callback ***/
typedef struct {
	void (*comp_vect)(void);
	void (*ovf_vect)(void);
}TC2_Callback;

/*** Handler ***/
typedef struct{
	TC2_Callback callback;
	
	// V-table
	void (*compoutmode)(unsigned char compoutmode);
	void (*compare)(unsigned char compare);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
}TC2_Handler;

void tc2_enable(unsigned char wavegenmode, unsigned char interrupt);
TC2_Handler* tc2(void);

#endif
/*** EOF ***/


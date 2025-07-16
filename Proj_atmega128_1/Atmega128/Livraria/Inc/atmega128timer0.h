/************************************************************************
	ATMEGA128TIMER0
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
************************************************************************/
#ifndef _ATMEGA128TIMER0_H_
	#define _ATMEGA128TIMER0_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant and Macro ***/
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
} TC0_Callback;

/*** Handler ***/
typedef struct {
	TC0_Callback callback;

	// V-table
	void (*compoutmode)(unsigned char compoutmode);
	void (*compare)(unsigned char compare);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
} TC0_Handler;

void tc0_enable(unsigned char wavegenmode, unsigned char interrupt);
TC0_Handler* tc0(void);

#endif
/*** EOF ***/


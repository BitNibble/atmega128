/************************************************************************
	ATMEGA128TIMER3
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
************************************************************************/
#ifndef _ATMEGA128TIMER3_H_
	#define _ATMEGA128TIMER3_H_

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
	void (*capt_vect)(void);
	void (*compa_vect)(void);
	void (*compb_vect)(void);
	void (*compc_vect)(void);
	void (*ovf_vect)(void);
}TC3_Callback;

/*** Handler ***/
typedef struct{
	TC3_Callback callback;
	
	// V-table
	void (*compoutmodeA)(unsigned char compoutmode);
	void (*compoutmodeB)(unsigned char compoutmode);
	void (*compoutmodeC)(unsigned char compoutmode);
	void (*compareA)(uint16_t compareA);
	void (*compareB)(uint16_t compareB);
	void (*compareC)(uint16_t compareC);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
}TC3_Handler;

void tc3_enable(unsigned char wavegenmode, unsigned char interrupt);
TC3_Handler* tc3(void);

#endif
/*** EOF ***/


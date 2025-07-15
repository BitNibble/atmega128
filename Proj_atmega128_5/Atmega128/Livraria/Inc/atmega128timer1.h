/************************************************************************
    ATMEGA128TIMER1
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
************************************************************************/
#ifndef _ATMEGA128TIMER1_H_
	#define _ATMEGA128TIMER1_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant and Macro ***/
#ifndef ATMEGA_128_TIMER_COUNTER
	#define ATMEGA_128_TIMER_COUNTER
#endif

#if !defined(__AVR_ATmega64__) && !defined(__AVR_ATmega128__)
	#error "Not ATmega128"
#endif

/*** Callback ***/
typedef struct {
    void (*capt_vect)(void);
    void (*compa_vect)(void);
    void (*compb_vect)(void);
    void (*compc_vect)(void);
    void (*ovf_vect)(void);
} TC1_Callback;

/*** Handler ***/
typedef struct {
    TC1_Callback callback;

    /*** V-Table ***/
    void    (*compoutmodeA)(uint8_t mode);
    void    (*compoutmodeB)(uint8_t mode);
    void    (*compoutmodeC)(uint8_t mode);
    void    (*compareA)(uint16_t value);
    void    (*compareB)(uint16_t value);
    void    (*compareC)(uint16_t value);
    uint8_t (*start)(uint16_t prescaler);
    uint8_t (*stop)(void);
} TC1;

void  tc1_enable(uint8_t wavegenmode, uint8_t interrupt);
TC1* tc1(void);

#endif
/*** EOF ***/

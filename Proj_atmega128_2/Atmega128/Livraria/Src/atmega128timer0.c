/*************************************************************************
	ATMEGA128TIMER0
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
*************************************************************************/
/*** Library ***/
#include "atmega128timer0.h"

/*** Procedure and Function declaration ***/
void TIMER_COUNTER0_compoutmode(unsigned char compoutmode);
void TIMER_COUNTER0_compare(unsigned char compare);
uint8_t TIMER_COUNTER0_start(unsigned int prescaler);
uint8_t TIMER_COUNTER0_stop(void);

/*** Internal State ***/
static TC0_Handler atmega128_tc0 = {
	// Callback
	.callback = {
		.comp_vect = NULL,
		.ovf_vect = NULL
	},
	// V-table
	.compoutmode = TIMER_COUNTER0_compoutmode,
	.compare = TIMER_COUNTER0_compare,
	.start = TIMER_COUNTER0_start,
	.stop = TIMER_COUNTER0_stop
};
uint8_t timer0_state;

/*** Handler ***/
void tc0_enable(unsigned char wavegenmode, unsigned char interrupt)
// PARAMETER SETTING
// wavegenmode: Normal; PWM phase correct; Fast PWM; default - Normal;
// interrupt: off; overflow; output compare; both; default - off.
{
	timer0_state = 0;

	// Clear WGM01:WGM00
	tc0_reg()->tccr0.var &= ~((1 << WGM00) | (1 << WGM01));

	switch(wavegenmode) {
		case 0: // Normal
		break;
		case 1: // PWM Phase Correct
		tc0_reg()->tccr0.var |= (1 << WGM00);
		break;
		case 2: // CTC
		tc0_reg()->tccr0.var |= (1 << WGM01);
		break;
		case 3: // Fast PWM
		tc0_reg()->tccr0.var |= (1 << WGM00) | (1 << WGM01);
		break;
		default:
		break;
	}

	// Disable interrupts first
	tc0_reg()->timsk.var &= ~((1 << TOIE0) | (1 << OCIE0));

	switch(interrupt) {
		case 0: // None
		break;
		case 1: // Overflow only
		tc0_reg()->timsk.var |= (1 << TOIE0);
		cpu_reg()->sreg.par.i = 1; // Enable global interrupts
		break;
		case 2: // Compare Match only
		tc0_reg()->timsk.var |= (1 << OCIE0);
		cpu_reg()->sreg.par.i = 1;
		break;
		case 3: // Both
		tc0_reg()->timsk.var |= (1 << TOIE0) | (1 << OCIE0);
		cpu_reg()->sreg.par.i = 1;
		break;
		default:
		break;
	}

	// Set default OCR0 to 0xFF
	tc0_reg()->ocr0.var = 0xFF;
}

TC0_Handler* tc0(void) {
	return &atmega128_tc0;
}

/*** Procedure and Function definition ***/
uint8_t TIMER_COUNTER0_start(unsigned int prescaler)
// Start Timer0 with valid prescaler setting
{
	if (!timer0_state) {
		// Stop timer first
		tc0_reg()->tccr0.var &= ~(7 << CS00);

		switch(prescaler) {
			case 1:   // No prescale
			tc0_reg()->tccr0.var |= (1 << CS00);
			break;
			case 8:
			tc0_reg()->tccr0.var |= (1 << CS01);
			break;
			case 64:
			tc0_reg()->tccr0.var |= (1 << CS01) | (1 << CS00);
			break;
			case 256:
			tc0_reg()->tccr0.var |= (1 << CS02);
			break;
			case 1024:
			tc0_reg()->tccr0.var |= (1 << CS02) | (1 << CS00);
			break;
			default:  // Default = 1024
			tc0_reg()->tccr0.var |= (1 << CS02) | (1 << CS00);
			break;
		}
		timer0_state = 85;
	}
	return timer0_state;
}

uint8_t TIMER_COUNTER0_stop(void)
// Stop Timer0 by clearing prescaler
{
	tc0_reg()->tccr0.var &= ~(7 << CS00);
	timer0_state = 0;
	return timer0_state;
}

void TIMER_COUNTER0_compoutmode(unsigned char compoutmode)
// Set OC0 (PB4) output compare mode
{
	// Clear previous COM00/COM01 bits
	tc0_reg()->tccr0.var &= ~((1 << COM00) | (1 << COM01));

	switch(compoutmode) {
		case 0: // Normal I/O
		break;
		case 1: // Toggle OC0 on Compare Match (non-PWM only)
		gpiob_reg()->ddr.var |= (1 << 4); // Set PB4 as output
		tc0_reg()->tccr0.var |= (1 << COM00);
		break;
		case 2: // Clear OC0 on Compare Match
		gpiob_reg()->ddr.var |= (1 << 4);
		tc0_reg()->tccr0.var |= (1 << COM01);
		break;
		case 3: // Set OC0 on Compare Match
		gpiob_reg()->ddr.var |= (1 << 4);
		tc0_reg()->tccr0.var |= (1 << COM01) | (1 << COM00);
		break;
		default:
		break;
	}
}

void TIMER_COUNTER0_compare(unsigned char compare)
// Set Output Compare Register OCR0
{
	tc0_reg()->ocr0.var = compare;
}

/*** Interrupt ***/
ISR(TIMER0_COMP_vect)
{
	if (atmega128_tc0.callback.comp_vect) {
		atmega128_tc0.callback.comp_vect();
	}
}
ISR(TIMER0_OVF_vect)
{
	if (atmega128_tc0.callback.ovf_vect) {
		atmega128_tc0.callback.ovf_vect();
	}
}
	
/*** EOF ***/


/*************************************************************************
	ATMEGA128TIMER2
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
*************************************************************************/
/*** Library ***/
#include "atmega128timer2.h"

/*** Procedure and Function declaration ***/
void TIMER_COUNTER2_compoutmode(unsigned char compoutmode);
void TIMER_COUNTER2_compare(unsigned char compare);
uint8_t TIMER_COUNTER2_start(unsigned int prescaler);
uint8_t TIMER_COUNTER2_stop(void);

/*** Internal State ***/
static TC2 atmega128_tc2 = {
	// Callback
	.callback = {
		.comp_vect = NULL,
		.ovf_vect = NULL
	},
	// V-table
	.compoutmode = TIMER_COUNTER2_compoutmode,
	.compare = TIMER_COUNTER2_compare,
	.start = TIMER_COUNTER2_start,
	.stop = TIMER_COUNTER2_stop
};
uint8_t timer2_state;

/*** Handler ***/
void tc2_enable(unsigned char wavegenmode, unsigned char interrupt)
// PARAMETER SETTING
// wavegen mode: Normal; PWM phase correct; Fast PWM; default-Normasl;
// interrupt: off; overflow; output compare; both; default - non.
{
	timer2_state = 0;
	
	tc2_reg()->tccr2.var &= ~((1 << WGM20) | (1 << WGM21));
	switch(wavegenmode){ // TOP -- Update of OCR2 at -- TOV0 Flag Set on
		case 0: // Normal, 0xFF -- Immediate -- MAX
		break;
		case 1: // PWM Phase Correct, 0xFF -- TOP -- BOTTOM
			tc2_reg()->tccr2.var |= (1 << WGM20);
		break;
		case 2: // CTC, OCR2 -- Immediate -- MAX
			tc2_reg()->tccr2.var |= (1 << WGM21);
		break;
		case 3: // Fast PWM, 0xFF -- BOTTOM -- MAX
			tc2_reg()->tccr2.var |= (1 << WGM20) | (1 << WGM21);
		break;
		default:
		break;
	}
	tc2_reg()->timsk.var &= ~((1 << TOIE2) | (1 << OCIE2));
	switch(interrupt){
		case 0:
		break;
		case 1:
			tc2_reg()->timsk.var |= (1 << TOIE2);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 2:
			tc2_reg()->timsk.var |= (1 << OCIE2);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 3:
			tc2_reg()->timsk.var |= (1 << TOIE2);
			tc2_reg()->timsk.var |= (1 << OCIE2);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		default:
		break;
	}
	tc2_reg()->ocr2.var = ~0;
}

TC2* tc2(void){ return &atmega128_tc2; }

/*** Procedure and Function definition ***/
uint8_t TIMER_COUNTER2_start(unsigned int prescaler)
// PARAMETER SETTING
// Frequency oscillator devision factor or prescaler.
// prescaler: clk T0S /(No prescaling); clk T0S /8 (From prescaler); clk T0S /64 (From prescaler);
// clk T0S /256 (From prescaler); clk T0S /1024 (From prescaler); External clock source on Tn pin. Clock on falling edge;
// External clock source on Tn pin. Clock on rising edge; default - clk T 0 S /1024 (From prescaler).
{
	if(!timer2_state){ // one shot
		tc2_reg()->tccr2.var &= ~(7 << CS20); // No clock source. (Timer/Counter stopped)
		switch(prescaler){
			//case 0: // No clock source. (Timer/Counter stopped)
			//break;
			case 1: // clkI/O/(No prescaler)
				tc2_reg()->tccr2.var |= (1 << CS20);
			break;
			case 8: // clkI/O/8 (From prescaler)
				tc2_reg()->tccr2.var |= (1 << CS21);
			break;
			case 64: // clkI/O/64 (From prescaler)
				tc2_reg()->tccr2.var |= (3 << CS20);
			break;
			case 256: // clkI/O/256 (From prescaler)
				tc2_reg()->tccr2.var |= (1 << CS22);
			break;
			case 1024: // clkI/O/1024 (From prescaler)
				tc2_reg()->tccr2.var |= (5 << CS20);
			break;
			case 6: // External clock source on T2 pin. Clock on falling edge [PD7]
				tc2_reg()->tccr2.var |= (6 << CS20);
			break;
			case 7: // External clock source on T2 pin. Clock on rising edge [PD7]
				tc2_reg()->tccr2.var |= (7 << CS20);
			break;
			default:
				tc2_reg()->tccr2.var |= (5 << CS20);
			break;
		}
		timer2_state = 85;
	}
	return timer2_state;
}
void TIMER_COUNTER2_compoutmode(unsigned char compoutmode)
// compoutmode: Normal gpio operation, OC0 disconnected; Toggle OC0 on compare match; 
// Clear OC0 on compare match when up-counting. Set OC0 on compare match when downcounting. Clear OC0 on compare match;
// Set OC0 on compare match when up-counting. Clear OC0 on compare match when downcounting. Set OC0 on compare match ;
// default-Normal gpio operation, OC0 disconnected.
{
	tc2_reg()->tccr2.var &= ~((1 << COM20) | (1 << COM21));
	switch(compoutmode){ // OC2  -->  PB7
		case 0: // Normal gpio operation, OC2 disconnected.
		break;
		case 1: // Reserved
			// Toggle OC2 on compare match
			gpiob_reg()->ddr.var |= 0x80;
			tc2_reg()->tccr2.var |= (1 << COM20);
		break;
		case 2: // Clear OC2 on compare match when up-counting. Set OC0 on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x80;
			tc2_reg()->tccr2.var |= (1 << COM21);
		break;
		case 3: // Set OC2 on compare match when up-counting. Clear OC0 on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x80;
			tc2_reg()->tccr2.var |= (1 << COM20) | (1 << COM21);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER2_compare(unsigned char compare)
{
	tc2_reg()->ocr2.var = compare;
}
uint8_t TIMER_COUNTER2_stop(void)
// stops timer by setting prescaler to zero
{
	tc2_reg()->tccr2.var &= ~(7 << CS20); // No clock source. (Timer/Counter stopped)
	timer2_state = 0;
	return timer2_state;
}

/*** Interrupt ***/
ISR(TIMER2_COMP_vect)
{
	if(atmega128_tc2.callback.comp_vect){ atmega128_tc2.callback.comp_vect(); }
}
ISR(TIMER2_OVF_vect)
{
	if(atmega128_tc2.callback.ovf_vect){ atmega128_tc2.callback.ovf_vect(); }
}

/*** EOF ***/


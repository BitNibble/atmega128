/*************************************************************************
	ATMEGA128TIMER1
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     14/07/2025
*************************************************************************/
/*** Library ***/
#include "atmega128timer1.h"

/*** Procedure and Function declaration ***/
void TIMER_COUNTER1_compoutmodeA(unsigned char compoutmode);
void TIMER_COUNTER1_compoutmodeB(unsigned char compoutmode);
void TIMER_COUNTER1_compoutmodeC(unsigned char compoutmode);
void TIMER_COUNTER1_compareA(uint16_t compare);
void TIMER_COUNTER1_compareB(uint16_t compare);
void TIMER_COUNTER1_compareC(uint16_t compare);
uint8_t TIMER_COUNTER1_start(unsigned int prescaler);
uint8_t TIMER_COUNTER1_stop(void);

/*** Internal State ***/
static TC1 atmega128_tc1 = {
	// Callback
	.callback = {
		.capt_vect = NULL,
		.compa_vect = NULL,
		.compb_vect = NULL,
		.compc_vect = NULL,
		.ovf_vect = NULL
	},
	// V-table
	.compoutmodeA = TIMER_COUNTER1_compoutmodeA,
	.compoutmodeB = TIMER_COUNTER1_compoutmodeB,
	.compoutmodeC = TIMER_COUNTER1_compoutmodeC,
	.compareA = TIMER_COUNTER1_compareA,
	.compareB = TIMER_COUNTER1_compareB,
	.compareC = TIMER_COUNTER1_compareC,
	.start = TIMER_COUNTER1_start,
	.stop = TIMER_COUNTER1_stop
};
uint8_t timer1_state;

/*** Handler ***/
void tc1_enable(unsigned char wavegenmode, unsigned char interrupt)
// PARAMETER SETTING
// wavegen mode: Normal; PWM, Phase Correct, 8-bit; PWM, Phase Correct, 9-bit; PWM, Phase Correct, 10-bit;
// CTC; Fast PWM, 8-bit; Fast PWM, 9-bit; Fast PWM, 10-bit; PWM, Phase and Frequency Correct; PWM, Phase and Frequency Correct;
// PWM, Phase Correct; PWM, Phase Correct; CTC; (Reserved); Fast PWM; Fast PWM.
// interrupt: off; overflow; output compare; both; default - non.
// for more information read data sheet.
{
	timer1_state = 0;
	
	tc1_reg()->tccr1a.var &= ~((1 << WGM11) | (1 << WGM10));
	tc1_reg()->tccr1b.var &= ~((1 << WGM13) | (1 << WGM12));
	switch(wavegenmode){ // TOP -- Update of OCRnX at -- TOV Flag Set on
		case 0: // Normal, 0xFFFF -- Immediate -- MAX
		break;
		case 1: // PWM Phase Correct 8-bit, 0x00FF -- TOP -- BOTTOM
			tc1_reg()->tccr1a.var |= (1 << WGM10);
		break;
		case 2:	// PWM Phase Correct 9-bit, 0x01FF -- TOP -- BOTTOM
			tc1_reg()->tccr1a.var |= (1 << WGM11);
		break;
		case 3:	// PWM Phase Correct 10-bit, 0x03FF -- TOP -- BOTTOM
			tc1_reg()->tccr1a.var |= (1 << WGM11) | (1 << WGM10);
		break;
		case 4:	// CTC, OCRnA Immediate MAX
			tc1_reg()->tccr1b.var |= (1 << WGM12);
		break;
		case 5:	// Fast PWM 8-bit, 0x00FF -- BOTTOM -- TOP
			tc1_reg()->tccr1a.var |= (1 << WGM10);
			tc1_reg()->tccr1b.var |= (1 << WGM12);
		break;
		case 6:	// Fast PWM 9-bit, 0x01FF -- BOTTOM -- TOP
			tc1_reg()->tccr1a.var |= (1 << WGM11);
			tc1_reg()->tccr1b.var |= (1 << WGM12);
		break;
		case 7:	// Fast PWM 10-bit, 0x03FF -- BOTTOM -- TOP
			tc1_reg()->tccr1a.var |= (1 << WGM11) | (1 << WGM10);
			tc1_reg()->tccr1b.var |=(1 << WGM12);
		break;
		case 8:	// PWM Phase and Frequency Correct, ICRnA -- BOTTOM -- BOTTOM
			tc1_reg()->tccr1b.var |= (1 << WGM13);
		break;
		case 9:	// PWM Phase and Frequency Correct, OCRnA -- BOTTOM -- BOTTOM
			tc1_reg()->tccr1a.var |= (1 << WGM10);
			tc1_reg()->tccr1b.var |= (1 << WGM13);
		break;
		case 10: // PWM Phase Correct, ICRn -- TOP -- BOTTOM
			tc1_reg()->tccr1a.var |= (1 << WGM11);
			tc1_reg()->tccr1b.var |= (1 << WGM13);
		break;
		case 11: // PWM Phase Correct, OCRnA -- TOP -- BOTTOM
			tc1_reg()->tccr1a.var |= (1 << WGM11) | (1 << WGM10);
			tc1_reg()->tccr1b.var |= (1 << WGM13);
		break;
		case 12: // CTC, ICRn -- Immediate -- MAX
			tc1_reg()->tccr1b.var |= (1 << WGM13) | (1 << WGM12);
		break;
		case 13: // (Reserved), -- -- --
			tc1_reg()->tccr1a.var |= (1 << WGM10);
			tc1_reg()->tccr1b.var |= (1 << WGM13) | (1 << WGM12);
		break;
		case 14: // Fast PWM, ICRn -- BOTTOM -- TOP
			tc1_reg()->tccr1a.var |= (1 << WGM11);
			tc1_reg()->tccr1b.var |= (1 << WGM13) | (1 << WGM12);
		break;
		case 15: // Fast PWM, OCRnA -- BOTTOM -- TOP
			tc1_reg()->tccr1a.var |= (1 << WGM11) | (1 << WGM10);
			tc1_reg()->tccr1b.var |= (1 << WGM13) | (1 << WGM12);
		break;
		default:
		break;
	}
	tc1_reg()->tccr1a.var &= ~((3 << COM1A0) | (3 << COM1B0) | (3 << COM1C0));
	tc1_reg()->timsk.var &= ~((1 << TICIE1) | (1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1));
	tc1_reg()->etimsk.var &= ~(1 << OCIE1C);
	switch(interrupt){ // ICP1  -->  PD4
		case 0:
		break;
		case 1:
			tc1_reg()->timsk.var |= (1 << TOIE1);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 2:
			tc1_reg()->timsk.var |= (1 << OCIE1A);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 3:
			tc1_reg()->timsk.var |= (1 << OCIE1B);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 4:
			tc1_reg()->etimsk.var |= (1 << OCIE1C);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 5:
			tc1_reg()->timsk.var |= (1 << TICIE1);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 6:
			tc1_reg()->timsk.var |= (1 << OCIE1A) | (1 << TOIE1);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 7:
			tc1_reg()->timsk.var |= (1 << OCIE1B) | (1 << TOIE1);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 8:
			tc1_reg()->timsk.var |= (1 << TOIE1);
			tc1_reg()->etimsk.var |= (1 << OCIE1C);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 9:
			tc1_reg()->timsk.var |= (1 << TICIE1) | (1 << TOIE1);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 10:
			tc1_reg()->timsk.var |= (1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 11:
			tc1_reg()->timsk.var |= (1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1);
			tc1_reg()->etimsk.var |= (1 << OCIE1C);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		case 12:
			tc1_reg()->timsk.var |= (1 << OCIE1A) | (1 << OCIE1B);
			tc1_reg()->etimsk.var |= (1 << OCIE1C);
			cpu_reg()->sreg.var |= 1 << 7;
		break;
		default:
		break;
	}
	tc1_reg()->ocr1a = writeHLbyte(~0);
	tc1_reg()->ocr1b = writeHLbyte(~0);
	tc1_reg()->ocr1c = writeHLbyte(~0);
}

TC1* tc1(void){ return &atmega128_tc1; }

/*** Procedure and Function definition ***/
uint8_t TIMER_COUNTER1_start(unsigned int prescaler)
// PARAMETER SETTING
// Frequency oscillator devision factor or prescaler.
// prescaler: clk T0S /(No prescaling); clk T0S /8 (From prescaler); clk T0S /64 (From prescaler);
// clk T0S /256 (From prescaler); clk T0S /1024 (From prescaler); External clock source on Tn pin. Clock on falling edge;
// External clock source on Tn pin. Clock on rising edge; default - clk T 0 S /1024 (From prescaler).
{
	if(!timer1_state){ // one shot
		tc1_reg()->tccr1b.var &= ~(7 << CS10); // No clock source. (Timer/Counter stopped)
		switch(prescaler){
			//case 0: // No clock source. (Timer/Counter stopped)
			//break;
			case 1: // clkI/O/1 (No prescaler
				tc1_reg()->tccr1b.var |= (1 << CS10);
			break;
			case 8: // clkI/O/8 (From prescaler)
				tc1_reg()->tccr1b.var |= (1 << CS11);
			break;
			case 64: // clkI/O/64 (From prescaler)
				tc1_reg()->tccr1b.var |= (3 << CS10);
			break;
			case 256: // clkI/O/256 (From prescaler)
				tc1_reg()->tccr1b.var |= (1 << CS12);
			break;
			case 1024: // clkI/O/1024 (From prescaler)
				tc1_reg()->tccr1b.var |= (5 << CS10);
			break;
			case 6: // External clock source on Tn pin. Clock on falling edge [PD6]
				tc1_reg()->tccr1b.var |= (6 << CS10);
			break;
			case 7: // External clock source on Tn pin. Clock on rising edge [PD6]
				tc1_reg()->tccr1b.var |= (7 << CS10);
			break;
			default: // clkI/O/1024 (From prescaler)
				tc1_reg()->tccr1b.var |= (5 << CS10);
			break;
		}
		timer1_state = 85;
	}
	return timer1_state;
}
void TIMER_COUNTER1_compoutmodeA(unsigned char compoutmode)
{
	tc1_reg()->tccr1a.var &= ~(3 << COM1A0);
	switch(compoutmode){ // OC1A  -->  PB5
		case 0: // Normal gpio operation, OC1A disconnected.
		break;
		case 1: // Reserved
			// Toggle OC1A on compare match
			gpiob_reg()->ddr.var |= 0x20;
			tc1_reg()->tccr1a.var |= (1 << COM1A0);
		break;
		case 2: // Clear OC1A on compare match when up-counting. Set OC1A on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x20;
			tc1_reg()->tccr1a.var |= (1 << COM1A1);
		break;
		case 3: // Set OC1A on compare match when up-counting. Clear OC1A on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x20;
			tc1_reg()->tccr1a.var |= (1 << COM1A0) | (1 << COM1A1);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER1_compoutmodeB(unsigned char compoutmode)
{
	tc1_reg()->tccr1a.var &= ~(3 << COM1B0);
	switch(compoutmode){ // OC1B  -->  PB6
		case 0: // Normal gpio operation, OC1B disconnected.
		break;
		case 1: // Reserved
			// Toggle OC1A or OC1B on compare match
			gpiob_reg()->ddr.var |= 0x40;
			tc1_reg()->tccr1a.var |= (1 << COM1B0);
		break;
		case 2: // Clear OC1B on compare match when up-counting. Set OC1B on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x40;
			tc1_reg()->tccr1a.var |= (1 << COM1B1);
		break;
		case 3: // Set OC1B on compare match when up-counting. Clear OC1B on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x40;
			tc1_reg()->tccr1a.var |= (1 << COM1B0) | (1 << COM1B1);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER1_compoutmodeC(unsigned char compoutmode)
{
	tc1_reg()->tccr1a.var &= ~(3 << COM1C0);
	switch(compoutmode){ // OC1C  -->  PB7
		case 0: // Normal gpio operation, OC1C disconnected.
		break;
		case 1: // Reserved
			// Toggle OC1A or OC1C on compare match
			gpiob_reg()->ddr.var |= 0x80;
			tc1_reg()->tccr1a.var |= (1 << COM1C0);
		break;
		case 2: // Clear OC1C on compare match when up-counting. Set OC1C on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x80;
			tc1_reg()->tccr1a.var |= (1 << COM1C1);
		break;
		case 3: // Set OC1C on compare match when up-counting. Clear OC1C on compare
			// match when down counting.
			gpiob_reg()->ddr.var |= 0x80;
			tc1_reg()->tccr1a.var |= (1 << COM1C0) | (1 << COM1C1);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER1_compareA(uint16_t compare)
{
	tc1_reg()->ocr1a = writeHLbyte(compare);
}
void TIMER_COUNTER1_compareB(uint16_t compare)
{
	tc1_reg()->ocr1b = writeHLbyte(compare);
}
void TIMER_COUNTER1_compareC(uint16_t compare)
{
	tc1_reg()->ocr1c = writeHLbyte(compare);
}
uint8_t TIMER_COUNTER1_stop(void)
// stops timer by setting prescaler to zero
{
	tc1_reg()->tccr1b.var &= ~(7 << CS10); // No clock source. (Timer/Counter stopped)
	timer1_state = 0;
	return timer1_state;
}

/*** Interrupt ***/
ISR(TIMER1_CAPT_vect)
{
	if(atmega128_tc1.callback.capt_vect){ atmega128_tc1.callback.capt_vect(); }
}
ISR(TIMER1_COMPA_vect)
{
	if(atmega128_tc1.callback.compa_vect){ atmega128_tc1.callback.compa_vect(); }
}
ISR(TIMER1_COMPB_vect)
{
	if(atmega128_tc1.callback.compb_vect){ atmega128_tc1.callback.compb_vect(); }
}
ISR(TIMER1_COMPC_vect)
{
	if(atmega128_tc1.callback.compc_vect){ atmega128_tc1.callback.compc_vect(); }
}
ISR(TIMER1_OVF_vect)
{
	if(atmega128_tc1.callback.ovf_vect){ atmega128_tc1.callback.ovf_vect(); }
}
	
/*** EOF ***/


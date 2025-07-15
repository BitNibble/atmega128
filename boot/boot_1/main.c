#ifndef __AVR_ATmega128__
	#define __AVR_ATmega128__
#endif

#define F_CPU 16000000UL

#include "atmega128.h"

int main(void)
{
	uint8_t i = 0xFF;
	gpioc_reg()->ddr.var |= ((1 << PC0) | (1 << PC7));

	if (pgm_read_word(0x0000) == 0xFFFF) {
		for (i = 0; i < 6; i++)
		{
			gpioc_reg()->port.var &= ~(1 << PC0);
			_delay_ms(500);
			gpioc_reg()->port.var |= 1 << PC0;
			_delay_ms(500);
		}
	}else{
		for (i = 0; i < 6; i++)
		{
			gpioc_reg()->port.var &= ~(1 << PC7);
			_delay_ms(50);
			gpioc_reg()->port.var |= (1 << PC7);
			_delay_ms(50);
		}
	}
	
	boot_rww_enable();
	
	asm("jmp 0");
}

// LINK: -Wl,-section-start=.text=0x1FC00


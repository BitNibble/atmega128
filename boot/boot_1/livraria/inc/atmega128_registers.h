/************************************************************************
	ATMEGA 128 REGISTERS
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega324 by ETT ET-BASE
Date:     05072025
************************************************************************/
#ifndef _ATMEGA128_REGISTERS_H_
	#define _ATMEGA128_REGISTERS_H_
	
/*** Compiler ***/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

/*** Gloabl Library ***/
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>

/*** Base Typedef ***/
typedef union {
	struct { 
		uint8_t bit0:1,bit1:1,bit2:1,bit3:1,bit4:1,bit5:1,bit6:1,bit7:1; 
	} par;
	uint8_t var;
} U_byte;

typedef union {
	struct{
		U_byte l;
		U_byte h;
	}par;
	uint16_t var;
} U_word;

/*****************************/
/****      REGISTERS      ****/
/*****************************/
// PINF 0x20 (U_byte)
// PINE 0x21 (U_byte)
// DDRE 0x22 (U_byte)
// PORTE 0x23 (U_byte)
// ADCL ADCH 0x24 0x25 (U_word)
typedef union {
	struct{
		uint8_t adps : 3, adie : 1, adif : 1, adfr : 1, adsc : 1, aden : 1;
	}par;
	uint8_t var;
} ADCSRA_TypeDef; // 0x26
typedef union {
	struct{
		uint8_t mux : 5, adlar : 1, refs : 2;
	}par;
	uint8_t var;
} ADMUX_TypeDef; // 0x27
typedef union {
	struct{
		uint8_t acis : 2, acic : 1, acie : 1, aci : 1, aco : 1, acbg : 1, acd : 1;
	}par;
	uint8_t var;
} ACSR_TypeDef; // 0x28
// UBRR0L 0x29 (U_byte)
typedef union {
	struct{
		uint8_t txb80 : 1, rxb80 : 1, ucsz02 : 1, txen0 : 1, rxen0 : 1, udrie0 : 1, txcie0 : 1, rxcie0 : 1;
	}par;
	uint8_t var;
} UCSR0B_TypeDef; // 0x2A
typedef union {
	struct{
		uint8_t mpcm0 : 1, u2x0 : 1, upe0 : 1, dor0 : 1, fe0 : 1, udre0 : 1, txc0 : 1, rxc0 : 1;
	}par;
	uint8_t var;
} UCSR0A_TypeDef; // 0x2B
// _UDR0 0x2C (U_byte)
typedef union {
	struct{
		uint8_t spr : 2, cpha : 1, cpol : 1, mstr : 1, dord : 1, spe : 1, spie : 1;
	}par;
	uint8_t var;
} SPCR_TypeDef; // 0x2D
typedef union {
	struct{
		uint8_t spi2x : 1, fill0 : 5, wcol : 1, spif : 1;
	}par;
	uint8_t var;
} SPSR_TypeDef; // 0x2E
// SPDR 0x2F (U_byte)
// PIND 0x30 (U_byte)
// DDRD 0x31 (U_byte)
// PORTD 0x32 (U_byte)
// PINC 0x33 (U_byte)
// DDRC 0x34 (U_byte)
// PORTC 0x35 (U_byte)
// PINB 0x36 (U_byte)
// DDRB 0x37 (U_byte)
// PORTB 0x38 (U_byte)
// PINA 0x39 (U_byte)
// DDRA 0x3A (U_byte)
// PORTA 0x3B (U_byte)
typedef union {
	struct{
		uint8_t eere : 1, eewe : 1, eemwe : 1, eerie : 1, fill0 : 4;
	}par;
	uint8_t var;
} EECR_TypeDef; // 0x3C
// EEDR 0x3D (U_byte)
// EEARL EEARH 0x3E 0x3F (U_word)
typedef union {
	struct{
		uint8_t psr321 : 1, psr0 : 1, pud : 1, acme : 1, fill0 : 3, tsm : 1;
	}par;
	uint8_t var;
} SFIOR_TypeDef; // 0x40
typedef union {
	struct{
		uint8_t wdp : 3, wde : 1, wdce : 1, fill0 : 3;
	}par;
	uint8_t var;
} WDTCR_TypeDef; // 0x41
// OCDR 0x42 (U_byte)
// OCR2 0x43 (U_byte)
// TCNT2 0x44 (U_byte)
typedef union {
	struct{
		uint8_t cs2 : 3, wgm21 : 1, com2 : 2, wgm20 : 1, foc2 : 1;
	}par;
	uint8_t var;
} TCCR2_TypeDef; // 0x45
// ICR1L ICR1H 0x46 0x47 (U_word)
// OCR1BL OCR1BH 0x48 0x49 (U_word)
// OCR1AL OCR1AH 0x4A 0x4B (U_word)
// TCNT1L TCNT1H 0x4C 0x4D (U_word)
typedef union {
	struct{
		uint8_t cs1 : 3, wgm12 : 1, wgm13 : 1, fill0 : 1, ices1 : 1, icnc1 : 1;
	}par;
	uint8_t var;
} TCCR1B_TypeDef; // 0x4E
typedef union {
	struct{
		uint8_t wgm10 : 1, wgm11 : 1, com1c : 2, com1b : 2, com1a : 2;
	}par;
	uint8_t var;
} TCCR1A_TypeDef; // 0x4F
typedef union {
	struct{
		uint8_t tcr0ub : 1, ocr0ub : 1, tcn0ub : 1, as0 : 1, fill0 : 4;
	}par;
	uint8_t var;
} ASSR_TypeDef; // 0x50
// OCR0 0x51 (U_byte)
// TCNT0 0x52 (U_byte)
typedef union {
	struct{
		uint8_t cs0 : 3, wgm01 : 1, com0 : 2, wgm00 : 1, foc0 : 1;
	}par;
	uint8_t var;
} TCCR0_TypeDef; // 0x53
typedef union {
	struct{
		uint8_t porf : 1, extrf : 1, borf : 1, wdrf : 1, jtrf : 1, fill0 : 2, jtd : 1;
	}par;
	uint8_t var;
} MCUCSR_TypeDef; // 0x54
typedef union {
	struct{
		uint8_t ivce : 1, ivsel : 1, sm2 : 1, sm0 : 1, sm1 : 1, se : 1, swr10 : 1, sre : 1;
	}par;
	uint8_t var;
} MCUCR_TypeDef; // 0x55
typedef	volatile union {
	struct{
		uint8_t tov0 : 1, ocf0 : 1, tov1 : 1, ocf1b : 1, ocf1a : 1, icf1 : 1, tov2 : 1, ocf2 : 1;
	}par;
	uint8_t var;
} TIFR_Typedef; // 0x56
typedef union {
	struct{
		uint8_t toie0 : 1, ocie0a : 1, toie1 : 1, ocie1b : 1, ocie1a : 1, ticie1 : 1, toie2 : 1, ocie2 : 1;
	}par;
	uint8_t var;
} TIMSK_TypeDef; // 0x57
typedef union {
	struct{
		uint8_t intf0 : 1, intf1 : 1, intf2 : 1, intf3 : 1, intf4 : 1, intf5 : 1, intf6 : 1, intf7 : 1;
	}par;
	uint8_t var;
} EIFR_TypeDef; // 0x58
typedef union {
	struct{
		uint8_t int0 : 1, int1 : 1, int2 : 1, int3 : 1, int4 : 1, int5 : 1, int6 : 1, int7 : 1;
	}par;
	uint8_t var;
} EIMSK_TypeDef; // 0x59
typedef union {
	struct{
		uint8_t isc4 : 2, isc5 : 2, isc6 : 2, isc7 : 2;
	}par;
	uint8_t var;
} EICRB_TypeDef; // 0x5A
typedef union {
	struct{
		uint8_t rampz0 : 1, fill0 : 7;
	}par;
	uint8_t var;
} RAMPZ_TypeDef; // 0x5B
typedef union {
	struct{
		uint8_t xdiv : 7, xdiven : 1;
	}par;
	uint8_t var;
} XDIV_TypeDef; // 0x5C
// SPL SPH 0x5D 0x5E (U_word)
typedef union {
	struct{
		uint8_t c : 1, z : 1, n : 1, v : 1, s : 1, h : 1, t : 1, i : 1;
	}par;
	uint8_t var;
} SREG_TypeDef; // 0x5F
// Reserved 0x60
// DDRF 0x61 (U_byte)
// PORTF 0x62 (U_byte)
// PING 0x63 (U_byte)
// DDRG 0x64 (U_byte)
// PORTG 0x65 (U_byte)
// Reserved 0x66
// Reserved 0x67
typedef union {
	struct{
		uint8_t spmen : 1, pgers : 1, pgwrt : 1, blbset : 1, rwwsre : 1, fill0 : 1, rwwsb : 1, spmie : 1;
	}par;
	uint8_t var;
} SPMCSR_TypeDef; // 0x68
// Reserved 0x69
typedef union {
	struct{
		uint8_t isc0 : 2, isc1 : 2, isc2 : 2, isc3 : 2;
	}par;
	uint8_t var;
} EICRA_TypeDef; // 0x6A
// Reserved 0x6B
typedef union {
	struct{
		uint8_t xmm : 3, fill0 : 4, xmbk : 1;
	}par;
	uint8_t var;
} XMCRB_TypeDef; // 0x6C
typedef union {
	struct{
		uint8_t fill0 : 1, srw11 : 1, srw00 : 1, srw01 : 1, srl : 3, fill1 : 1;
	}par;
	uint8_t var;
} XMCRA_TypeDef; // 0x6D
// Reserved 0x6E
// OSCAL 0x6F (U_byte)
// TWBR 0x70 (U_byte)
typedef union {
	struct{
		uint8_t twps : 2, fill0 : 1, tws : 5;
	}par;
	uint8_t var;
} TWSR_TypeDef; // 0x71
typedef union {
	struct{
		uint8_t twgce : 1, twa : 7;
	}par;
	uint8_t var;
} TWAR_TypeDef; // 0x72
// TWDR 0x73 (U_byte)
typedef union {
	struct{
		uint8_t twie : 1, fill0 : 1, twen : 1, twwc : 1, twsto : 1, twsta : 1, twea : 1, twint : 1;
	}par;
	uint8_t var;
} TWCR_TypeDef; // 0x74
// Reserved 0x75
// Reserved 0x76
// Reserved 0x77
// OCR1CL OCR1CH 0x78 0x79 (U_word)
typedef union {
	struct{
		uint8_t fill0 : 5, foc1c : 1, foc1b : 1, foc1a : 1;
	}par;
	uint8_t var;
} TCCR1C_TypeDef; // 0x7A
// Reserved 0x7B
typedef union {
	struct{
		uint8_t ocf1c : 1, ocf3c : 1, tov3 : 1, ocf3b : 1, ocf3a : 1, icf3 : 1, fill0 : 2;
	}par;
	uint8_t var;
} ETIFR_TypeDef; // 0x7C
typedef union {
	struct{
		uint8_t ocie1c : 1, ocie3c : 1, toie3 : 1, ocie3b : 1, ocie3a : 1, ticie3 : 1, fill0 : 2;
	}par;
	uint8_t var;
} ETIMSK_TypeDef; // 0x7D
// Reserved 0x7E
// Reserved 0x7F
// ICR3L ICR3H 0x80 0x81 (U_word)
// OCR3CL OCR3CH 0x82 0x83 (U_word)
// OCR3BL OCR3BH 0x84 0x85 (U_word)
// OCR3AL OCR3AH 0x86 0x87 (U_word)
// TCNT3L TCNT3H 0x88 0x89 (U_word)
typedef union {
	struct{
		uint8_t cs3 : 3, wgm32 : 1, wgm33 : 1, fill0 : 1, ices3 : 1, icnc3 : 1;
	}par;
	uint8_t var;
} TCCR3B_TypeDef; // 0x8A
typedef union {
	struct{
		uint8_t wgm30 : 1, wgm31 : 1, com3c : 2, com3b : 2, com3a : 2;
	}par;
	uint8_t var;
} TCCR3A_TypeDef; // 0x8B
typedef union {
	struct{
		uint8_t fill0 : 5, foc3c : 1, foc3b : 1, foc3a : 1;
	}par;
	uint8_t var;
} TCCR3C_TypeDef; // 0x8C
// Reserved 0x8D
// Reserved 0x8E
// Reserved 0x8F
typedef union{
	struct{
		uint8_t brr0h : 4, reserved : 4;
	}par;
	uint8_t var;
} UBRR0H_TypeDef; // 0x90
// Reserved 0x91
// Reserved 0x92
// Reserved 0x93
// Reserved 0x94
typedef union {
	struct{
		uint8_t ucpol0 : 1, ucsz0 : 2, usbs0 : 1, upm0 : 2, umsel0 : 1, fill0 : 1;
	}par;
	uint8_t var;
} UCSR0C_TypeDef; // 0x95
// Reserved 0x96
// Reserved 0x97
typedef union{
	struct{
		uint8_t brr1h : 4, reserved : 4;
	}par;
	uint8_t var;
} UBRR1H_TypeDef; // 0x98
// UBRR1L 0x99 (U_byte)
typedef union {
	struct{
		uint8_t txb81 : 1, rxb81 : 1, ucsz12 : 1, txen1 : 1, rxen1 : 1, udrie1 : 1, txcie1 : 1, rxcie1 : 1;
	}par;
	uint8_t var;
} UCSR1B_TypeDef; // 0x9A
typedef union {
	struct{
		uint8_t mpcm1 : 1, u2x1 : 1, upe1 : 1, dor1 : 1, fe1 : 1, udre1 : 1, txc1 : 1, rxc1 : 1;
	}par;
	uint8_t var;
} UCSR1A_TypeDef; // 0x9B
// UDR1 0x9C (U_byte)
typedef union {
	struct{
		uint8_t ucpol1 : 1, ucsz10 : 1, ucsz11 : 1, usbs1 : 1, upm1 : 2, umsel1 : 1, fill0 : 1;
	}par;
	uint8_t var;
} UCSR1C_TypeDef; // 0x9D
// Reserved 0x9E
// Reserved 0x9F
// Reserved 0xA0
// Reserved 0xA1
// Reserved 0xA2
// Reserved 0xA3
// Reserved 0xA4
// Reserved 0xA5
// Reserved 0xA6
// Reserved 0xA7
// Reserved 0xA8
// Reserved 0xA9
// Reserved 0xAA
// Reserved 0xAB
// Reserved 0xAC
// Reserved 0xAD
// Reserved 0xAE
// Reserved 0xAF
// Reserved 0xB0
// Reserved 0xB1
// Reserved 0xB2
// Reserved 0xB3
// Reserved 0xB4
// Reserved 0xB5
// Reserved 0xB6
// Reserved 0xB7
// Reserved 0xB8
// Reserved 0xB9
// Reserved 0xBA
// Reserved 0xBB
// Reserved 0xBC
// Reserved 0xBD
// Reserved 0xBE
// Reserved 0xBF
// Reserved 0xC0
// Reserved 0xC1
// Reserved 0xC2
// Reserved 0xC3
// Reserved 0xC4
// Reserved 0xC5
// Reserved 0xC6
// Reserved 0xC7
// Reserved 0xC8
// Reserved 0xC9
// Reserved 0xCA
// Reserved 0xCB
// Reserved 0xCC
// Reserved 0xCD
// Reserved 0xCE
// Reserved 0xCF
// Reserved 0xD0
// Reserved 0xD1
// Reserved 0xD2
// Reserved 0xD3
// Reserved 0xD4
// Reserved 0xD5
// Reserved 0xD6
// Reserved 0xD7
// Reserved 0xD8
// Reserved 0xD9
// Reserved 0xDA
// Reserved 0xDB
// Reserved 0xDC
// Reserved 0xDD
// Reserved 0xDE
// Reserved 0xDF
// Reserved 0xE0
// Reserved 0xE1
// Reserved 0xE2
// Reserved 0xE3
// Reserved 0xE4
// Reserved 0xE5
// Reserved 0xE6
// Reserved 0xE7
// Reserved 0xE8
// Reserved 0xE9
// Reserved 0xEA
// Reserved 0xEB
// Reserved 0xEC
// Reserved 0xED
// Reserved 0xEE
// Reserved 0xEF
// Reserved 0xF0
// Reserved 0xF1
// Reserved 0xF2
// Reserved 0xF3
// Reserved 0xF4
// Reserved 0xF5
// Reserved 0xF6
// Reserved 0xF7
// Reserved 0xF8
// Reserved 0xF9
// Reserved 0xFA
// Reserved 0xFB
// Reserved 0xFC
// Reserved 0xFD
// Reserved 0xFE
// Reserved 0xFF

#endif
/*** EOF ***/


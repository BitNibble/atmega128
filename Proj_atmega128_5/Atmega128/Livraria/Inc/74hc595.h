/************************************************************************
	74HC595
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: 74HC595
Date:     25/10/2020
************************************************************************/
#ifndef _74HC595_H_
	#define _74HC595_H_

/*** Library ***/
#include <stdint.h>
#include <inttypes.h>
	
/*** Constant & Macro ***/
//#define STM32F4
#if defined (STM32F4)
	#define IO_var uint32_t
#else
	#define IO_var uint8_t
#endif

/*** Parameter ***/
typedef struct{
	uint8_t HC595_datapin;
	uint8_t HC595_clkpin;
	uint8_t HC595_outpin;
	volatile IO_var *hc595_DDR;
	volatile IO_var *hc595_PORT;
}HC595_Parameter;

/*** Handler ***/
typedef struct
{
	HC595_Parameter par;
	// V-table
	void (*bit)(HC595_Parameter* par, uint8_t state);
	void (*ibyte)(HC595_Parameter* par, uint8_t byte);
	void (*byte)(HC595_Parameter* par, uint8_t byte);
	void (*out)(HC595_Parameter* par);
}HC595_Handler;

HC595_Handler hc595_enable(volatile IO_var *ddr, volatile IO_var *port, uint8_t datapin, uint8_t clkpin, uint8_t outpin);

#endif
/*** EOF ***/

/******
1º Sequence
2º Scope
	- Library Scope
	- File Scope
	- Function Scope
	- Precedence Scope
3º Pointer and Variable
4º Casting
******/


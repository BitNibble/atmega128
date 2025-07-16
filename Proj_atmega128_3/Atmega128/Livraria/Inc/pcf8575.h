/***************************************************************************************************
	PCF8575
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: PCF8575
Date:     13072025
***************************************************************************************************/
#ifndef _PCF8575_H_
	#define _PCF8575_H_

/*** Library ***/
#include <inttypes.h>

#if defined __AVR_Atmega328__ || defined __AVR_ATmega328P__
	#include "Atmega328twi.h"
#elif defined __AVR_ATmega64__ || defined __AVR_ATmega128__ || defined __AVR_ATmega128A__
	#include "atmega128twi.h"
#else
	#error "MCU Lib not available"
#endif

/*** Constant & Macro ***/
#ifndef HIGH
	#define HIGH 0xFF
#endif
#ifndef LOW
	#define LOW 0x00
#endif

#define PCF8575_BASE_ADDRESS 32

/*** Parameter ***/
typedef struct{
	TWI0_Handler* twi;
	uint8_t pcf8575_id;
	uint16_t state;
}PCF8575_Parameter;

/*** Handler ***/
typedef struct{
	PCF8575_Parameter par;
	
	// V-table
	void (*writehbits)( PCF8575_Parameter *par, uint16_t hbits, uint8_t state );
	uint16_t (*readhbits)( PCF8575_Parameter *par, uint16_t hbits );
}PCF8575_Handler;

PCF8575_Handler pcf8575_enable( uint8_t pcf8575_id, uint8_t twi_prescaler );

#endif
/*** EOF ***/


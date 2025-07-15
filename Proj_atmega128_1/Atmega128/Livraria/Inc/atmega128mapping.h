/*********************************************************
	ATMEGA 128 MAPPING
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     15/07/2025
**********************************************************/
#ifndef _ATMEGA128MAPPING_H_
	#define _ATMEGA128MAPPING_H_

#include "atmega128.h"

/*********************************************************/
/****************** Include Switches  ********************/
/*********************************************************/
// User may Comment out if not in use
// MODULES
#define _ANALOG_MODULE_
#define _TIMER0_MODULE_
#define _TIMER1_MODULE_
#define _TIMER2_MODULE_
#define _TIMER3_MODULE_
#define _EEPROM_MODULE_
#define _INTERRUPT_MODULE_
#define _SPI_MODULE_
#define _TWI_MODULE_
#define _USART0_MODULE_
#define _USART1_MODULE_

/*********************************************************/
/******************* Hardware Library ********************/
/*********************************************************/
// MODULES
#ifdef _ANALOG_MODULE_
	#include "atmega128analog.h"
//#else
//	#warning "NO ANALOG MODULE"
#endif
#ifdef _TIMER0_MODULE_
	#include "atmega128timer0.h"
//#else
//	#warning "NO TIMER0 MODULE"
#endif
#ifdef _TIMER1_MODULE_
#include "atmega128timer1.h"
//#else
//	#warning "NO TIMER1 MODULE"
#endif
#ifdef _TIMER2_MODULE_
#include "atmega128timer2.h"
//#else
//	#warning "NO TIMER2 MODULE"
#endif
#ifdef _TIMER3_MODULE_
#include "atmega128timer3.h"
//#else
//	#warning "NO TIMER3 MODULE"
#endif
#ifdef _EEPROM_MODULE_
	#include "atmegaeeprom.h"
//#else
//	#warning "NO EEPROM MODULE"
#endif
#ifdef _INTERRUPT_MODULE_
	#include "atmega128interrupt.h"
//#else
//	#warning "NO INTERRUPT MODULE"
#endif
#ifdef _SPI_MODULE_
#include "atmega128spi.h"
//#else
//	#warning "NO SPI MODULE"
#endif
#ifdef _TWI_MODULE_
	#include "atmega128twi.h"
//#else
//	#warning "NO TWI MODULE"
#endif
#ifdef _USART0_MODULE_
#include "atmega128usart0.h"
//#else
//	#warning "NO UART0 MODULE"
#endif
#ifdef _USART1_MODULE_
#include "atmega128usart1.h"
//#else
//	#warning "NO UART1 MODULE"
#endif

/*******************************************************************/
/************************* ATMEGA 128 IMAGE ************************/
/*************************** Third Layer ***************************/
/*******************************************************************/
typedef struct {
	/***GPWR***/
	// removed, no user permission.
	/***AC***/
	Atmega128AnalogComparator_TypeDef* ac_reg;
	/***ADC***/
	Atmega128AnalogToDigitalConverter_TypeDef* adc_reg;
	#ifdef _ANALOG_MODULE_
		void (*adc_enable)( uint8_t Vreff, uint8_t Divfactor, int n_channel, ... );
	#endif
	/***BOOTLOAD***/
	Atmega128BootLoader_TypeDef* bootload_reg;
	/***CPU***/
	Atmega128CPURegister_TypeDef* cpu_reg;
	/***EEPROM***/
	Atmega128Eeprom_TypeDef* eeprom_reg;
	/***EXINT***/
	Atmega128ExternalInterrupts_TypeDef* exint_reg;
	/***PORTA***/
	Atmega128PORTA_TypeDef* gpioa_reg;
	/***PORTB***/
	Atmega128PORTB_TypeDef* gpiob_reg;
	/***PORTC***/
	Atmega128PORTC_TypeDef* gpioc_reg;
	/***PORTD***/
	Atmega128PORTD_TypeDef* gpiod_reg;
	/***PORTE***/
	Atmega128PORTE_TypeDef* gpioe_reg;
	/***PORTF***/
	Atmega128PORTF_TypeDef* gpiof_reg;
	/***PORTG***/
	Atmega128PORTG_TypeDef* gpiog_reg;
	/***JTAG***/
	Atmega128JtagInterface_TypeDef* jtag_reg;
	/***MISC***/
	Atmega128OtherRegisters_TypeDef* misc_reg;
	/***SPI***/
	Atmega128SerialPeripheralInterface_TypeDef* spi_reg;
	#ifdef _SPI_MODULE_
		void (*spi_enable)( uint8_t master_slave_select, uint8_t data_order,  uint8_t data_modes, uint8_t prescaler );
	#endif
	/***TC1***/
	Atmega128TimerCounter1_TypeDef* tc1_reg;
	#ifdef _TIMER1_MODULE_
		void  (*tc1_enable)(uint8_t wavegenmode, uint8_t interrupt);
	#endif
	/***TC3***/
	Atmega128TimerCounter3_TypeDef* tc3_reg;
	#ifdef _TIMER3_MODULE_
		void (*tc3_enable)(unsigned char wavegenmode, unsigned char interrupt);
	#endif
	/***TC2***/
	Atmega128TimerCounter2_TypeDef* tc2_reg;
	#ifdef _TIMER2_MODULE_
		void (*tc2_enable)(unsigned char wavegenmode, unsigned char interrupt);
	#endif
	/***TC0***/
	Atmega128TimerCounter0_TypeDef* tc0_reg;
	#ifdef _TIMER0_MODULE_
		void (*tc0_enable)(unsigned char wavegenmode, unsigned char interrupt);
	#endif
	/***TWI***/
	Atmega128TwoWireSerialInterface_TypeDef* twi_reg;
	#ifdef _TWI_MODULE_
		void (*twi_enable)(uint8_t atmega_id,  uint8_t prescaler);
	#endif
	/***USART0***/
	Atmega128Usart0_TypeDef* usart0_reg;
	#ifdef _USART0_MODULE_
		void (*usart0_enable)( uint32_t baudrate, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
	#endif
	/***USART1***/
	Atmega128Usart1_TypeDef* usart1_reg;
	#ifdef _USART1_MODULE_
		void (*usart1_enable)( uint32_t baud, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
	#endif
	/***WDT***/
	Atmega128WatchdogTimer_TypeDef* wdt_reg;
	/***Pointer Function***/
	void (*Clock_Prescaler_Select)( volatile uint8_t prescaler );
	void (*Move_Interrupts_To_Boot)( void );
}ATMEGA128;

ATMEGA128* atmega128( void );

#endif
/*** EOF ***/


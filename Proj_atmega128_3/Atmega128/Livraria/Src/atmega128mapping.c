/**********************************************************
	ATMEGA 128 MAPPING
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     15/07/2025
**********************************************************/
/*** Library ***/
#include "atmega128mapping.h"

/*** Handler ***/
static const ATMEGA128 setup_atmega128 = { 
	/***GPWR***/
	// removed due to be restricted to user.
	/***AC***/
	.ac_reg = (Atmega128AnalogComparator_TypeDef*) 0x0028,
	/***ADC***/
	.adc_reg = (Atmega128AnalogToDigitalConverter_TypeDef*) 0x0024,
	#ifdef _ANALOG_MODULE_
		.adc_enable = adc_enable,
	#endif
	/***BOOTLOAD***/
	.bootload_reg = (Atmega128BootLoader_TypeDef*) 0x0068,
	/***CPU***/
	.cpu_reg = (Atmega128CPURegister_TypeDef*) 0x0054,
	/***EEPROM***/
	.eeprom_reg = (Atmega128Eeprom_TypeDef*) 0x003C,
	/***EXINT***/
	.exint_reg = (Atmega128ExternalInterrupts_TypeDef*) 0x0058,
	/***PORTA***/
	.gpioa_reg = (Atmega128PORTA_TypeDef*) 0x0039,
	/***PORTB***/
	.gpiob_reg = (Atmega128PORTB_TypeDef*) 0x0036,
	/***PORTC***/
	.gpioc_reg = (Atmega128PORTC_TypeDef*) 0x0033,
	/***PORTD***/
	.gpiod_reg = (Atmega128PORTD_TypeDef*) 0x0030,
	/***PORTE***/
	.gpioe_reg = (Atmega128PORTE_TypeDef*) 0x0021,
	/***PORTF***/
	.gpiof_reg = (Atmega128PORTF_TypeDef*) 0x0020,
	/***PORTG***/
	.gpiog_reg = (Atmega128PORTG_TypeDef*) 0x0063,
	/***JTAG***/
	.jtag_reg = (Atmega128JtagInterface_TypeDef*) 0x0042,
	/***MISC***/
	.misc_reg = (Atmega128OtherRegisters_TypeDef*) 0x0040,
	/***SPI***/
	.spi_reg = (Atmega128SerialPeripheralInterface_TypeDef*) 0x002D,
	#ifdef _SPI_MODULE_
		.spi_enable = spi_enable,
	#endif
	/***TC1***/
	.tc1_reg = (Atmega128TimerCounter1_TypeDef*) 0x0040,
	#ifdef _TIMER1_MODULE_
		.tc1_enable = tc1_enable,
	#endif
	/***TC3***/
	.tc3_reg = (Atmega128TimerCounter3_TypeDef*) 0x0040,
	#ifdef _TIMER3_MODULE_
		.tc3_enable = tc3_enable,
	#endif
	/***TC2***/
	.tc2_reg = (Atmega128TimerCounter2_TypeDef*) 0x0043,
	#ifdef _TIMER2_MODULE_
		.tc2_enable = tc2_enable,
	#endif
	/***TC0***/
	.tc0_reg = (Atmega128TimerCounter0_TypeDef*) 0x0040,
	#ifdef _TIMER0_MODULE_
		.tc0_enable = tc0_enable,
	#endif
	/***TWI***/
	.twi_reg = (Atmega128TwoWireSerialInterface_TypeDef*) 0x0070,
	#ifdef _TWI_MODULE_
		.twi_enable = twi_enable,
	#endif
	/***USART0***/
	.usart0_reg = (Atmega128Usart0_TypeDef*) 0x0029,
	#ifdef _USART0_MODULE_
		.usart0_enable = usart0_enable,
	#endif
	/***USART1***/
	.usart1_reg = (Atmega128Usart1_TypeDef*) 0x0098,
	#ifdef _USART1_MODULE_
		.usart1_enable = usart1_enable,
	#endif
	/***WDT***/
	.wdt_reg = (Atmega128WatchdogTimer_TypeDef*) 0x0041,
	/***Pointer Function***/
	.Clock_Prescaler_Select = ClockPrescalerSelect,
	.Move_Interrupts_To_Boot = MoveInterruptsToBoot
};

ATMEGA128* atmega128(void){ return (ATMEGA128*) &setup_atmega128; }

/*** EOF ***/


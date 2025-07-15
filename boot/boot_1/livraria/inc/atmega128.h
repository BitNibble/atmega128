/***********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:	  05072025
***********************************************************************/
#ifndef _ATMEGA128_H_
	#define _ATMEGA128_H_

/*** Compiler ***/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Global Library ***/
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/fuse.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "atmega128_registers.h"

/*** Global Constant & Macro ***/
#define TWO 2
#define NIBBLE_BITS 4
#define BYTE_BITS 8
#define WORD_BITS 16
#define DWORD_BITS 32
#define QWORD_BITS 64
#define SRAMSTART 0x0100
#define SRAMEND 0x10FF
// Macros for common operations
#define SET_REG(REG, HBITS)		(REG |= HBITS)
#define CLEAR_REG(REG, HBITS)	(REG &= ~HBITS)
#define READ_BIT(REG, BIT)		((REG >> BIT) & 1)
#define TOGGLE_REG(REG, HBITS)	(REG ^= HBITS)

/*****************************/
/**** MAIN HARDWARE LAYER ****/
/*****************************/
// GPWR
typedef volatile struct {
	uint8_t r0; // 0x00
	uint8_t r1; // 0x01
	uint8_t r2; // 0x02
	uint8_t r3; // 0x03
	uint8_t r4; // 0x04
	uint8_t r5; // 0x05
	uint8_t r6; // 0x06
	uint8_t r7; // 0x07
	uint8_t r8; // 0x08
	uint8_t r9; // 0x09
	uint8_t r10; // 0x0A
	uint8_t r11; // 0x0B
	uint8_t r12; // 0x0C
	uint8_t r13; // 0x0D
	uint8_t r14; // 0x0E
	uint8_t r15; // 0x0F
	uint8_t r16; // 0x10
	uint8_t r17; // 0x11
	uint8_t r18; // 0x12
	uint8_t r19; // 0x13
	uint8_t r20; // 0x14
	uint8_t r21; // 0x15
	uint8_t r22; // 0x16
	uint8_t r23; // 0x17
	uint8_t r24; // 0x18
	uint8_t r25; // 0x19
	uint16_t X; // 0x1A 0x1B
	uint16_t Y; // 0x1C 0x1D
	uint16_t Z; // 0x1E 0x1F
} Atmega128GPWR_TypeDef;

Atmega128GPWR_TypeDef* gpwr_reg(void);

// Analog Comparator (AC)
typedef volatile struct {
	ACSR_TypeDef acsr; // 0x28
	uint8_t fill[23]; // (40 - 28) - 1
	SFIOR_TypeDef sfior; // 0x40
} Atmega128AnalogComparator_TypeDef;

Atmega128AnalogComparator_TypeDef* ac_reg(void);

// Analog to Digital Converter (ADC)
typedef volatile struct {
	U_word adc; // 0x24 0x25
	ADCSRA_TypeDef adcsra; // 0x26
	ADMUX_TypeDef admux; // 0x27
} Atmega128AnalogToDigitalConverter_TypeDef;

Atmega128AnalogToDigitalConverter_TypeDef* adc_reg(void);

// Boot loader (BOOT_LOAD)
typedef volatile struct {
	SPMCSR_TypeDef spmcsr; // 0x68
} Atmega128BootLoader_TypeDef;

Atmega128BootLoader_TypeDef* bootload_reg(void);

// CPU Register (CPU)
typedef volatile struct {
	MCUCSR_TypeDef mcucsr; // 0x54
	MCUCR_TypeDef mcucr; // 0x55
	uint8_t fill1[5]; // (5B - 55) - 1
	RAMPZ_TypeDef rampz; // 0x5B
	XDIV_TypeDef xdiv; // 0x5C
	U_word sp; // 0x5D 0x5E
	SREG_TypeDef sreg; // 0x5F
	uint8_t fill2[12]; // (6C - 5F) - 1
	XMCRB_TypeDef xmcrb; // 0x6C
	XMCRA_TypeDef xmcra; // 0x6D
	uint8_t fill3; // (6F - 6D) - 1
	U_byte osccal; // 0x6F
} Atmega128CPURegister_TypeDef;

Atmega128CPURegister_TypeDef* cpu_reg(void);

// EEPROM (EEPROM)
typedef volatile struct {
	EECR_TypeDef eecr; // 0x3C
	U_byte eedr; // 0x3D
	U_word eear; // 0x3E 0x3F
} Atmega128Eeprom_TypeDef;

Atmega128Eeprom_TypeDef* eeprom_reg(void);

// External Interrupts (EXINT)
typedef volatile struct {
	EIFR_TypeDef eifr; // 0x58
	EIMSK_TypeDef eimsk; // 0x59
	EICRB_TypeDef eicrb; // 0x5A
	uint8_t fill[15]; // (6A - 5A) - 1
	EICRA_TypeDef eicra; // 0x6A
} Atmega128ExternalInterrupts_TypeDef;

Atmega128ExternalInterrupts_TypeDef* exint_reg(void);

// I/O Port (PORTA)
typedef volatile struct {
	U_byte pin; // 0x39
	U_byte ddr; // 0x3A
	U_byte port; // 0x3B
} Atmega128PORTA_TypeDef;

Atmega128PORTA_TypeDef* gpioa_reg(void);

// I/O Port (PORTB)
typedef volatile struct {
	U_byte pin; // 0x36
	U_byte ddr; // 0x37
	U_byte port; // 0x38
} Atmega128PORTB_TypeDef;

Atmega128PORTB_TypeDef* gpiob_reg(void);

// I/O Port (PORTC)
typedef volatile struct {
	U_byte pin; // 0x33
	U_byte ddr; // 0x34
	U_byte port; // 0x35
} Atmega128PORTC_TypeDef;

Atmega128PORTC_TypeDef* gpioc_reg(void);

// I/O Port (PORTD)
typedef volatile struct {
	U_byte pin; // 0x30
	U_byte ddr; // 0x31
	U_byte port; // 0x32
} Atmega128PORTD_TypeDef;

Atmega128PORTD_TypeDef* gpiod_reg(void);

// I/O Port (PORTE)
typedef volatile struct {
	U_byte pin; // 0x21
	U_byte ddr; // 0x22
	U_byte port; // 0x23
} Atmega128PORTE_TypeDef;

Atmega128PORTE_TypeDef* gpioe_reg(void);

// I/O Port (PORTF)
typedef volatile struct {
	U_byte  pin; // 0x20
	uint8_t fill[64]; // (0x61 - 0x20) - 1
	U_byte ddr; // 0x61
	U_byte port; // 0x62
} Atmega128PORTF_TypeDef;

Atmega128PORTF_TypeDef* gpiof_reg(void);

// I/O Port (PORTG)
typedef volatile struct {
	U_byte pin; // 0x63
	U_byte ddr; // 0x64
	U_byte port; // 0x65
} Atmega128PORTG_TypeDef;

Atmega128PORTG_TypeDef* gpiog_reg(void);

// JTAG Interface (JTAG)
typedef volatile struct {
	U_byte ocdr; // 0x42
	uint8_t fill[17]; // (54 - 42) - 1
	MCUCSR_TypeDef mcucsr; // 0x54
} Atmega128JtagInterface_TypeDef;

Atmega128JtagInterface_TypeDef* jtag_reg(void);

// JTAG Interface (JTAG)
typedef volatile struct {
	MCUCSR_TypeDef mcucsr; // 0x54
} Atmega128JtagInterfaceControlStatus_TypeDef;

Atmega128JtagInterfaceControlStatus_TypeDef* jtag_cs_reg(void);

// Other Registers (MISC)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
} Atmega128OtherRegisters_TypeDef;

Atmega128OtherRegisters_TypeDef* misc_reg(void);

// Serial Peripheral Interface (SPI)
typedef volatile struct {
	SPCR_TypeDef spcr; // 0x2D
	SPSR_TypeDef spsr; // 0x2E
	U_byte spdr; // 0x2F
} Atmega128SerialPeripheralInterface_TypeDef;

Atmega128SerialPeripheralInterface_TypeDef* spi_reg(void);

// Timer/Counter, 16-bit (TC1)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
	uint8_t fill1[5]; // (46 - 40) - 1
	U_word icr1; // 0x46 0x47
	U_word ocr1b; // 0x48 0x49
	U_word ocr1a; // 0x4A 0x4B
	U_word tcnt1; // 0x4C 0x4D
	TCCR1B_TypeDef tccr1b; // 0x4E
	TCCR1A_TypeDef tccr1a; // 0x4F
	uint8_t fill2[6]; // (56 - 4F) - 1
	TIFR_Typedef tifr; // 0x56
	TIMSK_TypeDef timsk; // 0x57
	uint8_t fill3[32]; // (78 - 57) - 1
	U_word ocr1c; // 0x78 0x79
	TCCR1C_TypeDef tccr1c; // 0x7A
	uint8_t fill4; // (7C - 7A) - 1
	ETIFR_TypeDef etifr; // 0x7C
	ETIMSK_TypeDef etimsk; // 0x7D
} Atmega128TimerCounter1_TypeDef;

Atmega128TimerCounter1_TypeDef* tc1_reg(void);

// Timer/Counter, 16-bit (TC3)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
	uint8_t fill1[59]; // (7c - 40) - 1
	ETIFR_TypeDef etifr; // 0x7C
	ETIMSK_TypeDef etimsk; // 0x7D
	uint8_t fill2[2]; // (80 - 7D) - 1
	U_word icr3; // 0x80 0x81
	U_word ocr3c; // 0x82 0x83
	U_word ocr3b; // 0x84 0x85
	U_word ocr3a; // 0x86 0x87
	U_word tcnt3; // 0x88 0x89
	TCCR3B_TypeDef tccr3b; // 0x8A
	TCCR3A_TypeDef tccr3a; // 0x8B
	TCCR3C_TypeDef tccr3c; // 0x8C
} Atmega128TimerCounter3_TypeDef;

Atmega128TimerCounter3_TypeDef* tc3_reg(void);

// Timer/Counter 1 and 3
typedef volatile struct {
	ETIFR_TypeDef etifr; // 0x7C
} Atmega128TimerExternalInterruptFlag_TypeDef;

Atmega128TimerExternalInterruptFlag_TypeDef* tc_exif_reg(void);

// Timer/Counter 1 and 3
typedef volatile struct {
	ETIMSK_TypeDef etimsk; // 0x7D
} Atmega128TimerExternalInterruptMask_TypeDef;

Atmega128TimerExternalInterruptMask_TypeDef* tc_exim_reg(void);

// Timer/Counter, 8-bit (TC2)
typedef volatile struct {
	U_byte ocr2; // 0x43
	U_byte tcnt2; // 0x44
	TCCR2_TypeDef tccr2; // 0x45
	uint8_t fill[16]; // (56 - 45) - 1
	TIFR_Typedef tifr; // 0x56
	TIMSK_TypeDef timsk; // 0x57
} Atmega128TimerCounter2_TypeDef;

Atmega128TimerCounter2_TypeDef* tc2_reg(void);

// Timer/Counter, 8-bit A sync (TC0)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
	uint8_t fill1[15]; // (50 - 40) - 1
	ASSR_TypeDef assr; // 0x50
	U_byte ocr0; // 0x51
	U_byte tcnt0; // 0x52
	TCCR0_TypeDef tccr0; // 0x53
	uint8_t fill2[2]; // (56 - 53) - 1
	TIFR_Typedef tifr; // 0x56
	TIMSK_TypeDef timsk; // 0x57
} Atmega128TimerCounter0_TypeDef;

Atmega128TimerCounter0_TypeDef* tc0_reg(void);

// Timer/Counter 0, 1 and 2
typedef volatile struct {
	TIFR_Typedef tifr; // 0x56
} Atmega128TimerInterruptFlag_TypeDef;

Atmega128TimerInterruptFlag_TypeDef* tc_if_reg(void);

// Timer/Counter 0, 1 and 2
typedef volatile struct {
	TIMSK_TypeDef timsk; // 0x57
} Atmega128TimerInterruptMask_TypeDef;

Atmega128TimerInterruptMask_TypeDef* tc_im_reg(void);

// Two Wire Serial Interface (TWI)
typedef volatile struct {
	U_byte twbr; // 0x70
	TWSR_TypeDef twsr; // 0x71
	TWAR_TypeDef twar; // 0x72
	U_byte twdr; // 0x73
	TWCR_TypeDef twcr; // 0x74
} Atmega128TwoWireSerialInterface_TypeDef;

Atmega128TwoWireSerialInterface_TypeDef* twi_reg(void);

// USART (USART0)
typedef volatile struct {
	U_byte ubrr0l; // 0x29
	UCSR0B_TypeDef ucsr0b; // 0x2A
	UCSR0A_TypeDef ucsr0a; // 0x2B
	U_byte udr0; // 0x2C
	uint8_t fill1[99]; // (90 - 2C) - 1
	UBRR0H_TypeDef ubrr0h; // 0x90
	uint8_t fill2[4]; // (95 - 90) - 1
	UCSR0C_TypeDef ucsr0c; // 0x95
} Atmega128Usart0_TypeDef;

Atmega128Usart0_TypeDef* usart0_reg(void);

// USART (USART1)
typedef volatile struct {
	UBRR1H_TypeDef ubrr1h; // 0x98
	U_byte ubrr1l; // 0x99
	UCSR1B_TypeDef ucsr1b; // 0x9A
	UCSR1A_TypeDef ucsr1a; // 0x9B
	U_byte udr1; // 0x9C
	UCSR1C_TypeDef ucsr1c; // 0x9D
} Atmega128Usart1_TypeDef;

Atmega128Usart1_TypeDef* usart1_reg(void);

// Watchdog Timer (WDT)
typedef volatile struct {
	WDTCR_TypeDef wdtcr; // 0x41
} Atmega128WatchdogTimer_TypeDef;

Atmega128WatchdogTimer_TypeDef* wdt_reg(void);

/*********************************************************************/
/********** Atmega 128 Procedure and Function declaration ************/
/*********************************************************************/
uint16_t readHLbyte(U_word reg);
uint16_t readLHbyte(U_word reg);
U_word writeHLbyte(uint16_t val);
U_word writeLHbyte(uint16_t val);
uint16_t swapbyte(uint16_t num);
uint16_t BAUDRATEnormal(uint32_t BAUD);
uint16_t BAUDRATEdouble(uint32_t BAUD);
uint16_t BAUDRATEsynchronous(uint32_t BAUD);
void ClockPrescalerSelect(volatile uint8_t prescaler);
void MoveInterruptsToBoot(void);
/*** Procedure and Function ToolSet ***/
void set_reg(volatile uint8_t* reg, uint8_t hbits);
void clear_reg(volatile uint8_t* reg, uint8_t hbits);
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t bit_n);
uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk);
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data);
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data);
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n);
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
/*** NULL Check ***/
int isPtrNull(void* ptr);
int isCharPtrFlush(void* ptr);
/*** Fall Threw Delay ***/
int ftdelayCycles(uint8_t lock_ID, unsigned int n_cycle);
void ftdelayReset(uint8_t ID);

#endif

/*** Interrupt Vectors FLASH ***
typedef struct { // IVSEL = 0
	U_word RESET_vect; // 0x0000
	U_word EXT_INT0_vect; // 0x0002
	U_word EXT_INT1_vect; // 0x0004
	U_word EXT_INT2_vect; // 0x0006
	U_word EXT_INT3_vect; // 0x0008
	U_word EXT_INT4_vect; // 0x000A
	U_word EXT_INT5_vect; // 0x000C
	U_word EXT_INT6_vect; // 0x000E
	U_word EXT_INT7_vect; // 0x0010
	U_word TIM2_COMP_vect; // 0x0012
	U_word TIM2_OVF_vect; // 0x0014
	U_word TIM1_CAPT_vect; // 0x0016
	U_word TIM1_COMPA_vect; // 0x0018
	U_word TIM1_COMPB_vect; // 0x001A
	U_word TIM1_OVF_vect; // 0x001C
	U_word TIM0_COMP_vect; // 0x001E
	U_word TIM0_OVF_vect; // 0x0020
	U_word SPI_STC_vect; // 0x0022
	U_word USART0_RXC_vect; // 0x0024
	U_word USART0_DRE_vect; // 0x0026
	U_word USART0_TXC_vect; // 0x0028
	U_word ADC_vect; // 0x002A
	U_word EE_RDY_vect; // 0x002C
	U_word ANA_COMP_vect; // 0x002E
	U_word TIM1_COMPC_vect; // 0x0030
	U_word TIM3_CAPT_vect; // 0x0032
	U_word TIM3_COMPA_vect; // 0x0034
	U_word TIM3_COMPB_vect; // 0x0036
	U_word TIM3_COMPC_vect; // 0x0038
	U_word TIM3_OVF_vect; // 0x003A
	U_word USART1_RXC_vect; // 0x003C
	U_word USART1_DRE_vect; // 0x003E
	U_word USART1_TXC_vect; // 0x0040
	U_word TWI_vect; // 0x0042
	U_word SPM_RDY_vect; // 0x0044
} Atmega128InterruptVectors_TypeDef;
********************************/

/*** EOF ***/


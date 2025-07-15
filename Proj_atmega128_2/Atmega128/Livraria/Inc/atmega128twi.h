/***************************************************************************************************
	ATMEGA128TWI
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     13/07/2025
 **************************************************************************************************/
#ifndef _ATMEGA128TWI_H_
#define _ATMEGA128TWI_H_

/*** Library ***/
#include <util/twi.h>
#include "atmega128.h"

/*** Constant & Macro ***/
#ifndef _TWI_MODULE_
	#define _TWI_MODULE_
#endif
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif
#ifndef TWI_SCL_CLOCK
	#define	TWI_SCL_CLOCK 100000UL
#endif
#define TWI_NACK 0
#define TWI_ACK 1
#define TWI_WRITE 0
#define TWI_READ 1
/**************/
#define TWI_T_START 0X08
#define TWI_T_REPEATSTART 0X10
// Status Codes for MASTER Transmitter Mode
#define TWI_M_SLAW_R_ACK 0X18
#define TWI_M_SLAW_R_NACK 0X20
#define TWI_M_DATABYTE_T_ACK 0X28  // fixed correct status
#define TWI_M_DATABYTE_T_NACK 0X30
#define TWI_ARB_LOST 0X38           // combined arb lost status
// Status Codes for Master Receiver Mode
#define TWI_M_SLAR_R_ACK 0X40
#define TWI_M_SLAR_R_NACK 0X48
#define TWI_M_DATABYTE_R_ACK 0X50
#define TWI_M_DATABYTE_R_NACK 0X58
// Status Codes for SLAVE Receiver Mode
#define TWI_SR_OSLAW_T_ACK 0X60
#define TWI_MARBLSLARW_SR_OSLAW_T_ACK 0X68
#define TWI_SR_GCALL_T_ACK 0X70
#define TWI_MARBLSLARW_SR_GCALL_T_ACK 0X78
#define TWI_POSLAW_SR_DATABYTE_T_ACK 0X80
#define TWI_POSLAW_SR_DATABYTE_T_NACK 0X88
#define TWI_PGCALL_SR_DATABYTE_T_ACK 0X90
#define TWI_PGCALL_SR_DATABYTE_T_NACK 0X98
#define TWI_SR_STOPREPEATSTART 0XA0
// Status Codes for Slave Transmitter Mode
#define TWI_ST_OSLAR_T_ACK 0XA8
#define TWI_MARBLSLARW_ST_OSLAR_T_ACK 0XB0
#define TWI_ST_DATABYTE_R_ACK 0XB8
#define TWI_ST_DATABYTE_R_NACK 0XC0
#define TWI_ST_LASTDATABYTE_R_ACK 0XC8
// Miscellaneous States
#define TWI_TWINT_AT_ZERO 0XF8
#define TWI_BUS_ERROR 0X00
// Masks
#define TWI_IO_MASK 0x03
#define TWI_STATUS_MASK 0xF8
#define TWI_PRESCALER_MASK 0x03
#define TWI_ADDRESS_REGISTER_MASK 0xFE
#define Nticks 1023 // anti polling freeze.

/*** Handler ***/
typedef struct{
	// V-table
	void (*start)(void);
	uint8_t (*connect)(uint8_t address, uint8_t rw);  // now returns uint8_t status
	uint8_t (*master_write)(uint8_t var_twiData_u8);  // now returns uint8_t status
	uint8_t (*master_read)(uint8_t ack_nack);
	void (*stop)(void);
	uint8_t (*status)(void);

	// Optional: slave enable stub for future
	// void (*slave_enable)(uint8_t address);
}TWI0;

void twi_enable(uint8_t atmega_ID, uint8_t prescaler);
TWI0* twi(void);

#endif
/*** EOF ***/


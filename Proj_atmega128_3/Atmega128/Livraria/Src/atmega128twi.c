/***************************************************************************************************
	ATMEGA128TWI
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     13/07/2025
***************************************************************************************************/
#include "atmega128twi.h"
#include <util/delay.h>

/*** Procedure and Function declaratiom ***/
void TWI_start(void);
uint8_t TWI_connect(uint8_t address, uint8_t rw);
uint8_t TWI_master_write(uint8_t var_twiData_u8);
uint8_t TWI_master_read(uint8_t ack_nack);
void TWI_stop(void);
// auxiliary
uint8_t TWI_status(void);
void TWI_wait_twint(uint16_t nticks);

/*** Internal State ***/
static TWI0_Handler atmega128_twi = {
	// V-table
	.start = TWI_start,
	.connect = TWI_connect,
	.stop = TWI_stop,
	.master_write = TWI_master_write,
	.master_read = TWI_master_read,
	.status = TWI_status,
	// .slave_enable = NULL, // optional stub
};

/*** Handler ***/
void twi_enable(uint8_t atmega_id,  uint8_t prescaler)
{
	if(atmega_id > 0 && atmega_id < 128){
		twi_reg()->twar.par.twa = atmega_id;
		twi_reg()->twar.par.twgce = 1;
		}else{
		twi_reg()->twar.par.twgce = 1;
	}
	gpiod_reg()->ddr.var |= TWI_IO_MASK;
	gpiod_reg()->port.var |= TWI_IO_MASK;
	switch(prescaler){
		case 1:
		twi_reg()->twsr.par.twps = 0;
		break;
		case 4:
		twi_reg()->twsr.par.twps = 1;
		break;
		case 16:
		twi_reg()->twsr.par.twps = 2;
		break;
		case 64:
		twi_reg()->twsr.par.twps = 3;
		break;
		default:
		prescaler = 1;
		twi_reg()->twsr.par.twps = 0;
		break;
	}
	twi_reg()->twbr.var = ((F_CPU / TWI_SCL_CLOCK) - 16) / (2 * prescaler);
}

TWI0_Handler* twi(void){ return &atmega128_twi; }

/*** Procedure and Function definition ***/
void TWI_start(void)
{
	uint8_t cmd = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	twi_reg()->twcr.var = cmd;

	TWI_wait_twint(Nticks);

	switch(TWI_status()){
		case TWI_T_START:
		// OK
		break;
		default:
		TWI_stop(); // error recovery
		break;
	}
}

uint8_t TWI_connect(uint8_t address, uint8_t rw)
{
	uint8_t cmd = (address << 1) | (rw & 0x01);
	twi_reg()->twdr.var = cmd;

	cmd = (1 << TWINT) | (1 << TWEN);
	twi_reg()->twcr.var = cmd;

	TWI_wait_twint(Nticks);

	switch(TWI_status()){
		case TWI_M_SLAW_R_ACK:
		case TWI_M_SLAR_R_ACK:
		return 0; // success
		default:
		TWI_stop(); // error recovery
		return 1; // error
	}
}

uint8_t TWI_master_write(uint8_t var_twiData_u8)
{
	twi_reg()->twdr.var = var_twiData_u8;

	uint8_t cmd = (1 << TWINT) | (1 << TWEN);
	twi_reg()->twcr.var = cmd;

	TWI_wait_twint(Nticks);

	switch(TWI_status()){
		case TWI_M_DATABYTE_T_ACK:
		return 0; // success
		default:
		TWI_stop(); // error recovery
		return 1; // error
	}
}

uint8_t TWI_master_read(uint8_t ack_nack)
{
	uint8_t cmd = (1 << TWINT) | (1 << TWEN);
	if(ack_nack)
	cmd |= (1 << TWEA);
	twi_reg()->twcr.var = cmd;

	TWI_wait_twint(Nticks);

	if(TWI_status() == TWI_ARB_LOST){
		TWI_stop();
		// can return special error value? For now just 0
	}

	return twi_reg()->twdr.var;
}

void TWI_stop(void)
{
	uint8_t cmd = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	twi_reg()->twcr.var = cmd;

	_delay_us(100); // small delay to ensure stop completes
}

uint8_t TWI_status(void)
{
	return twi_reg()->twsr.var & TWI_STATUS_MASK;
}

void TWI_wait_twint(uint16_t nticks)
{
	unsigned int i;
	for(i = 0; !(twi_reg()->twcr.var & (1 << TWINT)); i++){
		_delay_us(1);
		if(i > nticks) // timeout
		break;
	}
}

/*** EOF ***/


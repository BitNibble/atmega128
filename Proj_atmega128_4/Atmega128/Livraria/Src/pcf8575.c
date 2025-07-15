/***************************************************************************************************
	PCF8575
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: PCF8575
Date:     13072025
***************************************************************************************************/
/*** Library ***/
#include "pcf8575.h"

/*** Procedure and Function declaration ***/
void PCF8575_writehbits( pcf8575_parameter *par, uint16_t hbits, uint8_t state );
uint16_t PCF8575_readhbits( pcf8575_parameter *par, uint16_t hbits );

/*** Handler ***/
PCF8575 pcf8575_enable(uint8_t pcf8575_id, uint8_t twi_prescaler)
{
	twi_enable(pcf8575_id, twi_prescaler);
	
	PCF8575 setup_pcf8575 = {
		.par = {
			.twi =  twi(),
			.pcf8575_id = pcf8575_id,
			.state = 0xFFFF
		},
		// V-table
		.writehbits = PCF8575_writehbits,
		.readhbits = PCF8575_readhbits
	};
	PCF8575_writehbits(&setup_pcf8575.par,65535,1);
	
	return setup_pcf8575;
}

/*** Procedure and Function definition ***/
void PCF8575_writehbits(pcf8575_parameter *par, uint16_t hbits, uint8_t state)
{
	if(state) par->state |= hbits; else par->state &= ~hbits;
	par->twi->start();
	par->twi->connect(par->pcf8575_id,TWI_WRITE);
	par->twi->master_write(writeHLbyte(par->state).par.l.var);
	par->twi->master_write(writeHLbyte(par->state).par.h.var);
	par->twi->stop();
}

uint16_t PCF8575_readhbits(pcf8575_parameter *par, uint16_t hbits)
{
	U_word data;
	par->twi->start();
	par->twi->connect(par->pcf8575_id, TWI_READ);
	data.par.l.var = par->twi->master_read(TWI_ACK);
	data.par.h.var = par->twi->master_read(TWI_NACK);
	par->twi->stop();
	return (readHLbyte(data) & hbits);
}

/*** EOF ***/


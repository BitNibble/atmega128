/*************************************************************************
	HX711
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License     
Hardware: Atmega 128
Date:     08032021_start
************************************************************************/
/*** Library ***/
#include "hx711.h"
#include <avr/io.h>

/*** Variable ***/
HX711_Calibration HX711_Default_50Kg = {
	.offset_32 = 35800,
	.offset_64 = 71600,
	.offset_128 = 143200,
	.divfactor_32 = 23,
	.divfactor_64 = 46,
	.divfactor_128 = 92,
	.status = ZERO
};

volatile uint8_t *hx711_DDR;
volatile uint8_t *hx711_PIN;
volatile uint8_t *hx711_PORT;
uint8_t hx711_datapin;
uint8_t hx711_clkpin;
int32_t* ptr;

/*** Procedure and Function declaration ***/
uint8_t HX711_get_amplify(HX711_Handler* self);
void HX711_reset_readflag(HX711_Handler* self);
uint8_t HX711_read_bit(void);
void HX711_set_amplify(HX711_Handler* self, uint8_t amplify);
uint8_t HX711_query(HX711_Handler* self);
int32_t HX711_read_raw(HX711_Handler* self);
float HX711_raw_average(HX711_Handler* self, uint8_t n);
uint8_t HX711_get_readflag(HX711_Handler* self);
HX711_Calibration* HX711_get_cal(HX711_Handler* self);

/*** Handler ***/
HX711_Handler hx711_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port, uint8_t datapin, uint8_t clkpin)
{
	//LOCAL VARIABLES
	uint8_t tSREG;
	tSREG = STATUS_instanceISTER;
	STATUS_instanceISTER &= ~(1<<7);
	
	HX711_Handler setup_hx711;
	
	//import parameters
	hx711_DDR = ddr;
	hx711_PIN = pin;
	hx711_PORT = port;
	hx711_datapin = datapin;
	hx711_clkpin = clkpin;
	//initialize variables
	*hx711_DDR |= (ONE<<clkpin);
	*hx711_PORT |= (ONE<<datapin);
	setup_hx711.readflag = ZERO;
	setup_hx711.trigger = ZERO;
	setup_hx711.amplify = ONE;
	setup_hx711.ampcount = ONE;
	setup_hx711.bitcount = HX711_ADC_bits;
	setup_hx711.buffer[0] = ZERO;
	setup_hx711.buffer[1] = ZERO;
	setup_hx711.buffer[2] = ZERO;
	setup_hx711.buffer[3] = ZERO;
	setup_hx711.bufferindex = HX711_VECT_SIZE-ONE;
	setup_hx711.raw_reading = ZERO;
	setup_hx711.sum = ZERO;
	setup_hx711.av_n = ZERO;
	setup_hx711.raw_mean = ZERO;
	// offset para mesa usada.
	setup_hx711.cal_data.offset_32 = HX711_Default_50Kg.offset_32; // to subtract B
	setup_hx711.cal_data.offset_64 = HX711_Default_50Kg.offset_64; // to subtract A 64
	setup_hx711.cal_data.offset_128 = HX711_Default_50Kg.offset_128; // to subtract A 128
	// GAIN FACTOR
	setup_hx711.cal_data.divfactor_32 = HX711_Default_50Kg.divfactor_32; // to divide
	setup_hx711.cal_data.divfactor_64 = HX711_Default_50Kg.divfactor_64; // to divide
	setup_hx711.cal_data.divfactor_128 = HX711_Default_50Kg.divfactor_128; // to divide
	setup_hx711.cal_data.status = HX711_Default_50Kg.status;
	HX711_Default = &HX711_Default_50Kg;
	// V-table
	setup_hx711.get_amplify=HX711_get_amplify;
	setup_hx711.read_bit=HX711_read_bit;
	setup_hx711.set_amplify=HX711_set_amplify;
	setup_hx711.query=HX711_query;
	setup_hx711.read_raw=HX711_read_raw;
	setup_hx711.raw_average=HX711_raw_average;
	setup_hx711.get_readflag=HX711_get_readflag;
	setup_hx711.get_cal=HX711_get_cal;
	STATUS_instanceISTER = tSREG;
	
	return setup_hx711;
}

/*** Procedure and Function definition ***/
uint8_t HX711_get_amplify(HX711_Handler* self)
{
	return self->amplify;
}
void HX711_reset_readflag(HX711_Handler* self)
{
	self->readflag=OFF;
}
uint8_t HX711_read_bit(void)
{	
	uint16_t istate;
	*hx711_PORT|=(ONE<<hx711_clkpin);
	// 0.1us minimum
	for(istate=ZERO; istate<HX711_ticks; istate++); // inline delay
	istate=*hx711_PIN & (ONE<<hx711_datapin);
	*hx711_PORT &= ~(ONE<<hx711_clkpin);
	return istate;
}
// Gain selector
// AVDD connected to 5V, channel B gain=32
void HX711_set_amplify(HX711_Handler* self, uint8_t amplify)
{
	switch(amplify){
		case 128:
			self->amplify = ONE; //channel A
			self->ampcount = ONE;
		break;
		case 32:
			self->amplify = 2; //channel B
			self->ampcount = 2; 
		break;
		case 64:
			self->amplify = 3; //channel A
			self->ampcount = 3;
		break;
		default:
			self->amplify = ONE;
			self->ampcount = ONE;
		break;
	}
}
uint8_t HX711_query(HX711_Handler* self)
{
	uint8_t flag=OFF; // one shot
	if(!self->readflag){
		if(!(*hx711_PIN & (ONE << hx711_datapin))){
			self->readflag=ON;
			flag=ON;
		}
	}
	return flag;
}
// Function to be used in the interrupt routine with appropriate cycle period.
int32_t HX711_read_raw(HX711_Handler* self)
{
	uint8_t aindex, bindex;
	int32_t value;
	aindex = self->bufferindex-ONE;
	bindex = self->bitcount-ONE;
	ptr=(int32_t*)self->buffer;
	// Interrupt 24 times sequence plus gain pulses
	if(self->readflag){
		if(self->bitcount){
			if (HX711_read_bit())
				self->buffer[aindex] |= ONE<<(bindex-(aindex*8));
			self->bitcount--;
			if(self->bitcount == 16)
				self->bufferindex=2;
			if(self->bitcount == 8)
				self->bufferindex=ONE;
		}else{
			if(self->ampcount){
				HX711_read_bit();
				self->ampcount--;
			}else{
				value = *(ptr);
				self->raw_reading = value;
				self->bitcount = HX711_ADC_bits;
				self->bufferindex = HX711_VECT_SIZE-ONE;
				self->ampcount = self->amplify;
				self->buffer[0] = ZERO;
				self->buffer[1] = ZERO;
				self->buffer[2] = ZERO;
				self->buffer[3] = ZERO;
				self->trigger = ONE;
				// Reset ready for next query
				HX711_reset_readflag(self);
			}
		}
	}
	return self->raw_reading;
}
float HX711_raw_average(HX711_Handler* self, uint8_t n)
{
	if(self->trigger){
		if(self->av_n < n){
			self->sum += self->raw_reading;
			self->av_n++;
		}else{
			self->av_n = ZERO;
			self->raw_mean = self->sum / n;
			self->sum = ZERO;
			self->sum += self->raw_reading;
			self->av_n++;
		}
		self->trigger = ZERO;
	}
	return self->raw_mean;
}
uint8_t HX711_get_readflag(HX711_Handler* self)
{
	return self->readflag;
}
HX711_Calibration* HX711_get_cal(HX711_Handler* self)
{
	return &(self->cal_data);
}

/*** EOF ***/


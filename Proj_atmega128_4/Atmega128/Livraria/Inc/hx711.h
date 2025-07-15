/************************************************************************
	HX711
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:     08032021_start
************************************************************************/
#ifndef _HX711_H_
	#define _HX711_H_

/*** Library ***/
#include <inttypes.h>

/*** Constant & Macro ***/
#ifndef STATUS_instanceISTER
	#define STATUS_instanceISTER SREG
#endif

#define ZERO 0
#define OFF 0x00
#define ONE 1
#define ON 0xFF
#define HX711_ticks 36 // fine tunned to 36
#define HX711_ADC_bits 24
#define HX711_VECT_SIZE 4

/*** Parameter ***/
typedef struct{
	int32_t offset_32; // ZERO set point A
	int32_t offset_64; // ZERO set point B 64
	int32_t offset_128; // ZERO set point B 128
	uint8_t divfactor_32; // interval A
	uint8_t divfactor_64; // interval B
	uint8_t divfactor_128; // interval B
	uint8_t status;
}HX711_calibration;

HX711_calibration* HX711_Default;

/*** Handler ***/
struct model_hx711{
	volatile uint8_t readflag; // indicate start of bit shifting
	uint8_t trigger; // pickup signal
	uint8_t amplify; // number of end clock cycles
	uint8_t ampcount; // count down final amplify pulses
	uint8_t bitcount; // count down 24 bit
	uint8_t buffer[4]; // reading buffer
	uint8_t bufferindex; // buffer index
	int32_t raw_reading; // reading to be published
	int32_t sum;
	uint8_t av_n;
	float raw_mean;
	HX711_calibration cal_data;
	// V-table
	uint8_t (*get_amplify)(struct model_hx711* self);
	uint8_t (*read_bit)(void);
	void (*set_amplify)(struct model_hx711* self, uint8_t amplify);
	uint8_t (*query)(struct model_hx711* self);
	int32_t (*read_raw)(struct model_hx711* self);
	float (*raw_average)(struct model_hx711* self, uint8_t n);
	uint8_t (*get_readflag)(struct model_hx711* self);
	HX711_calibration* (*get_cal)(struct model_hx711* self);
};

typedef struct model_hx711 HX711;

HX711 hx711_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port, uint8_t datapin, uint8_t clkpin);

#endif
/*** EOF ***/


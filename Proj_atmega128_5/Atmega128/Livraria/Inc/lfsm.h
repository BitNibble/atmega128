/************************************************************************
	LFSM
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmel
Date:     25102020
************************************************************************/
#ifndef _LFSM_H_
	#define _LFSM_H_

/*** Library ***/
#include "atmegaeeprom.h"
#include <inttypes.h>

/*** Constant & Macro ***/
#define NPAGES 255 // 1 to 254 possible pages
#define BYTEL 0
#define BYTEH 65535

/*** Parameter ***/
struct LFSM_Data{
	uint8_t page;
	uint16_t feedback;
	uint8_t inhl;
	uint8_t inlh;
	uint16_t mask;
	uint16_t outhl;
	uint16_t outlh;
};

/*** Handler ***/
struct model_lfsm{
	EEPROM0_Handler* eprom;
	uint16_t sizeeeprom;
	uint8_t sizeblock;
	uint8_t page;
	uint8_t input;
	uint16_t output;
	uint8_t status;
	
	// V-table
	uint8_t (*read)(struct model_lfsm *r, uint8_t input);
	uint8_t (*learn)(struct model_lfsm *r, const uint8_t input, const uint16_t next, const uint16_t mask, const uint8_t page);
	uint16_t (*quant)(struct model_lfsm *r);
	uint8_t (*remove)(struct model_lfsm *r, uint8_t input);
	uint8_t (*premove)(struct model_lfsm *r, uint8_t input, uint8_t page);
	uint8_t (*deleteall)(struct model_lfsm *r);
	uint16_t (*getoutput)(struct model_lfsm *r);
	uint8_t (*getstatus)(struct model_lfsm *r);
	void (*setoutput)(struct model_lfsm *r,uint16_t output);
	uint8_t (*getpage)(struct model_lfsm *r);
	void (*setpage)(struct model_lfsm *r,uint8_t page);
};

typedef struct model_lfsm LFSM_Handler;

LFSM_Handler lfsm_enable(EEPROM0_Handler* eeprom, const uint16_t sizeeeprom);

#endif
/*** EOF ***/


/************************************************************************
	ROTENC
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Rotary encoder Potentiometer
Date:     25102020
************************************************************************/
#ifndef _ROTENC_H_
	#define _ROTENC_H_

/*** Library ***/
#include <inttypes.h>

/*** Parameter ***/
typedef struct{
	uint8_t PinChnA;
	uint8_t PinChnB;
	uint8_t pchn;
	uint8_t chn;
	uint16_t num;
}ROTENC_Parameter;

/*** Handler ***/
typedef struct{
	ROTENC_Parameter par;
	
	// V-table
	ROTENC_Parameter (*rte)(ROTENC_Parameter* par, uint8_t data);
}ROTENC_Handler;

ROTENC_Handler rotenc_enable(uint8_t ChnApin, uint8_t ChnBpin);

#endif
/*** EOF ***/


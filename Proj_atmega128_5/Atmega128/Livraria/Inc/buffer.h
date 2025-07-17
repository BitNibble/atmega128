/******************************************************************************
	BUFFER
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     17072025
*******************************************************************************/
#ifndef _BUFFER_H_
	#define _BUFFER_H_

/*** Library ***/
#include <stdint.h>
#include <inttypes.h>

/*** Constant & Macro ***/
#define BUFFvar char

/*** Parameter ***/
typedef struct{
	BUFFvar* orig;
	BUFFvar* head;
	BUFFvar* end;
}BUFF_Parameter;

/*** Handler ***/
typedef struct{
	BUFF_Parameter par;
	
	// V-table
	void (*push)(BUFF_Parameter* par, BUFFvar data);
	BUFFvar* (*raw)(BUFF_Parameter* par);
	void (*flush)(BUFF_Parameter* par);
}BUFF_Handler;

BUFF_Handler buff_enable(uint16_t size_buff, BUFFvar* buff);

#endif
/*** EOF ***/


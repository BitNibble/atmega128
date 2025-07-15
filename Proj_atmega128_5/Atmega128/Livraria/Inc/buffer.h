/******************************************************************************
	BUFFER
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     17112022
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
}buffer_parameter;

/*** Handler ***/
typedef struct{
	buffer_parameter par;
	void (*push)(buffer_parameter* par, BUFFvar data);
	BUFFvar* (*raw)(buffer_parameter* par);
	void (*flush)(buffer_parameter* par);
}BUFF;

BUFF buff_enable(uint16_t size_buff, BUFFvar* buff);

#endif
/*** EOF ***/


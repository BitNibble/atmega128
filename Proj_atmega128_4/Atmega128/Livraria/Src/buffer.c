/******************************************************************************
	BUFFER
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     17072025
******************************************************************************/
/*** Library ***/
#include "buffer.h"
#include <stdio.h>
#include <string.h>

/*** Procedure and Function declaration ***/
void BUFF_push(BUFF_Parameter* par, BUFFvar data);
BUFFvar* BUFF_raw(BUFF_Parameter* par);
void BUFF_flush(BUFF_Parameter* par);

/*** Handler ***/
BUFF_Handler buff_enable( uint16_t size_buff, BUFFvar* buff )
{
	// OBJECT STRUCT
	BUFF_Handler setup_buffer;
	// inic VAR
	setup_buffer.par.orig = buff;
	setup_buffer.par.head = buff;
	setup_buffer.par.end = buff + ( size_buff - 1 ); // generic
	// V-table
	setup_buffer.push = BUFF_push;
	setup_buffer.raw = BUFF_raw;
	setup_buffer.flush = BUFF_flush;
	
	return setup_buffer; // return copy
}

/*** Procedure and Function definition ***/
void BUFF_push( BUFF_Parameter* par, BUFFvar data ){
	BUFFvar* head; BUFFvar* next;
	head = par->head;
	if( par->end - head ){
		next = head + 1;
	}else{
		head = par->orig;
		next = head + 1;
	}
	*head = data;
	par->head = next;
}

BUFFvar* BUFF_raw( BUFF_Parameter* par ){
	*par->head = 0;
	return par->orig;
}

void BUFF_flush( BUFF_Parameter* par ){
	BUFFvar* head;
	head = par->orig;
	par->head = head;
	*head = 0;
}

/*** EOF ***/


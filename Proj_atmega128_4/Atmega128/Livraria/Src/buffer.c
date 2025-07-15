/******************************************************************************
	BUFFER
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     17112022
******************************************************************************/
/*** Library ***/
#include "buffer.h"
#include <stdio.h>
#include <string.h>

/*** Procedure and Function declaration ***/
void BUFF_push(buffer_parameter* par, BUFFvar data);
BUFFvar* BUFF_raw(buffer_parameter* par);
void BUFF_flush(buffer_parameter* par);

/*** Handler ***/
BUFF buff_enable( uint16_t size_buff, BUFFvar* buff )
{
	// OBJECT STRUCT
	BUFF setup_buffer;
	// inic VAR
	setup_buffer.par.orig = buff;
	setup_buffer.par.head = buff;
	setup_buffer.par.end = buff + ( size_buff ); // generic
	// V-table
	setup_buffer.push = BUFF_push;
	setup_buffer.raw = BUFF_raw;
	setup_buffer.flush = BUFF_flush;
	
	return setup_buffer; // return copy
}

/*** Procedure and Function definition ***/
void BUFF_push( buffer_parameter* par, BUFFvar data ){
	BUFFvar* head; BUFFvar* next;
	head = par->head;
	if(data){
		if( head == par->end ){
			head = par->orig;
			next = head + 1;
		}else{
			next = head + 1;
		}
			*head = data;
			*next = 0;
			par->head = next;
	}
}

BUFFvar* BUFF_raw( buffer_parameter* par ){
		return par->orig;
}

void BUFF_flush( buffer_parameter* par ){
	BUFFvar* head;
	head = par->orig;
	par->head = head;
	*head = 0;
}

/***EOF***/


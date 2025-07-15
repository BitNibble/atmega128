/************************************************************************
	EXPLODE
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     16032021
************************************************************************/
#ifndef _EXPLODE_H_
	#define _EXPLODE_H_

/*** Library ***/
#include <inttypes.h>

/*** Constant & Macro ***/
//#define STM32F4
#if defined (STM32F4)
	#define IO_var uint32_t
#else
	#define IO_var uint8_t
#endif

/*** Parameter ***/
typedef struct {
	IO_var XI;
	IO_var XF;
	IO_var HH;
	IO_var LL;
	IO_var LH;
	IO_var HL;
}explode_parameter;

/*** Handler ***/
typedef struct {
	explode_parameter par;
	
	// V-table
	void (*update)(explode_parameter* par, IO_var x);
}EXPLODE;

EXPLODE explode_enable(void);

#endif
/*** EOF ***/

/******
1º Sequence
2º Scope
	- Library Scope
	- File Scope
	- Function Scope
	- Precedence Scope
3º Pointer and Variable
4º Casting
******/


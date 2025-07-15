/************************************************************************
Title:    watch.h
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Date:     03072025
************************************************************************/
#ifndef _WATCH_H_
	#define _WATCH_H_

/*** Library ***/
#include <stdint.h>
#include <inttypes.h>

/*** Parameter ***/
typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	volatile uint32_t seconds;
} WATCH_TIME;

/*** Handler ***/
typedef struct {
	// V-table
	void (*preset)(uint8_t hour, uint8_t minute, uint8_t second);
	uint8_t (*start_delay)(uint8_t n_delay, uint32_t seconds);
	void (*increment)(void);
	char* (*show)(void);
} WATCH;

WATCH watch_enable(void);

/*** Procedure and Funtion declaration ***/
extern void WATCH_increment(void);
extern void WATCH_decrement(void);

#endif
/*** EOF ***/


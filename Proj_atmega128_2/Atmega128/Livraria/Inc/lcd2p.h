/************************************************************************
	LCD2P
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     12072025
************************************************************************/
#ifndef _LCD2P_H_
	#define _LCD2P_H_

/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Library ***/
#include <stdint.h>
#include <inttypes.h>

/*** Constant & Macro ***/
#define LCD_WIRING_3
#if defined(LCD_WIRING_1)
	#define LCD02P_RS 0
	#define LCD02P_RW 1
	#define LCD02P_EN 2
	#define LCD02P_NC 3
	#define LCD02P_DB4 0
	#define LCD02P_DB5 1
	#define LCD02P_DB6 2
	#define LCD02P_DB7 3
#elif defined(LCD_WIRING_2)
	#define LCD02P_RS 0
	#define LCD02P_RW 1
	#define LCD02P_EN 2
	#define LCD02P_NC 3
	#define LCD02P_DB4 4
	#define LCD02P_DB5 5
	#define LCD02P_DB6 6
	#define LCD02P_DB7 7
#elif defined(LCD_WIRING_3)
	#define LCD02P_RS 7
	#define LCD02P_RW 6
	#define LCD02P_EN 5
	#define LCD02P_NC 4
	#define LCD02P_DB4 3
	#define LCD02P_DB5 2
	#define LCD02P_DB6 1
	#define LCD02P_DB7 0
#endif

/*** Handler ***/
typedef struct{
	// V-table
	void (*write)(char c, unsigned short D_I);
	char (*read)(unsigned short D_I);
	uint8_t (*BF)(void);
	void (*putch)(char c);
	char (*getch)(void);
	void (*string)(const char *s); // RAW
	void (*string_size)(const char* s, uint8_t size); // RAW
	void (*hspace)(uint8_t n);
	void (*clear)(void);
	void (*gotoxy)(unsigned int y, unsigned int x);
	void (*reboot)(void);
	int (*printf)(const char *fmt, ...);
}LCD02P_Handler;

void lcd02p_enable(volatile uint8_t *cmdddr, volatile uint8_t *cmdpin, volatile uint8_t *cmdport, volatile uint8_t *dataddr, volatile uint8_t *datapin, volatile uint8_t *dataport);
LCD02P_Handler* lcd02p(void);

#endif
/*** EOF ***/


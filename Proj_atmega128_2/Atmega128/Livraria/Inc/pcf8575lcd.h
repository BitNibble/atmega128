/************************************************************************
	PCF8575LCD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     11042024
************************************************************************/
#ifndef _PCF8575LCD_H_
	#define _PCF8575LCD_H_

/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Library ***/
#include <inttypes.h>

/*** Constant & Macro ***/
// ASIGN PORT PINS TO LCD (can be setup in any way)
#define PCF_DB0 0
#define PCF_DB1 1
#define PCF_DB2 2
#define PCF_DB3 3
#define PCF_DB4 4
#define PCF_DB5 5
#define PCF_DB6 6
#define PCF_DB7 7
#define PCF_RS 8
#define PCF_RW 9
#define PCF_EN 10
#define PCF_NC 11
/***************/

/*** Handler ***/
typedef struct{
	// V-table
	void (*write)(char c, unsigned short D_I);
	char (*read)(unsigned short D_I);
	void (*BF)(void);
	void (*putch)(char c);
	char (*getch)(void);
	void (*string)(const char *s); // RAW
	void (*string_size)(const char* s, uint8_t size); // RAW
	void (*hspace)(uint8_t n);
	void (*clear)(void);
	void (*gotoxy)(unsigned int y, unsigned int x);
	void (*reboot)(void);
}PCF8575_LCD0_Handler;

PCF8575_LCD0_Handler pcf8575_lcd0_enable( uint8_t pcf8575_id, uint8_t twi_prescaler );
PCF8575_LCD0_Handler* pcf8575_lcd0(void);

#endif
/*** EOF ***/


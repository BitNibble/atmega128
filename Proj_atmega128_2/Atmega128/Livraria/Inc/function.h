/************************************************************************
	FUNCTION
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:   07012024
************************************************************************/
#ifndef _FUNCTION_H_
	#define _FUNCTION_H_

/*** Library ***/
/*******pc use*******
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
********************/
#include <stdint.h>
#include <inttypes.h>

/*** Constant & Macro ***/
#define FUNCSTRSIZE 31

/*** Handler ***/
typedef struct
{
	// V-table
	int (*stringlength)(const char string[]);
	void (*reverse)(char s[]);
	unsigned int (*mayia)(unsigned int xi, unsigned int xf, uint8_t nbits);
	void (*swap)(long *px, long *py);
	void (*copy)(char to[], char from[]);
	void (*squeeze)(char s[], int c);
	void (*shellsort)(int v[], int n);
	char* (*i16toa)(int16_t n);
	char* (*ui16toa)(uint16_t n);
	char* (*i32toa)(int32_t n);
	char* (*ui32toa)(uint32_t n);
	int (*trim)(char s[]);
	int (*pmax)(int a1, int a2);
	int (*gcd)(int u, int v);
	int (*strToInt)(const char string[]);
	int (*twocomptoint8bit)(int twoscomp);
	int (*twocomptoint10bit)(int twoscomp);
	int (*twocomptointnbit)(int twoscomp, uint8_t nbits);
	char (*dec2bcd)(char num);
	char (*bcd2dec)(char num);
	char* (*resizestr)(char *string, int size);
	long (*trimmer)(long x, long in_min, long in_max, long out_min, long out_max);
	unsigned char (*bcd2bin)(unsigned char val);
	unsigned char (*bin2bcd)(unsigned val);
	long (*gcd1)(long a, long b);
	char* (*print_binary)(unsigned int n_bits, unsigned int number);
	char* (*ftoa)(double num, uint8_t afterpoint);
	char* (*dectohex)(int32_t num);
	uint16_t (*SwapByte)(uint16_t num);
	char* (*print)( const char *format, ... );
	void (*strtovec)(char* pos, const char* str);
	/**********************pc use**********************
	char* (*fltos)(FILE* stream);
	char* (*ftos)(FILE* stream);
	int (*strtotok)(char* line,char* token[], const char* parser);
	char* (*putstr)(char* str);
	int (*getnum)(char* x);
	unsigned int (*getnumv2)(char* x);
	int (*readint)(int nmin, int nmax);
	**************************************************/
}FUNC;

FUNC* func(void);

#endif
/*** EOF ***/


/*************************************************************************
	FUNCTION
Author:   <sergio.salazar.santos@gmail.com> 
License:  GNU General Public License
Hardware: all
Date :    07012024
*************************************************************************/
/*** File Library ***/
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

static char function_str[FUNC_STRSIZE + 1] = {0};

/*** Procedure and Function declaration ***/
int StringLength (const char string[]);
void Reverse(char s[]);
uint8_t function_intinvstr(uint32_t num, uint8_t index);
uint8_t function_fPartStr(double num, uint8_t index, uint8_t afterpoint);
void function__swap(long *px, long *py);
/***/
uint8_t  bintobcd(uint8_t bin);
uint8_t leap_year_check(uint16_t year);
unsigned int function_mayia(unsigned int xi, unsigned int xf, uint8_t nbits);
void function_copy(char to[], char from[]);
void function_squeeze(char s[], int c);
void function_shellsort(int v[], int n);
char* function_i16toa(int16_t n);
char* function_ui16toa(uint16_t n);
char* function_i32toa(int32_t n);
char* function_ui32toa(uint32_t n);
int function_trim(char s[]);
int function_pmax(int a1, int a2);
int function_gcd (int u, int v);
int function_strToInt (const char string[]);
int function_twocomptoint8bit(int twoscomp);
int function_twocomptoint10bit(int twoscomp);
int function_twocomptointnbit(int twoscomp, uint8_t nbits);
char function_dec2bcd(char num);
char function_bcd2dec(char num);
char* function_resizestr(char *string, int size);
long function_trimmer(long x, long in_min, long in_max, long out_min, long out_max);
unsigned char function_bcd2bin(unsigned char val);
unsigned char function_bin2bcd(unsigned val);
long function_gcd1(long a, long b);
char* function_print_binary(unsigned int n_bits, unsigned int number);
void function_reverse(char* str, int len);
char* function_ftoa(double num, uint8_t afterpoint);
char* function_dectohex(int32_t num);
uint16_t function_SwapByte(uint16_t num);
char* function_print(const char *format, ... );
void function_strtovec(char* pos, const char* str);
int function_strtotok(char* line, char* token[], const char* parser);
void function_rmcrnl(char* str);
/************ pc use ************
char* function_fltos(FILE* stream);
char* function_ftos(FILE* stream);
int function_strtotok(char* line,char* token[], const char* parser);
char* function_putstr(char* str);
int function_getnum(char* x);
unsigned int function_getnumv2(char* x);
int function_readint(int nmin, int nmax);
******************************/

/*** Internal State ***/
static FUNC_Handler setup_func = {
	// V-table
	.stringlength = StringLength,
	.reverse = Reverse,
	.mayia = function_mayia,
	.swap = function__swap,
	.copy = function_copy,
	.squeeze = function_squeeze,
	.shellsort = function_shellsort,
	.i16toa = function_i16toa,
	.ui16toa = function_ui16toa,
	.i32toa = function_i32toa,
	.ui32toa = function_ui32toa,
	.trim = function_trim,
	.pmax = function_pmax,
	.gcd = function_gcd,
	.strToInt = function_strToInt,
	.twocomptoint8bit = function_twocomptoint8bit,
	.twocomptoint10bit = function_twocomptoint10bit,
	.twocomptointnbit = function_twocomptointnbit,
	.dec2bcd = function_dec2bcd,
	.bcd2dec = function_bcd2dec,
	.resizestr = function_resizestr,
	.trimmer = function_trimmer,
	.bcd2bin = function_bcd2bin,
	.bin2bcd = function_bin2bcd,
	.gcd1 = function_gcd1,
	.print_binary = function_print_binary,
	.ftoa = function_ftoa,
	.dectohex = function_dectohex,
	.SwapByte = function_SwapByte,
	.print = function_print,
	.strtovec = function_strtovec,
	.strtotok = function_strtotok,
	.rmcrnl = function_rmcrnl
	/***********pc use************
	.fltos = function_fltos,
	.ftos = function_ftos,
	.strtotok = function_strtotok,
	.putstr = function_putstr,
	.getnum = function_getnum,
	.getnumv2 = function_getnumv2,
	.readint = function_readint
	*****************************/
};

/*** Handler ***/
FUNC_Handler* func(void){ return &setup_func; }

/*** Procedure and Function definition ***/
unsigned int function_mayia(unsigned int xi, unsigned int xf, uint8_t nbits)
{
	unsigned int diff;
	unsigned int trans;
	unsigned int mask = (unsigned int)((1 << nbits) - 1);
	xi &= mask; xf &= mask; diff = xf ^ xi; trans = diff & xf;
	return (trans << nbits) | diff;
}
// interchange *px and *py
void function__swap(long *px, long *py)
{
	long temp = *px; *px = *py; *py = temp;
}
// copy: copy 'from' into 'to'; assume to is big enough
void function_copy(char to[], char from[])
{
	int i = 0;
	while ((to[i] = from[i]) != '\0') ++i;
}
// squeeze: delete all c from s
void function_squeeze(char s[], int c)
{
	int i, j;
	for (i = 0, j = 0; (s[i] != '\0'); i++){
		if (s[i] != c) s[j++] = s[i];
	}
	s[j] = '\0';
}
// shellsort: sort v[0]...v[n-1] into increasing order
void function_shellsort(int v[], int n)
{
	int gap, i, j, temp;
	for (gap = n / 2; gap > 0; gap /= 2)
		for (i = gap; i < n; i++)
			for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap){
				temp = v[j]; v[j] = v[j + gap]; v[j + gap] = temp;
			}
}
// ui32toa: convert n to characters in s
char* function_ui32toa(uint32_t n)
{
	uint8_t i;
	for(i = 0, function_str[i++] = n % 10 + '0'; (n /= 10) > 0; function_str[i++] = n % 10 + '0');
	function_str[i] = '\0';
	Reverse(function_str);
	return function_str;
}
// i32toa: convert n to characters in s
char* function_i32toa(int32_t n)
{
	uint8_t i;
	int32_t sign;
	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		function_str[i++] = (char) (n % 10 + '0');
	}while ((n /= 10) > 0);
	if (sign < 0) function_str[i++] = '-';
	function_str[i] = '\0';
	Reverse(function_str);
	return function_str;
}
// i16toa: convert n to characters in s
char* function_i16toa(int16_t n)
{
	uint8_t i;
	int16_t sign;
	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		function_str[i++] = (char) (n % 10 + '0');
	}while ((n /= 10) > 0);
	if (sign < 0) function_str[i++] = '-';
	function_str[i] = '\0';
	Reverse(function_str);
	return function_str;
}
// ui16toa: convert n to characters in s
char* function_ui16toa(uint16_t n)
{
	uint8_t i;
	for(i = 0, function_str[i++] = n % 10 + '0'; (n /= 10) > 0; function_str[i++] = n % 10 + '0');
	function_str[i] = '\0';
	Reverse(function_str);
	return function_str;
}
// trim: remove trailing blanks, tabs, newlines
int function_trim(char s[])
{
	int n;
	for (n = StringLength(s) - 1; n >= 0; n--)
		if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n') break;
	s[n + 1] = '\0';
	return n;
}
// larger number of two
int function_pmax(int a1, int a2)
{
	int biggest;
	if(a1 > a2){ biggest = a1; }else{ biggest = a2; }
	return biggest;
}
// common divisor
int function_gcd (int u, int v)
{
	int temp;
	while ( v != 0 ) {
		temp = u % v; u = v; v = temp;
	}
	return u;
}
long function_gcd_v2(long a, long b)
{
	long r;
	if (a < b) function__swap(&a, &b);
	if (!b){
		while ((r = a % b) != 0) {
			a = b; b = r;
		}
	}
	return b;
}
// Function to convert a string to an integer
int function_strToInt (const char string[])
{
	int i, intValue, result = 0;
	for ( i = 0; string[i] >= '0' && string[i] <= '9'; ++i ){
		intValue = string[i] - '0';
		result = result * 10 + intValue;
	}
	return result;
}
// Two's Complement function
int function_twocomptoint8bit(int twoscomp){
  
  int value;
	// Let's see if the byte is negative
  if (twoscomp & 0x80){
    // Invert
    twoscomp = ~twoscomp + 1;
		twoscomp = (twoscomp & 0xFF);
    // Cast as int and multiply by negative one
    value = (int)(twoscomp) * (-1);
    return value;
  }else{
    // Byte is non-negative, therefore convert to decimal and display
    // Make sure we are never over 1279
    twoscomp = (twoscomp & 0x7F);
    // Cast as int and return
    value = (int)(twoscomp);
    return value;
  }
}
// Two's Complement function, shifts 10 bit binary to signed integers (-512 to 512)
int function_twocomptoint10bit(int twoscomp){
	int value;
  // Let's see if the byte is negative
  if (twoscomp & 0x200){
    // Invert
    twoscomp = ~twoscomp + 1;
    twoscomp = (twoscomp & 0x3FF);
    // Cast as int and multiply by negative one
    value = (int)(twoscomp) * (-1);
    return value;
  }else{
    // Serial.println("We entered the positive loop");
    // Byte is non-negative, therefore convert to decimal and display
    twoscomp = (twoscomp & 0x1FF);
    // Cast as int and return
    // Serial.println(twoscomp);
    value = (int)(twoscomp);
    return value;
  }
}
// Two's Complement function, nbits
int function_twocomptointnbit(int twoscomp, uint8_t nbits){
  unsigned int signmask;
  unsigned int mask;
  signmask = (1 << (nbits - 1));
  mask = signmask - 1;
  // Let's see if the number is negative
  if ((unsigned int) twoscomp & signmask){
	twoscomp &= mask;
    twoscomp -= signmask;
  }else{
	  twoscomp &= mask;
  }
  return twoscomp;
}
// Convert Decimal to Binary Coded Decimal (BCD)
char function_dec2bcd(char num)
{
	return ((num / 10 * 16) + (num % 10));
}
// Convert Binary Coded Decimal (BCD) to Decimal
char function_bcd2dec(char num)
{
	return ((num / 16 * 10) + (num % 16));
}
// resizestr
char* function_resizestr(char *string, int size)
{
	int i;
	function_str[size] = '\0';
	for(i = 0; i < size; i++){
		if(*(string + i) == '\0'){
			for(; i < size; i++){ function_str[i] = ' '; }
			break;
		}
		function_str[i] = *(string + i);
	}
	return function_str;
}
// trimmer
long function_trimmer(long x, long in_min, long in_max, long out_min, long out_max)
// same as arduino map function.
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
// Function to count the number of characters in a string
int StringLength (const char string[])
{
	int count;
	for ( count = 0; string[count] != '\0'; count++ ) ;
	return count;
}
// reverse: reverse string s in place
void Reverse(char s[])
{
	char c; int i, j;
	for (i = 0, j = StringLength(s) - 1; i < j; i++, j--){
		c = s[i]; s[i] = s[j]; s[j] = c;
	}
}
// bcd2bin
unsigned char function_bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}
// bin2bcd
unsigned char function_bin2bcd(unsigned int val)
{
	return (unsigned char)(((val / 10) << 4) + val % 10);
}
// gcd1
long function_gcd1(long a, long b)
{
	long r;
	if (a < b) function__swap(&a, &b);
	if (!b){
		while ((r = a % b) != 0) {
			a = b; b = r;
		}
	}
	return b;
}
// printbinary
char* function_print_binary(unsigned int n_bits, unsigned int number)
{
	unsigned int i, c;
	for(i = (1 << (n_bits - 1)), c = 0; i; i >>= 1, c++)
		(number & i) ? (function_str[c] = '1') : (function_str[c] = '0');
	function_str[c] = '\0';
	return function_str;
}
// leapyearcheck
uint8_t leap_year_check(uint16_t year){
	uint8_t i;
	if (!(year % 400)) i = 1;
  	else if (!(year % 100)) i = 0;
  	else if (!(year % 4) ) i = 1;
  	else i = 0;
	return i;
}
// bintobcd
uint8_t bintobcd(uint8_t bin)
{
	return (uint8_t)((((bin) / 10) << 4) + ((bin) % 10));
}
// intinvstr
uint8_t function_intinvstr(uint32_t num, uint8_t index)
{
	for(function_str[index++] = (char)((num % 10) + '0'); (num /= 10) > 0 ; function_str[index++] = (char)((num % 10) + '0'));
	function_str[index] = '\0'; return index;
}
// fPartStr
uint8_t function_fPartStr(double num, uint8_t index, uint8_t afterpoint)
{
	for( num *= 10; afterpoint ; function_str[index++] = (uint8_t)(num + '0'), num -= (uint8_t)num, num *= 10, afterpoint--);
	function_str[index] = '\0'; return index;
}
// ftoa
char* function_ftoa(double num, uint8_t afterpoint)
{
	uint32_t ipart; double n, fpart; uint8_t k = 0; int8_t sign;
	if (num < 0){ n = -num; sign = -1;}else{n = num; sign = 1;}
	ipart = (uint32_t) n; fpart = n - (double)ipart;
	k = function_intinvstr(ipart, 0); if (sign < 0) function_str[k++] = '-'; function_str[k] = '\0'; Reverse(function_str);
	function_str[k++] = '.';
	function_fPartStr(fpart, k, afterpoint);
	return function_str;
}
// dectohex
char* function_dectohex(int32_t num)
{
	int32_t remainder; uint8_t j;
	for(j = 0, function_str[j] = '\0'; num; function_str[j] = '\0', num = num / 16){
		remainder = num % 16;
		if (remainder < 10) function_str[j++] = (char) (48 + remainder);
		else function_str[j++] = (char) (55 + remainder);
	}
	Reverse(function_str);
	return function_str;
}
// swapbyte
uint16_t function_SwapByte(uint16_t num)
{
	uint16_t tp;
	tp = (uint16_t)(num << 8);
	return (num >> 8) | tp;
}
char* function_print_v1( char* str, uint8_t size_str, const char* format, ... )
{
	va_list aptr; int ret;
	va_start(aptr, format);
	ret = vsnprintf( str, size_str, (const char*) format, aptr );
	//ret = vsnprintf( ptr, size_str, format, aptr );
	va_end(aptr);
	if(ret < 0){ return NULL; }else return str;
}
// print
char* function_print( const char* format, ... )
{
	va_list aptr; int ret;
	va_start(aptr, format);
	ret = vsnprintf( function_str, FUNC_STRSIZE, (const char*) format, aptr );
	// ret = vsnprintf( ptr, function_STRSIZE, format, aptr );
	va_end(aptr);
	if(ret < 0){ return NULL; }else return function_str;
}
// strtovec
void function_strtovec(char* pos, const char* str){
	int i;
	for(i=0; str[i]; *(pos + i) = str[i], i++);
}

int function_strtotok(char* line, char* token[], const char* parser)
{
    int i;
    for (i = 0, token[i] = strtok(line, parser); token[i]; i++, token[i] = strtok(NULL, parser));
    return i;
}

void function_rmcrnl(char* str)
{
	if (!str) return; 
    int len = (int)strlen(str) - 1;
	if(len > 1){
		for (int stop = len - 2; len > stop; len--) {
			if (*(str + len) == '\r' || *(str + len) == '\n') {
				*(str + len) = '\0';
			}
		}
	}else{*str = '\0';}
}
/********************************************************************
int gcd( int a, int b ) {
    int result ;
    // Compute Greatest Common Divisor using Euclid's Algorithm
    __asm__ __volatile__ ( "movl %1, %%eax;"
                          "movl %2, %%ebx;"
                          "CONTD: cmpl $0, %%ebx;"
                          "je DONE;"
                          "xorl %%edx, %%edx;"
                          "idivl %%ebx;"
                          "movl %%ebx, %%eax;"
                          "movl %%edx, %%ebx;"
                          "jmp CONTD;"
                          "DONE: movl %%eax, %0;" : "=g" (result) : "g" (a), "g" (b)
    );
    return result ;
}
//
float sinx( float degree ) {
    float result, two_right_angles = 180.0f ;
    // Convert angle from degrees to radians and then calculate sin value
    __asm__ __volatile__ ( "fld %1;"
                            "fld %2;"
                            "fldpi;"
                            "fmul;"
                            "fdiv;"
                            "fsin;"
                            "fstp %0;" : "=g" (result) : 
				"g"(two_right_angles), "g" (degree)
    ) ;
    return result ;
}
//
float cosx( float degree ) {
    float result, two_right_angles = 180.0f, radians ;
    // Convert angle from degrees to radians and then calculate cos value
    __asm__ __volatile__ ( "fld %1;"
                            "fld %2;"
                            "fldpi;"
                            "fmul;"
                            "fdiv;"
                            "fstp %0;" : "=g" (radians) : 
				"g"(two_right_angles), "g" (degree)
    ) ;
    __asm__ __volatile__ ( "fld %1;"
                            "fcos;"
                            "fstp %0;" : "=g" (result) : "g" (radians)
    ) ;
    return result ;
}
//
float square_root( float val ) {
    float result ;
    __asm__ __volatile__ ( "fld %1;"
                            "fsqrt;"
                            "fstp %0;" : "=g" (result) : "g" (val)
    ) ;
    return result ;
}
*/
/***pc use***
char* function_fltos(FILE* stream)
{
	int i, block, NBytes;
	char caracter;
	char* value = NULL;
	for(i=0, block=4, NBytes=0; (caracter = getc(stream)) != EOF; i++){
		if(i == NBytes){
			NBytes += block;
			value = (char*)realloc(value, NBytes * sizeof(char));
			if(value == NULL){
				perror("fltos at calloc");
				break;
			}
		}
		*(value + i) = caracter;
		if(caracter == '\n'){
			*(value + i) = '\0';
			break;
		}
	}
	return value;
}
char* function_ftos(FILE* stream)
{
	int i, block, NBytes;
	char caracter;
	char* value = NULL;
	for(i = 0, block = 8, NBytes = 0; (caracter = getc(stream)) != EOF; i++){
		if(i == NBytes){
			NBytes += block;
			value = (char*)realloc(value, NBytes * sizeof(char));
			if(value == NULL){
				perror("ftos at calloc");
				break;
			}
		}
		*(value + i) = caracter;
	}
	return value;
}
int function_strtotok(char* line, char* token[], const char* parser)
{
	int i;
	char *str;
	str=(char*)calloc(strlen(line),sizeof(char));
	for (i = 0, strcpy(str, line); ; i++, str = NULL) {
		token[i] = strtok(str, parser);
		if (token[i] == NULL)
			break;
		printf("%d: %s\n", i, token[i]);
	}
	free(str);
	return i;
}
char* function_putstr(char* str)
{
	int i; char* ptr;
	ptr = (char*)calloc(strlen(str), sizeof(char));
	if(ptr == NULL){
		perror("NULL!\n");
		return NULL;
	}
	for(i = 0; (ptr[i] = str[i]); i++){
		if(ptr[i] == '\0')
			break;
	}
	return (ptr);
}
int function_getnum(char* x)
{
	int num;
	if(!sscanf(x, "%d", &num))
		num = 0;
	return num;
}
unsigned int function_getnumv2(char* x)
{
	unsigned int RETURN;
	unsigned int value;
	unsigned int n;
	errno = 0;
	n = sscanf(x, "%d", &value);
	if(n == 1){
		RETURN = value;
	}else if(errno != 0){
		perror("getnum at sscanf");
		RETURN = 0;
	}else{
		RETURN = 0;
	}
	return RETURN;
}
int function_readint(int nmin, int nmax)
{
	int num;
	int flag;
	for(flag = 1; flag; ){
		for( num = 0; !scanf("%d", &num); getchar())
			;
		//printf("num: %d nmin: %d nmax: %d\n",num, nmin, nmax);
		if((num < nmin) || (num > nmax))
			continue;
		flag = 0;
	}
		return num;
}
********************************************************************/

/***EOF***/


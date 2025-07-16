/************************************************************************
Title: testing l293d
Author:    <sergio.salazar.santos@gmail.com>
License:   GNU General Public License
Software:  Atmel Studio 7 (ver 7.0.129)
Hardware:  atmega128 by ETT ET-BASE
	-PORTA LCD 4X20
	-PORTD RTC and UART1->FTDI chip->PC Putty or HC-05 115200kb 8 1 n.
Update:    01/01/2024
Comment:
	-AT+ROLE0\r\n
	-AT+TYPE1\r\n
	-AT+BAUD6\r\n
	-AT+ADVI3\r\n
	-AT+NAMEHome1\r\n
	-AT+PIN916919\r\n
	
	Nice
************************************************************************/
/*** Working Frequency ***/
#define F_CPU 16000000UL
/*** Library ***/
#include "atmega128usart1.h"
#include "function.h"
#include "lcd2p.h"
#include "pcf8563rtc.h"
#include "l293d.h"
#include "pcf8575.h"
#include <string.h>

/*** Constant & Macro ***/
#define TRUE 1
#define ZERO 0
#define Min 500     // 450 PWM servo motor
#define Max 2350    // 2450 PWM servo motor
#define SMIN -200
#define SMAX +200

/*** Variable ***/
PCF8563RTC_Time tm; // time struct RTC
PCF8563RTC_Date dt; // date struct RTC

PCF8563RTC_Handler rtc;
char* uartreceive = NULL; // capture

/**** Handler ****/
int main(void)
{
usart1_enable(38400,8,1,NONE); // UART 103 para 9600 (ESP01), 68 para 14400, 25 para 38400 (HC05), 8 para 115200
lcd02p_enable(&DDRA,&PINA,&PORTA,&DDRA,&PINA,&PORTA); // LCD Display 4X20
rtc = pcf8563rtc_enable( 16 ); // RTC with I2C
L293D_Handler l293 = l293d_enable(&DDRC, &PORTC, 0, 1, 2);
PCF8575_Handler pcf8575 = pcf8575_enable(32,16);

char uartmsg[UART1_RX_BUFFER_SIZE] = {0}; // One shot
char uartmsgprint[UART1_RX_BUFFER_SIZE] = {0}; // triggered

rtc.SetClkOut(1, 2);
l293.enable(&l293.par,0);

while(TRUE){
lcd02p()->reboot();
		
uartreceive = usart1_messageprint( usart1(), uartmsg, uartmsgprint, "\r\n");
tm=rtc.GetTime();
dt=rtc.GetDate();
			
lcd02p()->gotoxy(0,12);
lcd02p()->printf("%02d:%02d:%02d",rtc.bcd2dec(dt.days),rtc.bcd2dec(dt.century_months),rtc.bcd2dec(dt.years));
lcd02p()->gotoxy(1,12);
lcd02p()->printf("%02d:%02d:%02d",rtc.bcd2dec(tm.hours),rtc.bcd2dec(tm.minutes),rtc.bcd2dec(tm.VL_seconds));	
lcd02p()->gotoxy(3,0);
lcd02p()->string_size(uartmsgprint,13);

if(!strcmp("stop\r\n",uartmsgprint)){ l293.dir(&l293.par,0); pcf8575.writehbits(&pcf8575.par,0xFFFF,HIGH);}
if(!strcmp("forward\r\n",uartmsgprint)){ l293.dir(&l293.par,1); pcf8575.writehbits(&pcf8575.par,0xFFFF,LOW); }
if(!strcmp("reverse\r\n",uartmsgprint)){ l293.dir(&l293.par,2); }

}}

/*** EOF ***/


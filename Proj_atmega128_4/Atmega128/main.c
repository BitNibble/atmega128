/************************************************************************
Title: PID_TEST.c
Author:    <sergio.salazar.santos@gmail.com>
License:   GNU General Public License
Software:  Atmel Studio 7 (ver 7.0.129)
Hardware:  Atmega128 by ETT ET-BASE
	-PORTA LCD
	-PORTE Keyboard
	-PF0 Sensor LDR
	-PORTG HC595
Comment:

************************************************************************/
/*** Working Frequency ***/
#define F_CPU 16000000UL

/*** Library ***/
#include "atmega128mapping.h"
#include "function.h"
#include "lcd.h"
#include "pcf8563rtc.h"
#include "keypad.h"
#include "74hc595.h"
#include "znpid.h"
//#include "atmega128uart.h"
#include <util/delay.h>
#include <stdio.h> //sprintf
#include <string.h>

/*** Constant and Macro ***/
#define TRUE 1
#define ZERO 0
#define Min 500     // 450 PWM servo motor
#define Max 2350    // 2450 PWM servo motor
#define repeat 59

/*** Variable ***/
HC595_Handler shift;
ZNPID_Handler pid_1;
ZNPID_Handler pid_2;
uint8_t count=0; // 1Hz
uint8_t increment=0; // 1Hz
char* ptr=NULL; // pointing to analog reading string
uint16_t adcvalue; // analog reading
float pid_out_1;
float pid_out_2;
int32_t tmp;

/*** Procedure and Function declaration ***/
void timer0_comp_vect(void);
void PORTINIT();

/**** HANDLER ****/
int main(void)
{
PORTINIT(); // Inic Ports
/*** Handler ***/
lcd0_enable(&DDRA,&PINA,&PORTA); // LCD Display 4X20
KEYPAD_Handler keypad = keypad_enable(&DDRE,&PINE,&PORTE); // Keyboard
adc_enable(1, 128, 1, 0); // Channel 0 for Position
tc0_enable(2,2); // 1Hz to HC595
tc1_enable(9,0); // PWM Positioning
shift = hc595_enable(&DDRG,&PORTG,2,0,1);
pid_1 = znpid_enable();
pid_2 = znpid_enable();
/******/
char Menu='1'; // Main menu selector
char str[6]="0"; // analog vector
int16_t mvalue=90; // manual position reading
int16_t m_value; // manual positioning
char mstr[6]="90"; // manual position vector
ptr=str;
/***Parameters timers***/
tc0()->callback.comp_vect = timer0_comp_vect;
tc0()->compare(249);
tc0()->start(64);
tc1()->compoutmodeB(2);
tc1()->compareA(20000);
tc1()->start(8);
pid_1.set_kc(&pid_1.par, 1);
pid_1.set_kd(&pid_1.par, 0.2); //
pid_1.set_ki(&pid_1.par, 0.5); // will provoke overshoot, to much acceleration limit max value and minimum value.
pid_1.set_SP(&pid_1.par, 650);
/***Another one woopy ti dooo***/
pid_2.set_kc(&pid_2.par, 0.5);
pid_2.set_kd(&pid_2.par, 1);
pid_2.set_ki(&pid_2.par, 0.01);
pid_2.set_SP(&pid_2.par, 125);
/**********/
//TODO:: Please write your application code
while(TRUE){
/***PREAMBLE***/
lcd0()->reboot();
keypad.read();
//uartreceive=uart.read();
/***Reading input***/
lcd0()->gotoxy(3,13);
lcd0()->putch(':');
lcd0()->string_size(keypad.data->print,6);
/***ENTRY END***/
switch(Menu){
	/***MENU 1***/
	case '1': // Main Program Menu
		if(!strcmp(keypad.data->string,"A")){Menu='2';keypad.flush();lcd0()->clear();break;}
		if(!strcmp(keypad.data->string,"B")){Menu='3';keypad.flush();lcd0()->clear();break;}
			/***Reading analog***/
			adcvalue=adc()->read(0);
			lcd0()->gotoxy(0,0);
			lcd0()->string_size("Output: ",7);
			strcpy(str,func()->i16toa(adcvalue));
			lcd0()->string_size(str,4);
					
			if(pid_out_1 >-1024 && pid_out_1 <1024){
				tmp=func()->trimmer(pid_out_1,-1023,1023,Min,Max);
				tc1()->compareB(tmp);
				lcd0()->gotoxy(1,0);
				strcpy(str,func()->i16toa(tmp));
				lcd0()->string_size(str,4);
			}
					
			func()->ftoa(35.00567, 6);
			lcd0()->gotoxy(2,0);
			lcd0()->string_size(str,12);
			//lcd0()->gotoxy(2,0);
			//strcpy(str,func()->i32toa(pid_1.PV));
			//lcd0()->string_size(str,6);
			//if(pid_1.derivative>0){
				lcd0()->gotoxy(3,0);
				strcpy(str,func()->i32toa(pid_1.par.derivative));
				lcd0()->string_size(str,6);
			//}
					
					
			// PID_1 output
			lcd0()->gotoxy(0,13);
			strcpy(str,func()->i32toa(pid_out_1));
			lcd0()->string_size(str,6);
					
			// PID_2 output
			lcd0()->gotoxy(1,13);
			strcpy(str,func()->i32toa(pid_out_2));
			lcd0()->string_size(str,6);
					
		break;
	/***MENU 2***/
	case '2': // Manual position override 
		if(!strcmp(keypad.data->string,"A")){Menu='3';keypad.flush();lcd0()->clear();break;}
		if(!strcmp(keypad.data->string,"B")){Menu='1';keypad.flush();lcd0()->clear();break;}
		if(!strcmp(keypad.data->string,"C")){Menu='1';keypad.flush();lcd0()->clear();break;}
			lcd0()->gotoxy(0,0);
			lcd0()->string_size("Manual: ",8);
			lcd0()->string_size(mstr,3);
			lcd0()->gotoxy(1,0);
			lcd0()->string_size("Enter Angle",11);
			if(keypad.data->character==KEYPADENTERKEY){
				strncpy(mstr,keypad.data->string,6);
				mvalue=func()->strToInt(mstr);
				if(mvalue >=0 && mvalue <181){
					m_value=mvalue;
					tc1()->compareB(func()->trimmer(m_value,0,180,Min,Max));
					lcd0()->hspace(5);
				}else{
					lcd0()->string_size("  err",5);
				}
				keypad.flush();
			}
			//else
			//	tc1()->compareB(func()->trimmer(m_value,0,180,Min,Max));
			lcd0()->gotoxy(3,0);
			lcd0()->string_size("C - exit",8);
		break;
	/***MENU 3***/
	case '3': //Set Time and Date
		if(!strcmp(keypad.data->string,"A")){Menu='1';keypad.flush();lcd0()->clear();break;}
		if(!strcmp(keypad.data->string,"B")){Menu='2';keypad.flush();lcd0()->clear();break;}
		if(!strcmp(keypad.data->string,"C")){Menu='1';keypad.flush();lcd0()->clear();break;}
			lcd0()->gotoxy(0,0);
			lcd0()->string_size("Not being used",19);
					
			/***Play around***/
				
				
				
				
		break;
		/********************************************************************/
	default:
		Menu='1';
		break;
};
}}

/*** Procedure and Function definition ***/
void PORTINIT(void)
{
	//INPUT
	DDRF=0x00;
	PORTF=0x0F;
	//OUTPUT
	DDRB|=(1<<5) | (1<<6) | (1<<7);
}

void timer0_comp_vect(void) // 1Hz and usart Tx
{
	uint8_t Sreg;
	Sreg = SREG;
	SREG &= ~(1<<7);
	if(count>repeat){ //59 -> 1Hz
		pid_out_1=pid_1.output(&pid_1.par,adcvalue,0.5);
		pid_out_2=pid_2.output(&pid_2.par,adcvalue,0.5);
		increment++;
		if((increment & 0x0F) < 8){
			shift.bit(&shift.par, 0);
			shift.out(&shift.par);
		}else{
			shift.bit(&shift.par, 1);
			shift.out(&shift.par);
		}
		
		count=0;
	}else
		count++;
	SREG = Sreg;
}

/*** EOF ***/




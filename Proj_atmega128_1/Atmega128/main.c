/*********************************************************
File: LFSM
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
	-PORTA : LCD Display 4X20
	-PORTB : Buttons
	-PORTC : Led s
	-PORTE : Keypad 4X4
	-PORTG : HC595 PIN 0, 1, 2.
Date:
	      25/10/2020
Comment:
	LFSM
	This program can be used to create a lfsm program (eeprom file).
**********************************************************/
/*** Working Frequency ***/
#define F_CPU 16000000UL

/*** Library ***/
#include "atmega128mapping.h"
#include "lcd.h"
#include "function.h"
#include "keypad.h"
#include "lfsm.h"
#include "74hc595.h"

/*** Constant & Macro ***/
#define True 1

/*** Variable ***/
char* string=NULL;

/*** Procedure and Function declaration ***/
void PORTINIT(void);

/*** HANDLER ***/
int main(void)
{
PORTINIT();
uint8_t option=0; // Menu Jumper
uint8_t input_tmp=0;
KEYPAD_Data* keypadinput;
uint16_t output=0;
uint16_t mask=0;
uint8_t page=0;
uint16_t n;
KEYPAD_Handler keypad = keypad_enable(&DDRE,&PINE,&PORTE);
lcd0_enable(&DDRA,&PINA,&PORTA);
LFSM_Handler lfsm = lfsm_enable(eeprom(),363);
//HC595 shift = HC595enable(&DDRG,&PORTG,2,0,1);
/*****************************************************/
lfsm.setoutput(&lfsm,0);
/********* Replace with your application code **********/
while (True)
{
/*** Preamble ***/
lcd0()->reboot();
keypad.read();
/****** Readings ******/
keypadinput = keypad.data;
n=PINB; lcd0()->gotoxy(0,14); string=func()->ui16toa(n);
lcd0()->string_size(string,5);
/******/
switch(option){
	case 0: // Main Menu
		lcd0()->gotoxy(0,0);
		lcd0()->string_size("Running ",8);
		string=func()->ui16toa(lfsm.getpage(&lfsm));
		lcd0()->string_size(string,3);
		if(keypadinput->character == 'A'){ // Clear screen and empty buffer
			lcd0()->clear();
			keypad.flush();
			continue;
		}
		if(keypadinput->character == 'B'){ // Learn
			lcd0()->clear();
			keypad.flush();
			option=1;
		}
		if(keypadinput->character == 'C'){ // Delete or remove
			lcd0()->clear();
			keypad.flush();
			option=6;
		}
		lfsm.read(&lfsm,n); // Getting answer
		break;
	case 1: // New Entry Menu
		// Menu
		lcd0()->gotoxy(0,0);
		lcd0()->string_size("Enter: ",12);
		lcd0()->gotoxy(1,0);
		lcd0()->string_size("In  Out  Mask  Page",20);
		option=2;
		break;
	case 2: // First entry [input]
		lcd0()->gotoxy(0,7);
		lcd0()->string_size(keypadinput->print,4);
		if(keypadinput->character == 'D'){
			input_tmp=func()->strToInt(keypadinput->string);
			lcd0()->gotoxy(2,0);
			string=func()->ui16toa(input_tmp);
			lcd0()->string(string);
			keypad.flush();
			option=3;
		}
		if(keypadinput->character == 'C'){
			keypad.flush();
			option=2;
		}
		break;
	case 3: // Second Entry [output]
		lcd0()->gotoxy(0,7);
		lcd0()->string_size(keypadinput->print,4);
		if(keypadinput->character == 'D'){
			output=func()->strToInt(keypadinput->string);
			lcd0()->gotoxy(2,5);
			string=func()->ui16toa(output);
			lcd0()->string(string);
			keypad.flush();
			option=4;
		}
		if(keypadinput->character == 'C'){
			keypad.flush();
			option=3;
		}
		break;
	case 4: // Third Entry [mask]
		lcd0()->gotoxy(0,7);
		lcd0()->string_size(keypadinput->print,4);
		if(keypadinput->character == 'D'){
			mask=func()->strToInt(keypadinput->string);
			lcd0()->gotoxy(2,10);
			string=func()->ui16toa(mask);
			lcd0()->string(string);
			keypad.flush();
			option=5;
		}
		if(keypadinput->character == 'C'){
			keypad.flush();
			option=4;
		}
		break;
	case 5: // Fourth entry [page] and upload with reply
		lcd0()->gotoxy(0,7);
		lcd0()->string_size(keypadinput->print,4);
		if(keypadinput->character == 'D'){ // D is the enter key on the keyboard
			page=func()->strToInt(keypadinput->string);
			lcd0()->gotoxy(2,15);
			string=func()->ui16toa(page);
			lcd0()->string(string);
			//lcd0()->gotoxy(1,0);
			//lcd0()->hspace(20);
			//lcd0()->gotoxy(2,0);
			//lcd0()->hspace(20);
			switch(lfsm.learn(&lfsm,input_tmp,output,mask,page)){
				case 0: // Not used
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("disabled",7);
					break;
				case 4: // already programmed
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("exists",7);
					break;
				case 2: // new entry added
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("upload",7);
					break;
				case 3: // eeprom is full reject entry
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("full",7);
					break;
				default:
					break;
			}//End switch
		/******/
		//lfsm.read(&lfsm,input_tmp);
		keypad.flush();
		option=0;
		}//End if
		if(keypadinput->character == 'C'){ // re-enter value [page] 
			keypad.flush();
			option=5;
		}
		break;
	case 6: // Delete or Remove or No action Menu
		lcd0()->gotoxy(1,0);
		lcd0()->string_size("1-Delete 4-quant",16);
		lcd0()->gotoxy(2,0);
		lcd0()->string_size("2-Remove 3-Exit",16);
		option=7;
		break;
	case 7: // Delete or Remove or No action choice
		if(keypadinput->character == '1'){ // Delete
			lcd0()->gotoxy(1,0);
			lcd0()->hspace(17);
			lcd0()->gotoxy(2,0);
			lcd0()->hspace(17);
			lfsm.deleteall(&lfsm);
			lfsm.setpage(&lfsm,0);
			//lfsm.setoutput(&lfsm,0X00);
			keypad.flush();
			lcd0()->gotoxy(3,12);
			lcd0()->string_size("deleted",7);
			option=0;
		}else if(keypadinput->character == '2'){ // Remove
			lcd0()->gotoxy(1,0);
			lcd0()->hspace(17);
			lcd0()->gotoxy(2,0);
			lcd0()->hspace(17);
			keypad.flush();
			option=8;
		}else if(keypadinput->character == '3'){ // No action
			lcd0()->gotoxy(1,0);
			lcd0()->hspace(17);
			lcd0()->gotoxy(2,0);
			lcd0()->hspace(17);
			keypad.flush();
			option=0;
		}else if(keypadinput->character == '4'){ // Quant
			lcd0()->gotoxy(1,0);
			lcd0()->hspace(17);
			lcd0()->gotoxy(2,0);
			lcd0()->hspace(17);
			lcd0()->gotoxy(3,12);
			lcd0()->string_size(func()->i16toa(lfsm.quant(&lfsm)),7);
			keypad.flush();
			option=0;
		}
		break;
	case 8: // Remove Entry Data
		lcd0()->gotoxy(0,0);
		lcd0()->string_size("Enter: ",12);
		lcd0()->gotoxy(1,0);
		lcd0()->string_size("Input",20);
		option=9;
		break;
	case 9: // Remove Entry from EEprom
		lcd0()->gotoxy(0,7);
		lcd0()->string_size(keypadinput->print,4);
		if(keypadinput->character == 'D'){ // D is the enter key on the keyboard
			input_tmp=func()->strToInt(keypadinput->string);
			lcd0()->gotoxy(2,0);
			string=func()->ui16toa(input_tmp);
			lcd0()->string(string);
			switch(lfsm.remove(&lfsm,input_tmp)){ // feedback reply
				case 0:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("Non",7);
					break;
				case 1:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("Unknown",7);
					break;
				case 2:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("g logic",7);
					break;
				case 3:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("l logic",7);
					break;
				case 4:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("local",7);
					break;
				case 41:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("above",7);
					break;
				case 42:
					lcd0()->gotoxy(3,12);
					lcd0()->string_size("bellow",7);
					break;
				default:
					break;
			}//End switch
			/******/
			//lfsm.read(&lfsm,input_tmp);
			keypad.flush();
			option=0;
		}//End if
		if(keypadinput->character == 'C'){ // Repeat Remove Data Entry
			keypad.flush();
			option=8;
		}
		break;
	default:
		break;
}//End switch
/***DISPLAY***/
lcd0()->gotoxy(3,0);
lcd0()->string_size("Output ",7);
string=func()->ui16toa(lfsm.getoutput(&lfsm));
lcd0()->string_size(string,4);
//lcd0()->hspace(2);
//string=func()->ui16toa(lfsm.getstatus(&lfsm));
//lcd0()->string_size(string,4);
//shift.byte(lfsm.getoutput(&lfsm));
PORTC=~lfsm.getoutput(&lfsm);
}}

/*** Procedure and Function declaration ***/
void PORTINIT(void)
{
	DDRB=0X00; // Buttons
	PORTB=0XFF; // Buttons PULLUP
	DDRC=0XFF; // Led
	PORTC=0X00; // Led OFF
}

/*** EOF ***/


/***Comment
1º Sequence
2º Scope
3º Pointer and Variable
4º Casting
note: not defining interrupt function, or initializing an object will block the program.
***/




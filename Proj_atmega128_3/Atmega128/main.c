/************************************************************************
Title: BALANCA COMERCIAL
Author:    <sergio.salazar.santos@gmail.com>
License:   GNU General Public License
File: $Id: MAIN,v 1.8.2.1 21/02/2021 Exp $
Software:  Atmel Studio 7 (ver 7.0.129)
Hardware:  Atmega128 by ETT ET-BASE
	-PORTA LCD
	-PORTF pin 6,7 HX711, pin 0 to 5 Buttons, 
		PIN 0 -> OFFSET, 
		PIN 3 -> DEFAULT 5sec press, and up count for div factor, 
		PIN 4 -> ENTER GAIN FACTOR MENU 5 sec press, and down count for div factor, 
		PIN 5 -> ENTER KEY to validate entered value and put in effect.
		PIN 6 and 7 -> dedicated to comunicate with Signal Amplifier
	-PORTC status indicator leds
		PIN 5 -> Indicate using stored values
		PIN 6 -> Reset to default indicator (blinks four times)
		PIN 7 -> In Calibration Menu (on)
Comment:
	nice
************************************************************************/
/*** Working Frequency ***/
#define F_CPU 16000000UL

/*** Library ***/
#include "atmega128timer0.h"
#include "atmegaeeprom.h"
#include "atmega128timer1.h"

#include "explode.h"
#include "function.h"
#include "lcd.h"
#include "hx711.h"
#include <string.h>

/*** Constant & Macro***/
#define ZERO 0
#define ONE 1
#define TRUE 1
#define average_n 24 //64 -> 24
#define blink 8
#define IMASK 0x3F
#define _5sec 5
#define _10sec 10
#define minDIV 1
#define maxDIV 255

/*** Variable ***/
EXPLODE_Handler F;
//INTERRUPT intx;
HX711_Calibration HX711_data;
HX711_Calibration* HX711_ptr;
const uint8_t sizeblock = sizeof(HX711_Calibration);
HX711_Handler hx;
float tmp;
char result[32];
char Menu = '1'; // Main menu selector
uint8_t counter_1 = ZERO;
uint8_t counter_2 = ZERO;
uint8_t SiGnal = ZERO;
uint8_t count=blink;
uint16_t divfactor;

/*** Procedure and Function declaration ***/
void timer0_comp_vect(void);
void timer1_compa_vect(void);
void PORTINIT();

/**** HANDLER ****/
int main(void)
{
PORTINIT();
HX711_ptr = &HX711_data; // CALIBRATION DATA BUS

F = explode_enable();
tc0_enable(2,2); //2,2
tc1_enable(4,2); //4,2
hx = hx711_enable(&DDRF, &PINF, &PORTF, 6, 7); //6,7
//intx = INTERRUPTenable();
lcd0_enable(&DDRA,&PINA,&PORTA);

float value = 0;
float publish = 0;
uint8_t choice;
// Get default values to buss memory
HX711_data.offset_32 = hx.get_cal(&hx)->offset_32;
HX711_data.offset_64 = hx.get_cal(&hx)->offset_64;
HX711_data.offset_128 = hx.get_cal(&hx)->offset_128;
HX711_data.divfactor_32 = hx.get_cal(&hx)->divfactor_32;
HX711_data.divfactor_64 = hx.get_cal(&hx)->divfactor_64;
HX711_data.divfactor_128 = hx.get_cal(&hx)->divfactor_128;
HX711_data.status = hx.get_cal(&hx)->status;

tc0()->callback.comp_vect = timer0_comp_vect;
tc0()->compoutmode(1); // troubleshooting blinking PORTB 5
tc0()->compare(60); // 8 -> 79 -> 80 us, fine tunned = 8 -> 60 -> 30.4us
tc0()->start(8); // 1 -> 32 us , 8 -> 256 us , 32 64 128 256 1024

// to be used to jump menu for calibration in progress
tc1()->callback.compa_vect = timer1_compa_vect;
tc1()->compoutmodeA(1); // troubleshooting blinking PORTB 6
tc1()->compareA(62800); // Freq = 256 -> 62800 -> 2 s
tc1()->start(256);
//intx.set(1,0); // Not necessary, if used move IDC from PORTF to PORTD with new config pinage.
// HX711 Gain
hx.set_amplify(&hx, 64); // 32 64 128
choice = hx.get_amplify(&hx);
if(choice == 1)
	divfactor = (uint16_t) HX711_data.divfactor_128;
if(choice == 2)
	divfactor = (uint16_t) HX711_data.divfactor_32;
if(choice == 3)
	divfactor = (uint16_t) HX711_data.divfactor_64;
//Get stored calibration values and put them to effect
eeprom()->read_block(HX711_ptr, (const void*) ZERO, sizeblock);
if(HX711_ptr->status == 1){
	//Load stored value 
	hx.get_cal(&hx)->offset_32 = HX711_ptr->offset_32;
	hx.get_cal(&hx)->offset_64 = HX711_ptr->offset_64;
	hx.get_cal(&hx)->offset_128 = HX711_ptr->offset_128;
	hx.get_cal(&hx)->divfactor_32 = HX711_ptr->divfactor_32;
	hx.get_cal(&hx)->divfactor_64 = HX711_ptr->divfactor_64;
	hx.get_cal(&hx)->divfactor_128 = HX711_ptr->divfactor_128;
	hx.get_cal(&hx)->status=ZERO;
	PORTC &= ~(ONE << 5); // troubleshooting
}

//lcd0.gotoxy(1,0); // for troubleshooting
//lcd0.string_size(func()->ftoa(HX711_data.status, result, ZERO), 13);
//lcd0.string_size(func()->ftoa(hx.get_cal(&hx)->offset_64, result, ZERO), 13);

while(1){

lcd0()->reboot(); //Reboot LCD
F.update(&F.par, PINF); //PORTF INPUT READING

// Jump Menu SiGnal
if(SiGnal == ONE){ //INPUT FROM INTERRUPT SINALS
	Menu = '2';
	SiGnal = ZERO; // ONE SHOT
	lcd0()->clear();
}

while(hx.query(&hx))
//Catches falling Edge instance, begins bit shifting.

tmp = hx.raw_average(&hx, average_n); // average_n  25 or 50, smaller means faster or more readings

switch(Menu){
	
	case '1': // Main Program Menu
		lcd0()->gotoxy(0,4); //TITLE
		lcd0()->string_size("Weight Scale", 12); //TITLE
		
		//lcd0.gotoxy(1,0); // for troubleshooting
		//lcd0.string_size(func()->ftoa(hx.read_raw(&hx), result, ZERO), 13);
		
		if((F.par.HL & IMASK) & ONE){ // calibrate offset by pressing button 1
			HX711_data.offset_32 = tmp;
			HX711_data.offset_64 = tmp;
			HX711_data.offset_128 = tmp;
			HX711_data.status = ONE;
			eeprom()->update_block(HX711_ptr, (void*) ZERO, sizeblock);
			hx.get_cal(&hx)->offset_32 = HX711_ptr->offset_32;
			hx.get_cal(&hx)->offset_64 = HX711_ptr->offset_64;
			hx.get_cal(&hx)->offset_128 = HX711_ptr->offset_128;
			hx.get_cal(&hx)->status=ZERO;
			PORTC &= ~(ONE << 5);
		}
		if(choice == 1 || choice == 11)
			value = (tmp - hx.get_cal(&hx)->offset_128) / hx.get_cal(&hx)->divfactor_128; //value to be published to LCD
		if(choice == 2 || choice == 21)
			value = (tmp - hx.get_cal(&hx)->offset_32) / hx.get_cal(&hx)->divfactor_32; //value to be published to LCD
		if(choice == 3 || choice == 31)
			value = (tmp - hx.get_cal(&hx)->offset_64) / hx.get_cal(&hx)->divfactor_64; //value to be published to LCD
		
		//lcd0.gotoxy(3,0); // for troubleshooting
		//lcd0.string_size(func()->ftoa(tmp, result, ZERO), 13);
		//lcd0.string_size(func()->ftoa(hx.get_cal(&hx)->divfactor_128, result, ZERO), 13);
		//lcd0.string_size(func()->ftoa(hx.get_cal(&hx)->offset_128, result, ZERO), 13);
		
		if (value > 1000 || value < -1000){
			publish = value / 1000;
			lcd0()->gotoxy(2,1);
			lcd0()->string_size(func()->ftoa(publish, 3), 13); lcd0()->string_size("Kg", 4);
		}else{
			publish = value;
			lcd0()->gotoxy(2,1);
			lcd0()->string_size(func()->ftoa(publish, ZERO), 13); lcd0()->string_size("gram", 4);
		}
		break;
	
	case '2': // MANUAL CALIBRATE DIVFACTOR MENU
		
		lcd0()->gotoxy(0,1);
		lcd0()->string_size("SETUP GAIN FACTOR",17);
		switch(choice){
			case 1: // case 128
				divfactor=hx.get_cal(&hx)->divfactor_128;
				choice=11;
				break;
			case 11: // case 128
				lcd0()->gotoxy(2,9);
				if(F.par.HL == (ONE << 3)){
					divfactor++;
					if(divfactor > maxDIV)
						divfactor = maxDIV;
				}
				if(F.par.HL == (ONE << 4)){
					divfactor--;
					if(divfactor < minDIV)
						divfactor = minDIV;
				}
				HX711_data.divfactor_128 = divfactor;
				lcd0()->string_size(func()->ui16toa(divfactor),6);
				break;
			case 2: // case 32
				divfactor=hx.get_cal(&hx)->divfactor_32;
				choice=21;
				break;
			case 21: // case 32
				lcd0()->gotoxy(2,9);
				if(F.par.HL == (ONE << 3)){
					divfactor++;
					if(divfactor > maxDIV)
						divfactor = maxDIV;
				}
				if(F.par.HL == (ONE << 4)){
					divfactor--;
					if(divfactor < minDIV)
						divfactor=minDIV;
				}
				HX711_data.divfactor_32 = divfactor;
				lcd0()->string_size(func()->ui16toa(divfactor),6);
				break;
			case 3: // case 64
				divfactor=hx.get_cal(&hx)->divfactor_64;
				choice=31;
				break;
			case 31: // case 64
				lcd0()->gotoxy(2,9);
				if(F.par.HL == (ONE << 3)){
					divfactor++;
					if(divfactor > maxDIV)
						divfactor = maxDIV;
				}
				if(F.par.HL == (ONE << 4)){
					divfactor--;
					if(divfactor < minDIV)
						divfactor = minDIV;
				}
				HX711_data.divfactor_64 = divfactor;
				lcd0()->string_size(func()->ui16toa(divfactor),6);
				break;
			default:
				choice = 3;
				break;
		};
		// Exit and store value
		if((F.par.LL & IMASK) == (ONE << 5)){ // Button 6
			HX711_data.status = ONE;
			eeprom()->update_block(HX711_ptr, (void*) ZERO, sizeblock);
			hx.get_cal(&hx)->divfactor_32=divfactor;
			hx.get_cal(&hx)->divfactor_64=divfactor;
			hx.get_cal(&hx)->divfactor_128=divfactor;
			hx.get_cal(&hx)->status=ZERO;
			PORTC &= ~(ONE << 5); // troubleshooting
			PORTC |= (ONE << 7); // troubleshooting
			counter_2 = ZERO;
			Menu = '1';
			lcd0()->clear();
		}
		
		break;
	default:
		Menu = '1';
		break;
		
};
}}

/*** Procedure and Function definition ***/
void PORTINIT(void)
{
	//Control buttons
	PORTF |= IMASK;
	//troubleshooting output
	DDRC = 0xFF;
	PORTC = 0xFF;
}

void timer0_comp_vect(void) // 15.4 us intervals
{
	/***Block other interrupts during this procedure***/
	uint8_t Sreg = cpu_reg()->sreg.var;
	cpu_reg()->sreg.par.i = 0;
	hx.read_raw(&hx);
	/***enable interrupts again***/
	cpu_reg()->sreg.var = Sreg;
}
void timer1_compa_vect(void) // 1 second intervals
{
	/***CLEAR EEPROM OFFSET SEQUENCE START***/
	if((F.par.LL & IMASK) == (ONE << 3)) //button 4
		counter_1++;
	else if(counter_1 < _5sec+ONE)
		counter_1=ZERO;
	if(counter_1 > _5sec){
		counter_1 = _5sec+ONE; //lock in place
		PORTC ^= (ONE << 6); // troubleshooting
		count--;
		if(!count){ //led blinks x times
			// Delete eeprom memory ZERO
			eeprom()->update_block(HX711_Default, (void*) ZERO, sizeblock);
			hx.get_cal(&hx)->offset_32 = HX711_Default->offset_32;
			hx.get_cal(&hx)->offset_64 = HX711_Default->offset_64;
			hx.get_cal(&hx)->offset_128 = HX711_Default->offset_128;
			hx.get_cal(&hx)->divfactor_32 = divfactor = HX711_Default->divfactor_32;
			hx.get_cal(&hx)->divfactor_64 = divfactor = HX711_Default->divfactor_64;
			hx.get_cal(&hx)->divfactor_128 = HX711_Default->divfactor_128;
			hx.get_cal(&hx)->status = HX711_Default->status;
			PORTC |= (ONE << 5); // troubleshooting
			PORTC |= (ONE << 6); // troubleshooting
			counter_1 = ZERO;
			count=blink;
		}
	}
	/***CLEAR EEPROM OFFSET SEQUENCE END***/
	/***CAL DIVFACTOR DEFINE START***/
	if((F.par.LL & IMASK) == (ONE << 4)) //button 5
		counter_2++;
	else if(counter_2 < _5sec+ONE)
		counter_2=ZERO; //RESET TIMER
	if(counter_2 > _5sec){
		counter_2 = ZERO; //RESET TIMER
		SiGnal = ONE;
		PORTC &= ~(ONE << 7); // troubleshooting
	}
	/***CAL DIVFACTOR DEFINE END***/
}

/*** EOF ***/


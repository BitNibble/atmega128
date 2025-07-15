/*************************************************************************
	ESP01ATCOMMANDS
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ALL
Update:   29/12/2023
*************************************************************************/
/*** Library ***/
#include "esp01atcommands.h"
#include <string.h>

/*** Variable ***/
char ESP01ATstr[32];

/*** Procedure and Function definition ***/
char* esp01at_cmd(uint16_t num){
	switch(num)
	{
		/*** Query ***/
		case 1: // esp01
			strcpy(ESP01ATstr, "AT\r\n");
		break;
		case 28: // esp01 choose type com
			strcpy(ESP01ATstr, "AT+CWMODE?\r\n");
		break;
		case 29: // esp01 check if already connected, display the station IP address of our ESP-01 module
			strcpy(ESP01ATstr, "AT+CIFSR\r\n");
		break;
		/*** while connected ***/
		case 17: // Very usefull
			strcpy(ESP01ATstr, "AT+GETSTAT\r\n"); // Param ? 0-1 , 0——not connected, 1——connected
		break;
		case 18:
			strcpy(ESP01ATstr, "AT+DISC\r\n"); // Disconnect no
		break;
		/*******************/
		/****** SETUP ******/
		/*******************/
		case 281: // esp01 STA
			strcpy(ESP01ATstr, "AT+CWMODE=1\r\n");
		break;
		case 282: // esp01 AP
			strcpy(ESP01ATstr, "AT+CWMODE=2\r\n");
		break;
		case 283: // esp01 BOTH
			strcpy(ESP01ATstr, "AT+CWMODE=3\r\n");
		break;
		case 30: // esp01
			strcpy(ESP01ATstr, "AT+CWJAP=NOS-9C64,RUSXRCKL\r\n");
		break;
		case 311: // esp01 configure the ESP8266 ESP-01 module as a Single server
			strcpy(ESP01ATstr, "AT+CIPMUX=0\r\n");
		break;
		case 312: // esp01 configure the ESP8266 ESP-01 module as a Multiple server
			strcpy(ESP01ATstr, "AT+CIPMUX=1\r\n");
		break;
		case 321: // esp01 start the Open server at port 80
			strcpy(ESP01ATstr, "AT+CIPSERVER=1,80\r\n");
		break;
		case 322: // esp01 start the Close server at port 80
			strcpy(ESP01ATstr, "AT+CIPSERVER=0,80\r\n");
		break;
		case 331: // esp01 0 indicates the channel while 5 the number of characters that are going to be sent
			strcpy(ESP01ATstr, "AT+CIPSEND=0,5\r\n");
		break;
		case 341:	// esp01 0 indicates the channel while 5 the number of characters that are going to be sent
					// write message then enter "SEND OK."
			strcpy(ESP01ATstr, "AT+CIPSEND=0,5\r\n");
		break;
		case 342: // esp01 it is required to close the channel 0 first in order to display the characters on the browser
			strcpy(ESP01ATstr, "AT+CIPCLOSE=0\r\n");
		break;
		/*** Test ***/
		
		/*** Preamble ***/
		case 26: // hidden
		//	strcpy(ESP01ATstr, "AT+DEFAULT\r\n"); // 4dbm, 9600bps (115200bps), 200ms, PM=0, On board LED 2:1, External indicator led1: 2
		break;
		case 27:
			strcpy(ESP01ATstr, "AT+HELP\r\n"); // Lists commands
		break;
		/******/
		default:
			strcpy(ESP01ATstr, "AT\r\n");
		break;
	};
	return ESP01ATstr;
}

/*** EOF ***/


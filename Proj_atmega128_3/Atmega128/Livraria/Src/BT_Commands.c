/******************************************************************************
	BT AT COMMANDS
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ALL
Date:     20102024
Comment:
	BT05 and ZS-040
*******************************************************************************/
#include "BT_Commands.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*****************************************************/
/*** USER ***/
const char* BT_AT_NAME_1(void) {
	return "HOME1";
}
const char* BT_AT_NAME_2(void) {
	return "BT05";
}
const char* BT_AT_PIN_1(void) {
	return "916919";
}
/*****************************************************/
/******** General Inquiry Function AT Commands *******/
/*****************************************************/
const char* BT_AT_Test(void) {
	return "AT\r\n";
}
const char* BT_AT_GetVersion(void) {
	return "AT+VERSION\r\n";
}
const char* BT_AT_GetName(void) {
	return "AT+NAME\r\n";
}
const char* BT_AT_GetPin(void) {
	return "AT+PIN\r\n";
}
const char* BT_AT_GetStat(void) {
	return "AT+GETSTAT\r\n"; // Param ? 0-1 , 0——not connected, 1——connected
}
const char* BT_AT_GetRole(void) {
	return "AT+ROLE\r\n";
}
const char* BT_AT_GetAdvi(void) {
	return "AT+ADVI\r\n";
}
const char* BT_AT_GetType(void) {
	return "AT+TYPE\r\n";
}
const char* BT_AT_GetBaud(void) {
	return "AT+BAUD\r\n";
}
const char* BT_AT_GetLaddr(void) {
	return "AT+LADDR\r\n";
}
const char* BT_AT_GetUuid(void) {
	return "AT+UUID\r\n";
}
const char* BT_AT_GetChar(void) {
	return "AT+CHAR\r\n";
}
const char* BT_AT_GetInq(void) {
	return "AT+INQ\r\n";
}
const char* BT_AT_GetTest(void) {
	return "AT+GMR\r\n";
}
/*****************************************************/
/*****************************************************/
/*****************************************************/
const char* BT_AT_EnterSleep(void) {
	return "AT+SLEEP\r\n";
}
const char* BT_AT_Reset(void) {
	return "AT+RESET\r\n";
}
const char* BT_AT_RestoreDefaults(void) {
	return "AT+ORGL\r\n";
}
const char* BT_AT_Disconnect(void) {
	return "AT+DISC\r\n";
}
const char* BT_AT_GetHelp(void) {
	return "AT+HELP\r\n";
}
/*****************************************************/
/******** General Setting Function AT Commands *******/
/*****************************************************/
/*** Name Command ***/
const char* BT_AT_SetName(const char *name) {
	static char command[50];
	const char* str = NULL;
	uint8_t length = 0;
	if (name == NULL) { length = 0; }else { length = strlen(name); }
	if(length > 0 && length < 13){
		snprintf(command, sizeof(command), "AT+NAME%s\r\n", name);
		str = command;
	}
	return str;
}
/*** Pin Command ***/
const char* BT_AT_SetPin(uint32_t pin) { // Six digits
	const char* str = NULL;
	static char command[50];
	if(pin < 1000000) {
		snprintf(command, sizeof(command), "AT+PIN%06lu\r\n", pin);
		str = command;
	}
	return str;
}
/*** Role Commands ***/
const char* BT_AT_SetRole(uint8_t role) {
	const char* str = NULL;
	// Array of role rate
	const char* rolenum[] = {
			"AT+ROLE0\r\n",		// 0
			"AT+ROLE1\r\n",		// 1
			"AT+ROLE2\r\n",		// 2
			"AT+ROLE3\r\n",		// 3
			"AT+ROLE4\r\n",		// 4
			};
	switch(role){
		case 0: str = rolenum[0]; break;
		case 1: str = rolenum[1]; break;
		case 2: str = rolenum[2]; break;
		case 3: str = rolenum[3]; break;
		case 4: str = rolenum[4]; break;
		default: str = rolenum[0]; break;
	}
	return str;
}
const char* BT_AT_SetRole0(void) {
	return "AT+ROLE0\r\n"; // 0 ——slave mode
}
const char* BT_AT_SetRole1(void) {
	return "AT+ROLE1\r\n"; // 1 ——master mode
}
const char* BT_AT_SetRole2(void) {
	return "AT+ROLE2\r\n"; // 2 ——sensor mode
}
const char* BT_AT_SetRole3(void) {
	return "AT+ROLE3\r\n"; // 3 ——iBeacon mode
}
const char* BT_AT_SetRole4(void) {
	return "AT+ROLE4\r\n"; // 4 ——WeChat mode
}
/*** Advi Commands ***/
const char* BT_AT_SetAdvi(uint8_t advi) {
	const char* str = NULL;
	// Array of advi rate
	const char* advinum[] = {
			"AT+ADVI0\r\n",		// 0
			"AT+ADVI1\r\n",		// 1
			"AT+ADVI2\r\n",		// 2
			"AT+ADVI3\r\n",		// 3
			"AT+ADVI4\r\n",		// 4
			"AT+ADVI5\r\n",		// 5
			"AT+ADVI6\r\n",		// 6
			};
	switch(advi){
		case 0: str = advinum[0]; break;
		case 1: str = advinum[1]; break;
		case 2: str = advinum[2]; break;
		case 3: str = advinum[3]; break;
		case 4: str = advinum[4]; break;
		case 5: str = advinum[5]; break;
		case 6: str = advinum[6]; break;
		default: str = advinum[3]; break;
	}
	return str;
}
const char* BT_AT_SetAdvi0(void) {
	return "AT+ADVI0\r\n"; // 0 ——100ms
}
const char* BT_AT_SetAdvi1(void) {
	return "AT+ADVI1\r\n"; // 1 ——500ms
}
const char* BT_AT_SetAdvi2(void) {
	return "AT+ADVI2\r\n"; // 2 ——7500ms
}
const char* BT_AT_SetAdvi3(void) {
	return "AT+ADVI3\r\n"; // 3 ——1000ms
}
const char* BT_AT_SetAdvi4(void) {
	return "AT+ADVI4\r\n"; // 4 ——2000ms
}
const char* BT_AT_SetAdvi5(void) {
	return "AT+ADVI5\r\n"; // 5 ——4000ms
}
const char* BT_AT_SetAdvi6(void) {
	return "AT+ADVI6\r\n"; // 6 ——8000ms
}
/*** Type Commands ***/
const char* BT_AT_SetType(uint8_t type) {
	const char* str = NULL;
	// Array of type rate
	const char* typenum[] = {
			"AT+TYPE0\r\n",		// 0
			"AT+TYPE1\r\n",		// 1
			"AT+TYPE2\r\n",		// 2
			};
	switch(type){
		case 0: str = typenum[0]; break;
		case 1: str = typenum[1]; break;
		case 2: str = typenum[2]; break;
		default: str = typenum[0]; break;
	}
	return str;
}
const char* BT_AT_SetType0(void) {
	return "AT+TYPE0\r\n"; // 0 ——no password
}
const char* BT_AT_SetType1(void) {
	return "AT+TYPE1\r\n"; // 1 ——password pairing
}
const char* BT_AT_SetType2(void) {
	return "AT+TYPE2\r\n"; // 2 ——password pairing and binding
}
/*** Baud Rate Commands ***/
const char* BT_AT_SetBaud(uint32_t baud) {
	const char* str = NULL;
	// Array of baud rate
	const char* baudrate[] = {
			"AT+BAUD1\r\n",		// 1200
			"AT+BAUD2\r\n",		// 2400
			"AT+BAUD3\r\n",		// 4800
			"AT+BAUD4\r\n",		// 9600
			"AT+BAUD5\r\n",		// 19200
			"AT+BAUD6\r\n",		// 38400
			"AT+BAUD7\r\n",		// 57600
			"AT+BAUD8\r\n"		// 115200
			};
	switch(baud){
		case 1200: str = baudrate[0]; break;
		case 2400: str = baudrate[1]; break;
		case 4800: str = baudrate[2]; break;
		case 9600: str = baudrate[3]; break;
		case 19200: str = baudrate[4]; break;
		case 38400: str = baudrate[5]; break;
		case 57600: str = baudrate[6]; break;
		case 115200: str = baudrate[7]; break;
		default: str = baudrate[5]; break;
	}
	return str;
}
const char* BT_AT_SetBaud1200(void) {
	return "AT+BAUD1\r\n"; // 0 ——1200
}
const char* BT_AT_SetBaud2400(void) {
	return "AT+BAUD2\r\n"; // 0 ——2400
}
const char* BT_AT_SetBaud4800(void) {
	return "AT+BAUD3\r\n"; // 0 ——4800
}
const char* BT_AT_SetBaud9600(void) {
	return "AT+BAUD4\r\n"; // 4 —— 9600
}
const char* BT_AT_SetBaud19200(void) {
	return "AT+BAUD5\r\n"; // 5 ——19200
}
const char* BT_AT_SetBaud38400(void) {
	return "AT+BAUD6\r\n"; // 6 ——38400
}
const char* BT_AT_SetBaud57600(void) {
	return "AT+BAUD7\r\n"; // 7 —— 57600
}
const char* BT_AT_SetBaud115200(void) {
	return "AT+BAUD8\r\n"; // 8 ——115200
}
/*** Inq Commands ***/
const char* BT_AT_SetInq(uint8_t inq) {
	const char* str = NULL;
	// Array of inq rate
	const char* inqnum[] = {
			"AT+INQ0\r\n",		// 0
			"AT+INQ1\r\n",		// 1
			};
	switch(inq){
		case 0: str = inqnum[0]; break;
		case 1: str = inqnum[1]; break;
	}
	return str;
}
const char* BT_AT_SetInq0(void) {
	return "AT+INQ0\r\n"; // 0: stop scanning
}
const char* BT_AT_SetInq1(void) {
	return "AT+INQ1\r\n"; // 1: start scanning
}
/*****************************************************/
/********************** TEST *************************/
/*****************************************************/
const char* BT_AT_Run(uint8_t inquiry) {
	const char* str;
	switch(inquiry){
		case 1:
			str = BT_AT_GetVersion();
		break;
		case 2:
			str = BT_AT_GetName();
		break;
		case 3:
			str = BT_AT_GetPin();
		break;
		case 4:
			str = BT_AT_GetStat();
		break;
		case 5:
			str = BT_AT_GetRole();
		break;
		case 6:
			str = BT_AT_GetAdvi();
		break;
		case 7:
			str = BT_AT_GetType();
		break;
		case 8:
			str = BT_AT_GetBaud();
		break;
		case 9:
			str = BT_AT_GetLaddr();
		break;
		case 10:
			str = BT_AT_GetUuid();
		break;
		case 11:
			str = BT_AT_GetChar();
		break;
		case 12:
			str = BT_AT_Test();
			//str = BT_AT_GetInq();

		break;
		case 13:
			//str = BT_AT_Test();
			//str = BT_AT_GetHelp();
			//str = BT_AT_SetName("BLE01");
			str = BT_AT_GetTest();
		break;
		default:
			str = BT_AT_Test();
		break;
	}
	return str;
}

/*** EOF ***/


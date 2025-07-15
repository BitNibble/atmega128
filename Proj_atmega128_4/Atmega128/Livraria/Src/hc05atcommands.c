/*************************************************************************
	HC05ATCOMMANDS
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ALL
Update:   29/12/2023
*************************************************************************/
/*** Library ***/
#include "hc05atcommands.h"
#include <string.h>

/*** Variable ***/
char HC05ATstr[32];

/*** Procedure and Function definition ***/
char* hc05at_cmd(uint16_t num){
	switch(num)
	{
		/*** Query ***/
		case 1: // all
			strcpy(HC05ATstr, "AT\r\n");
		break;
		case 2:
			strcpy(HC05ATstr, "AT+VERSION\r\n");
		break;
		case 3:
		//	strcpy(HC05ATstr, "AT+BAND\r\n"); // Param ? MAC address
		break;
		case 4:
			strcpy(HC05ATstr, "AT+UUID\r\n"); // Param: 0x0001~0xFFFE, default ? 0xFFE0
		break;
		case 5:
			strcpy(HC05ATstr, "AT+CHAR\r\n"); // Param:0x0001~0xFFFE, default ? 0xFFE1
		break;
		case 6:
			strcpy(HC05ATstr, "AT+LADDR\r\n");
		break;
		case 7:
			strcpy(HC05ATstr, "AT+ROLE\r\n"); // Param ?? 0-4 ?0 ——slave mode?, 1 ——master mode?, 2 ——sensor mode, 3 ——iBeacon mode, 4 ——WeChat mode, default ? 0
		break;
		case 8:
			strcpy(HC05ATstr, "AT+TYPE\r\n"); // 0——no password,1——password pairing,2——password pairing and binding, default ? 0
		break;
		case 9:
			strcpy(HC05ATstr, "AT+BAUD\r\n"); // 0 ——115200,1 ——57600,2 ——38400,3 ——19200,4 —— 9600, default ? 0
		break;
		case 10:
			strcpy(HC05ATstr, "AT+ADVI\r\n"); // 0 ——100ms,1 ——500ms,2 ——7500ms,3 ——1000ms,4 ——2000ms,5 ——4000ms,6 ——8000ms
		break;
		case 11:
			strcpy(HC05ATstr, "AT+NAME\r\n"); // Param ? module Bluetooth name Default: MLT-BT05 longest ? 18 byte
		break;
		case 12:
			strcpy(HC05ATstr, "AT+PIN\r\n"); // Param ? 6 byte bit pairing password default “? 123456”
		break;
		case 13:
			strcpy(HC05ATstr, "AT+INQ\r\n"); // Param ? 1-0, 1: start scanning, 0: stop scanning
		break;
		case 14:
			strcpy(HC05ATstr, "AT+CLRBAND\r\n"); // no
		break;
		case 15:
			strcpy(HC05ATstr, "AT+SLEEP\r\n"); // when PM=1.
		break;
		case 16: // After Reset do cmd 1.
			strcpy(HC05ATstr, "AT+RESET\r\n");
		break;
		/*** while connected ***/
		case 17: // Very usefull
			strcpy(HC05ATstr, "AT+GETSTAT\r\n"); // Param ? 0-1 , 0——not connected, 1——connected
		break;
		case 18:
			strcpy(HC05ATstr, "AT+DISC\r\n"); // Disconnect no
		break;
		/*******************/
		/****** SETUP ******/
		/*******************/
		case 71:
			strcpy(HC05ATstr, "AT+ROLE0\r\n"); // 0 ——slave mode
		break;
		case 72:
			strcpy(HC05ATstr, "AT+ROLE1\r\n"); // 1 ——master mode
		break;
		case 73:
			strcpy(HC05ATstr, "AT+ROLE2\r\n"); // 2 ——sensor mode
		break;
		case 74:
			strcpy(HC05ATstr, "AT+ROLE3\r\n"); // 3 ——iBeacon mode
		break;
		case 75:
			strcpy(HC05ATstr, "AT+ROLE4\r\n"); // 4 ——WeChat mode
		break;
		case 81:
			strcpy(HC05ATstr, "AT+TYPE0\r\n"); // 0——no password
		break;
		case 82:
			strcpy(HC05ATstr, "AT+TYPE1\r\n"); // 1——password pairing
		break;
		case 83:
			strcpy(HC05ATstr, "AT+TYPE2\r\n"); // 2——password pairing and binding
		break;
		case 91: // zs-040: works. 1200 hidden
		//	strcpy(HC05ATstr, "AT+BAUD1\r\n");
		break;
		case 92: // zs-040: works. 2400 hidden
		//	strcpy(HC05ATstr, "AT+BAUD2\r\n");
		break;
		case 93: // zs-040: works. 4800 hidden
		//	strcpy(HC05ATstr, "AT+BAUD3\r\n");
		break;
		case 94: // zs-040: works. 9600 hidden
		//	strcpy(HC05ATstr, "AT+BAUD4\r\n");
		break;
		case 95: // zs-040: works. 19200 hidden
		//	strcpy(HC05ATstr, "AT+BAUD5\r\n");
		break;
		case 96: // zs-040: works. 38400 hidden
			strcpy(HC05ATstr, "AT+BAUD6\r\n");
		break;
		case 97: // zs-040: works. 57600 hidden
		//	strcpy(HC05ATstr, "AT+BAUD7\r\n");
		break;
		case 98: // zs-040: works. 115200 hidden
		//	strcpy(HC05ATstr, "AT+BAUD8\r\n");
		break;
		case 101:
			strcpy(HC05ATstr, "AT+ADVI0\r\n"); // 0 ——100ms
		break;
		case 102:
			strcpy(HC05ATstr, "AT+ADVI1\r\n"); // 1 ——500ms
		break;
		case 103:
			strcpy(HC05ATstr, "AT+ADVI2\r\n"); // 2 ——7500ms
		break;
		case 104:
			strcpy(HC05ATstr, "AT+ADVI3\r\n"); // 3 ——1000ms
		break;
		case 105:
			strcpy(HC05ATstr, "AT+ADVI4\r\n"); // 4 ——2000ms
		break;
		case 106:
			strcpy(HC05ATstr, "AT+ADVI5\r\n"); // 5 ——4000ms
		break;
		case 107:
			strcpy(HC05ATstr, "AT+ADVI6\r\n"); // 6 ——8000ms
		break;
		case 111:
			strcpy(HC05ATstr, "AT+NAMEBT05\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 112:
			strcpy(HC05ATstr, "AT+NAMEHome1\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 113:
			strcpy(HC05ATstr, "AT+NAMEHome2\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 114:
			strcpy(HC05ATstr, "AT+NAMEHome3\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 115:
			strcpy(HC05ATstr, "AT+NAMEHome4\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 121:
			strcpy(HC05ATstr, "AT+PIN123456\r\n");
		break;
		case 122:
			strcpy(HC05ATstr, "AT+PIN654321\r\n");
		break;
		case 123:
			strcpy(HC05ATstr, "AT+PIN916919\r\n");
		break;
		case 131:
			strcpy(HC05ATstr, "AT+INQ0\r\n"); // 0: stop scanning
		break;
		case 132:
			strcpy(HC05ATstr, "AT+INQ1\r\n"); // 1: start scanning
		break;
		case 19:
			strcpy(HC05ATstr, "AT+IBSUUID\r\n"); // Param ?FDA50693A4E24FB1AFCFC6EB07647825
		break;
		case 20:
			strcpy(HC05ATstr, "AT+MAJOR\r\n"); // Param : 0x0001~0xFFFE, default ? 0x0007
		break;
		case 21:
			strcpy(HC05ATstr, "AT+MINOR\r\n"); // Param: 0x0001~0xFFFE, default ? 0x000A
		break;
		case 22:
		//	strcpy(HC05ATstr, "AT+CONN<Param>\r\n"); // Param ? 0-7
		break;
		case 23:
		//	strcpy(HC05ATstr, "AT+RSLV< Param >\r\n"); // Param ? ( 0-7 )
		break;
		case 24:
		//	strcpy(HC05ATstr, "AT+GETDCN<Param>\r\n"); // Param ?? 1-8 ?
		break;
		case 25:
		//	strcpy(HC05ATstr, "AT+AT_WXSWH<Param>\r\n"); // Param ?? 0-1), 0 ? H5 communication, 1 ? factory server, Default: 0
		break;
		/*** Test ***/
		
		/*** Preamble ***/
		case 26: // hidden
		//	strcpy(HC05ATstr, "AT+DEFAULT\r\n"); // 4dbm, 9600bps (115200bps), 200ms, PM=0, On board LED 2:1, External indicator led1: 2
		break;
		case 27:
			strcpy(HC05ATstr, "AT+HELP\r\n"); // Lists commands
		break;
		/******/
		default:
			strcpy(HC05ATstr, "AT\r\n");
		break;
	};
	return HC05ATstr;
}

/*** EOF ***/


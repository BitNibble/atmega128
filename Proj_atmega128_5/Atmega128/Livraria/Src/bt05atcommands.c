/*************************************************************************
	BT05ATCOMMANDS
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ALL
Date:     24042023
*************************************************************************/
/*** Library ***/
#include "bt05atcommands.h"
#include <string.h>

/*** Variable ***/
char BT05ATstr[72];

/*** Procedure and Function definition ***/
char* bt05at_cmd(uint16_t num){
	switch(num)
	{
		case 1:
			strcpy(BT05ATstr, "AT\r\n");
		break;
		case 2:
			strcpy(BT05ATstr, "AT+VERSION\r\n");
		break;
		case 3:
			strcpy(BT05ATstr, "AT+NAME\r\n"); // Param ? module Bluetooth name Default: MLT-BT05 longest ? 18 byte
		break;
		case 4:
			strcpy(BT05ATstr, "AT+PIN\r\n"); // Param ? 6 byte bit pairing password default “? 123456”
		break;
		case 5: // Very usefull
			strcpy(BT05ATstr, "AT+GETSTAT\r\n"); // Param ? 0-1 , 0——not connected, 1——connected
		break;
		case 10:
			strcpy(BT05ATstr, "AT+ROLE\r\n"); // Param ?? 0-4 ?0 ——slave mode?, 1 ——master mode?, 2 ——sensor mode, 3 ——iBeacon mode, 4 ——WeChat mode, default ? 0
		break;
		case 101:
			strcpy(BT05ATstr, "AT+ROLE0\r\n"); // 0 ——slave mode
		break;
		case 102:
			strcpy(BT05ATstr, "AT+ROLE1\r\n"); // 1 ——master mode
		break;
		case 103:
			strcpy(BT05ATstr, "AT+ROLE2\r\n"); // 2 ——sensor mode
		break;
		case 104:
			strcpy(BT05ATstr, "AT+ROLE3\r\n"); // 3 ——iBeacon mode
		break;
		case 105:
			strcpy(BT05ATstr, "AT+ROLE4\r\n"); // 4 ——WeChat mode
		case 11:
			strcpy(BT05ATstr, "AT+LADDR\r\n");
		break;
		case 12:
			strcpy(BT05ATstr, "AT+ADVI\r\n"); // 0 ——100ms,1 ——500ms,2 ——7500ms,3 ——1000ms,4 ——2000ms,5 ——4000ms,6 ——8000ms
		break;
		case 121:
			strcpy(BT05ATstr, "AT+ADVI0\r\n"); // 0 ——100ms
		break;
		case 122:
			strcpy(BT05ATstr, "AT+ADVI1\r\n"); // 1 ——500ms
		break;
		case 123:
			strcpy(BT05ATstr, "AT+ADVI2\r\n"); // 2 ——7500ms
		break;
		case 124:
			strcpy(BT05ATstr, "AT+ADVI3\r\n"); // 3 ——1000ms
		break;
		case 125:
			strcpy(BT05ATstr, "AT+ADVI4\r\n"); // 4 ——2000ms
		break;
		case 126:
			strcpy(BT05ATstr, "AT+ADVI5\r\n"); // 5 ——4000ms
		break;
		case 127:
			strcpy(BT05ATstr, "AT+ADVI6\r\n"); // 6 ——8000ms
		break;
		case 13:
			strcpy(BT05ATstr, "AT+TYPE\r\n"); // 0——no password,1——password pairing,2——password pairing and binding, default ? 0
		break;
		case 131:
			strcpy(BT05ATstr, "AT+TYPE0\r\n"); // 0——no password
		break;
		case 132:
			strcpy(BT05ATstr, "AT+TYPE1\r\n"); // 1——password pairing
		break;
		case 133:
			strcpy(BT05ATstr, "AT+TYPE2\r\n"); // 2——password pairing and binding
		break;
		case 14:
			strcpy(BT05ATstr, "AT+BAUD\r\n"); // 0 ——115200,1 ——57600,2 ——38400,3 ——19200,4 —— 9600, default ? 0
		break;
		case 141: // zs-040: works. 1200 hidden
			//strcpy(BT05ATstr, "AT+BAUD1\r\n");
		break;
		case 142: // zs-040: works. 2400 hidden
			//strcpy(BT05ATstr, "AT+BAUD2\r\n");
		break;
		case 143: // zs-040: works. 4800 hidden
			//strcpy(BT05ATstr, "AT+BAUD3\r\n");
		break;
		case 144: // zs-040: works. 9600 hidden
			//strcpy(BT05ATstr, "AT+BAUD4\r\n");
		break;
		case 145: // zs-040: works. 19200 hidden
			//strcpy(BT05ATstr, "AT+BAUD5\r\n");
		break;
		case 146: // zs-040: works. 38400 hidden
			strcpy(BT05ATstr, "AT+BAUD6\r\n");
		break;
		case 147: // zs-040: works. 57600 hidden
			//strcpy(BT05ATstr, "AT+BAUD7\r\n");
		break;
		case 148: // zs-040: works. 115200 hidden
			//strcpy(BT05ATstr, "AT+BAUD8\r\n");
		break;
		case 15:
			strcpy(BT05ATstr, "AT+UUID\r\n"); // Param: 0x0001~0xFFFE, default ? 0xFFE0
		break;
		case 16:
			strcpy(BT05ATstr, "AT+CHAR\r\n"); // Param:0x0001~0xFFFE, default ? 0xFFE1
		break;
		case 17:
			strcpy(BT05ATstr, "AT+IBSUUID\r\n"); // Param ?FDA50693A4E24FB1AFCFC6EB07647825
		break;
		case 18:
			strcpy(BT05ATstr, "AT+MAJOR\r\n"); // Param : 0x0001~0xFFFE, default ? 0x0007
		break;
		case 19:
			strcpy(BT05ATstr, "AT+MINOR\r\n"); // Param: 0x0001~0xFFFE, default ? 0x000A
		break;
		case 20:
			strcpy(BT05ATstr, "AT+SLEEP\r\n"); // when PM=1.
		break;
		case 21:
			strcpy(BT05ATstr, "AT+INQ\r\n"); // Param ? 1-0, 1: start scanning, 0: stop scanning
		break;
		case 211:
			strcpy(BT05ATstr, "AT+INQ0\r\n"); // 0: stop scanning
		break;
		case 212:
			strcpy(BT05ATstr, "AT+INQ1\r\n"); // 1: start scanning
		break;
		case 22:
			//strcpy(BT05ATstr, "AT+CONN<Param>\r\n"); // Param ? 0-7
		break;
		case 23:
			//strcpy(BT05ATstr, "AT+RSLV< Param >\r\n"); // Param ? ( 0-7 )
		break;
		case 24:
			strcpy(BT05ATstr, "AT+BAND\r\n"); // Param ? MAC address
		break;
		case 25:
			strcpy(BT05ATstr, "AT+CLRBAND\r\n"); // no
		break;
		case 26:
			//strcpy(BT05ATstr, "AT+GETDCN<Param>\r\n"); // Param ?? 1-8 ?
		break;
		case 27:
			strcpy(BT05ATstr, "AT+DISC\r\n"); // Disconnect no
		break;
		case 28:
			//strcpy(BT05ATstr, "AT+AT_WXSWH<Param>\r\n"); // Param ?? 0-1), 0 ? H5 communication, 1 ? factory server, Default: 0
		break;
		case 29:
			strcpy(BT05ATstr, "AT+RESET\r\n");
		break;
		case 30: // hiden
			//strcpy(BT05ATstr, "AT+DEFAULT\r\n"); // 4dbm, 9600bps (115200bps), 200ms, PM=0, Onboard LED 2:1, External indicator led1: 2
		break;
		case 31:
			strcpy(BT05ATstr, "AT+HELP\r\n"); // Lists commands
		break;
		case 801:
			strcpy(BT05ATstr, "AT+PIN123456\r\n");
		break;
		case 802:
			strcpy(BT05ATstr, "AT+PIN654321\r\n");
		break;
		case 803:
			strcpy(BT05ATstr, "AT+PIN916919\r\n");
		break;
		case 901:
			strcpy(BT05ATstr, "AT+NAMEBT05\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 902:
			strcpy(BT05ATstr, "AT+NAMEHome1\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 903:
			strcpy(BT05ATstr, "AT+NAMEHome2\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 904:
			strcpy(BT05ATstr, "AT+NAMEHome3\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		case 905:
			strcpy(BT05ATstr, "AT+NAMEHome4\r\n"); // Param ? module Bluetooth name Default: BT05 longest ? 18 byte
		break;
		/******/
		default:
			strcpy(BT05ATstr, "AT\r\n");
		break;
	};
	return BT05ATstr;
}

/*** EOF ***/


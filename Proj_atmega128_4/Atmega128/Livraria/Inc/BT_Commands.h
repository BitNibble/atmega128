/******************************************************************************
	BT AT COMMANDS
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ALL
Date:     20102024
Comment:
	BT05 and ZS-040
*******************************************************************************/
#ifndef _BT_COMMANDS_H_
	#define _BT_COMMANDS_H_

#include <stdint.h>
#include <inttypes.h>

#define WIFI_CONNECT 0
#define START_SEVER 23
#define SERVER_SEND 26

/*****************************************************/
/*** USER ***/
const char* BT_AT_NAME_1(void);
const char* BT_AT_NAME_2(void);
const char* BT_AT_PIN_1(void);
/*****************************************************/
/******** General Inquiry Function AT Commands *******/
/*****************************************************/
const char* BT_AT_Run(uint8_t inquiry);
const char* BT_AT_Test(void);
const char* BT_AT_GetVersion(void);
const char* BT_AT_GetName(void);
const char* BT_AT_GetPin(void);
const char* BT_AT_GetStat(void);
const char* BT_AT_GetRole(void);
const char* BT_AT_GetAdvi(void);
const char* BT_AT_GetType(void);
const char* BT_AT_GetBaud(void);
const char* BT_AT_GetLaddr(void);
const char* BT_AT_GetUuid(void);
const char* BT_AT_GetChar(void);
const char* BT_AT_GetInq(void);
const char* BT_AT_GetTest(void);
/*****************************************************/
/*****************************************************/
/*****************************************************/
const char* BT_AT_EnterSleep(void);
const char* BT_AT_Reset(void);
const char* BT_AT_RestoreDefaults(void);
const char* BT_AT_Disconnect(void);
const char* BT_AT_GetHelp(void);
/*****************************************************/
/******** General Setting Function AT Commands *******/
/*****************************************************/
/*** Name Command ***/
const char* BT_AT_SetName(const char *name);
/*** Pin Command ***/
const char* BT_AT_SetPin(uint32_t pin);
/*** Role Commands ***/
const char* BT_AT_SetRole(uint8_t role);
const char* BT_AT_SetRole0(void);
const char* BT_AT_SetRole1(void);
const char* BT_AT_SetRole2(void);
const char* BT_AT_SetRole3(void);
const char* BT_AT_SetRole4(void);
/*** Advi Commands ***/
const char* BT_AT_SetAdvi(uint8_t advi);
const char* BT_AT_SetAdvi0(void);
const char* BT_AT_SetAdvi1(void);
const char* BT_AT_SetAdvi2(void);
const char* BT_AT_SetAdvi3(void);
const char* BT_AT_SetAdvi4(void);
const char* BT_AT_SetAdvi5(void);
const char* BT_AT_SetAdvi6(void);
/*** Type Commands ***/
const char* BT_AT_SetType(uint8_t type);
const char* BT_AT_SetType0(void);
const char* BT_AT_SetType1(void);
const char* BT_AT_SetType2(void);
/*** Baud Rate Commands ***/
const char* BT_AT_SetBaud(uint32_t baud);
const char* BT_AT_SetBaud1200(void);
const char* BT_AT_SetBaud2400(void);
const char* BT_AT_SetBaud4800(void);
const char* BT_AT_SetBaud9600(void);
const char* BT_AT_SetBaud19200(void);
const char* BT_AT_SetBaud38400(void);
const char* BT_AT_SetBaud57600(void);
const char* BT_AT_SetBaud115200(void);
/*** Inq Commands ***/
const char* BT_AT_SetInq(uint8_t inq);
const char* BT_AT_SetInq0(void);
const char* BT_AT_SetInq1(void);

#endif

/*** EOF ***/


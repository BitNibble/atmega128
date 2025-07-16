/************************************************************************
	ATMEGA128USART1
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     07/01/2024
************************************************************************/
#ifndef _ATMEGA128USART1_H_
	#define _ATMEGA128USART1_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant & Macro ***/
#ifndef UART1_RX_BUFFER_SIZE
	#define UART1_RX_BUFFER_SIZE 32
#endif

#define UARTvar char

#if ( ( UART1_RX_BUFFER_SIZE ) >= ( RAMEND - 0x60 ) )
	#error "size of UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE larger than size of SRAM"
#endif

#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
	#define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
	#define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#else
	#error "Not Atmega 128"
#endif

#define NONE 0
#define EVEN 2
#define ODD 3
#define UART_FRAME_ERROR		0x0800              /* Framing Error by UART        */
#define UART_OVERRUN_ERROR		0x0400              /* Overrun condition by UART    */
#define UART_BUFFER_OVERFLOW	0x0200              /* receive ring buffer overflow */
#define UART_NO_DATA			0x0100              /* no receive data available    */

/*** Handler ***/
typedef struct{
	// V-table
	UARTvar (*read)(void);
	UARTvar (*getch)(void);
	UARTvar* (*gets)(void);
	void (*rxflush)(void);
	void (*write)(UARTvar data);
	void (*putch)(UARTvar c);
	void (*puts)(UARTvar* s);
}USART1_Handler;

void usart1_enable( uint32_t baud, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
USART1_Handler* usart1(void);

/*** Procedure and Function declaration ***/
char* usart1_messageprint(USART1_Handler* uart, char* oneshot, char* msg, const char* endl);

#endif
/*** EOF ***/


/*************************************************************************
	ATMEGA128USART0
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     26/06/2025
*************************************************************************/
/*** Library ***/
#include "atmega128usart0.h"
#include "buffer.h"
#include <avr/interrupt.h>
#include <string.h>

/*** Variable ***/
static BUFF_Handler rx0buff;
static UARTvar UART0_Rx;
static UARTvar UART0_RxBuf[UART0_RX_BUFFER_SIZE] = {0};
static const uint16_t uart0_rx_buffer_size = UART0_RX_BUFFER_SIZE - 1;
static uint8_t UART0_LastRxError;
static uint8_t uart0flag;

/*** Procedure and Function declaration ***/
UARTvar uart0_read(void);
UARTvar uart0_getch(void);
UARTvar* uart0_gets(void);
void uart0_rxflush(void);
void uart0_write(UARTvar data);
void uart0_putch(UARTvar c);
void uart0_puts(UARTvar* s);
/*** Auxiliar ***/
uint8_t USART0ReceiveComplete(void);
uint8_t USART0TransmitComplete(void);
uint8_t USART0DataRegisterEmpty(void);
uint8_t USART0FrameError(void);
uint8_t USART0DataOverRun(void);
uint8_t USART0ParityError(void);
uint8_t USART0ReadErrors(void);
void USART0ClearErrors(void);
void USART0DoubleTransmissionSpeed(void);

/*** Internal State ***/
static USART0_Handler atmega128_usart0 = {
	// V-table
	.read = uart0_read,
	.getch = uart0_getch,
	.gets = uart0_gets,
	.rxflush = uart0_rxflush,
	.write = uart0_write,
	.putch = uart0_putch,
	.puts = uart0_puts
};

/*** Handler ***/
void usart0_enable(uint32_t baud, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity )
{
	uart0flag = 1;
	uint16_t ubrr = 0;
	rx0buff = buff_enable( uart0_rx_buffer_size, UART0_RxBuf );
	ubrr = BAUDRATEnormal(baud);
	// Set baud rate
	if ( ubrr & 0x8000 ) // The transfer rate can be doubled by setting the U2X bit in UCSRA.
	{
   		USART0DoubleTransmissionSpeed(); // Enable 2x speed 
   		ubrr = BAUDRATEdouble(baud);
   	}
	usart0_reg()->ubrr0h.var = writeHLbyte(ubrr).par.h.var;
	usart0_reg()->ubrr0l.var = writeHLbyte(ubrr).par.l.var;
	// Enable USART receiver and transmitter and receive complete interrupt
	usart0_reg()->ucsr0b.var = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	#ifdef URSEL0 // Set frame format: asynchronous, 8 data, no parity, 1 stop bit
		usart0_reg()->ucsr0c = (1 << UMSEL0) | (3 << UCSZ00);
	#else
		switch(FDbits){
			case 9:
				usart0_reg()->ucsr0b.var |= (1 << UCSZ02);
				usart0_reg()->ucsr0c.var |= (3 << UCSZ00);
			break;
			case 8:
				usart0_reg()->ucsr0b.var &= ~(1 << UCSZ02);
				usart0_reg()->ucsr0c.var |= (3 << UCSZ00);
			break;
			case 7:
				usart0_reg()->ucsr0b.var &= ~(1 << UCSZ02);
				usart0_reg()->ucsr0c.var |= (1 << UCSZ01);
				usart0_reg()->ucsr0c.var &= ~(1 << UCSZ00);
			break;
			case 6:
				usart0_reg()->ucsr0b.var &= ~(1 << UCSZ02);
				usart0_reg()->ucsr0c.var &= ~(1 << UCSZ01);
				usart0_reg()->ucsr0c.var |= (1 << UCSZ00);
			break;
			case 5:
				usart0_reg()->ucsr0b.var &= ~(1 << UCSZ02);
				usart0_reg()->ucsr0c.var &= ~(3 << UCSZ00);
			break;
			default:
				usart0_reg()->ucsr0b.var &= ~(1 << UCSZ02);
				usart0_reg()->ucsr0c.var |= (3 << UCSZ00);
			break;
		}
		switch(Stopbits){
			case 1:
				usart0_reg()->ucsr0c.var &= ~(1 << USBS0);
			break;
			case 2:
				usart0_reg()->ucsr0c.var |= (1 << USBS0);
			break;	
			default:
				usart0_reg()->ucsr0c.var &= ~(1 << USBS0);
			break;
		}
		switch(Parity){
			case 0:
				usart0_reg()->ucsr0c.var &= ~(3 << UPM00);
			break;
			case 2:
				usart0_reg()->ucsr0c.var |= (1 << UPM01);
				usart0_reg()->ucsr0c.var &= ~(1 << UPM00);
			break;
			case 3:
				usart0_reg()->ucsr0c.var |= (3 << UPM00);
			break;	
			default:
				usart0_reg()->ucsr0c.var &= ~(3 << UPM00);
			break;
		}
	#endif
	cpu_reg()->sreg.par.i = 1;
}

USART0_Handler* usart0(void){ return &atmega128_usart0; }

/*** Procedure and Function definition ***/
UARTvar uart0_read(void)
{
	UARTvar c;
	c = UART0_Rx;
	UART0_Rx = 0;
	return c;
}
UARTvar uart0_getch(void)
{
	return uart0_read();
}
UARTvar* uart0_gets(void)
{
	return rx0buff.raw(&rx0buff.par);
}
void uart0_rxflush(void)
{
	rx0buff.flush(&rx0buff.par);
	UART0_LastRxError = 0;
}
void uart0_write(UARTvar data)
{
	uint16_t timeout;
	usart0_reg()->ucsr0b.var |= 1 << UDRIE0;
	usart0_reg()->udr0.var = data;
	for( timeout = 600; !USART0DataRegisterEmpty() && timeout; timeout-- ); // minimum -> +/- 450
	//for( ; !USART0DataRegisterEmpty(); ); // without timeout
}
void uart0_putch(UARTvar c)
{
	uart0_write(c);
}
void uart0_puts(UARTvar* s)
{
	char tmp;
	while(*s){
		tmp = *(s++);
		uart0_putch(tmp);
	}
}
/*** Complimentary functions ***/
char* usart0_messageprint(USART0_Handler* uart, char* oneshot, char* msg, const char* endl)
{
	char* ptr;
	uint8_t length;
	uint8_t endlength = strlen(endl);
	if(uart0flag){ *oneshot = 0; uart0flag = 0; uart->rxflush();} // the matrix
	ptr = uart->gets();
	length = strlen(ptr);
	if(length >= endlength){
		if( !strcmp( ptr+(length-endlength), endl ) ){ strcpy(oneshot, ptr); strcpy(msg, ptr); uart0flag = 0xFF; }
		// default
		//else if( !strcmp( ptr+(length-endlength), "\r" ) ){ strcpy(oneshot, ptr); strcpy(msg, ptr); uart0flag = 0xFF; }
		//else if( !strcmp( ptr+(length-endlength), "\n" ) ){ strcpy(oneshot, ptr); strcpy(msg, ptr); uart0flag = 0xFF; }
	}
	return ptr;
}
/*** Auxiliar ***/
uint8_t USART0ReceiveComplete(void)
{
	return (UCSR0A & (1 << RXC0));
}
uint8_t USART0TransmitComplete(void)
{
	return (UCSR0A & (1 << TXC0));
}
uint8_t USART0DataRegisterEmpty(void)
{
	return (UCSR0A & (1 << UDRE0));
}
uint8_t USART0FrameError(void)
{
	return (UCSR0A & (1 << FE0));
}
uint8_t USART0DataOverRun(void)
{
	return (UCSR0A & (1 << DOR0));
}
uint8_t USART0ParityError(void)
{
	return (UCSR0A & (1 << FE0));
}
uint8_t USART0ReadErrors(void)
{
	return get_reg_block(UCSR0A,3,2);
}
void USART0ClearErrors(void)
{
	set_reg_block(&UCSR0A,3,2,0);
}
void USART0DoubleTransmissionSpeed(void)
{
	set_reg_block(&UCSR0A,4,1,1);
}
/*** Interrupt ***/
ISR(UART0_RECEIVE_INTERRUPT)
{
	unsigned char bit9;
	unsigned char usr;
	
	usr  = USART0ReadErrors();
	bit9 = usart0_reg()->ucsr0b.var;
	bit9 = 0x01 & (bit9 >> 1);
	
	if(usr){ UART0_LastRxError = usr; }
	
	UART0_Rx = usart0_reg()->udr0.var;
	rx0buff.push(&rx0buff.par, UART0_Rx);
}
ISR(UART0_TRANSMIT_INTERRUPT)
{
	usart0_reg()->ucsr0b.var &= ~(1 << UDRIE0);
}

/*** EOF ***/


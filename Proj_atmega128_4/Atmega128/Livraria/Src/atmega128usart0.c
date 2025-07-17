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
static BUFF_Handler USART0_rxbuff;
static UARTvar USART0_rx;
static UARTvar USART0_rxbuf[UART0_RX_BUFFER_SIZE] = {0};
static const uint16_t USART0_rx_buffer_size = UART0_RX_BUFFER_SIZE - 1;
static uint8_t USART0_lastrxerror;
static uint8_t USART0_flag;

/*** Procedure and Function declaration ***/
UARTvar USART0_read(void);
UARTvar USART0_getch(void);
UARTvar* USART0_gets(void);
void USART0_rxflush(void);
void USART0_write(UARTvar data);
void USART0_putch(UARTvar c);
void USART0_puts(UARTvar* s);
/*** Auxiliar ***/
uint8_t USART0_receivecomplete(void);
uint8_t USART0_transmitcomplete(void);
uint8_t USART0_dataregisterempty(void);
uint8_t USART0_frameerror(void);
uint8_t USART0_dataoverrun(void);
uint8_t USART0_parityerror(void);
uint8_t USART0_readerrors(void);
void USART0_clearerrors(void);
void USART0_doubletransmissionspeed(void);

/*** Internal State ***/
static USART0_Handler atmega128_usart0 = {
	// V-table
	.read = USART0_read,
	.getch = USART0_getch,
	.gets = USART0_gets,
	.rxflush = USART0_rxflush,
	.write = USART0_write,
	.putch = USART0_putch,
	.puts = USART0_puts
};

/*** Handler ***/
void usart0_enable(uint32_t baud, unsigned int fdbits, unsigned int stopbits, unsigned int parity)
{
	USART0_flag = 1;
	uint16_t ubrr = 0;
	USART0_rxbuff = buff_enable(UART0_RX_BUFFER_SIZE, USART0_rxbuf);
	ubrr = BAUDRATEnormal(baud);
	// Set baud rate
	if (ubrr & 0x8000) // The transfer rate can be doubled by setting the U2X bit in UCSRA.
	{
   		USART0_doubletransmissionspeed(); // Enable 2x speed 
   		ubrr = BAUDRATEdouble(baud);
   	}
	usart0_reg()->ubrr0h.var = writeHLbyte(ubrr).par.h.var;
	usart0_reg()->ubrr0l.var = writeHLbyte(ubrr).par.l.var;
	// Enable USART receiver and transmitter and receive complete interrupt
	usart0_reg()->ucsr0b.var = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	#ifdef URSEL0 // Set frame format: asynchronous, 8 data, no parity, 1 stop bit
		usart0_reg()->ucsr0c = (1 << UMSEL0) | (3 << UCSZ00);
	#else
		switch(fdbits){
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
		switch(stopbits){
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
		switch(parity){
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
UARTvar USART0_read(void)
{
	UARTvar c;
	c = USART0_rx;
	USART0_rx = 0;
	return c;
}
UARTvar USART0_getch(void)
{
	return USART0_read();
}
UARTvar* USART0_gets(void)
{
	return USART0_rxbuff.raw(&USART0_rxbuff.par);
}
void USART0_rxflush(void)
{
	USART0_rxbuff.flush(&USART0_rxbuff.par);
	USART0_lastrxerror = 0;
}
void USART0_write(UARTvar data)
{
	uint16_t timeout;
	usart0_reg()->ucsr0b.var |= 1 << UDRIE0;
	usart0_reg()->udr0.var = data;
	for(timeout = 600; !USART0_dataregisterempty() && timeout; timeout--);
}
void USART0_putch(UARTvar c)
{
	USART0_write(c);
}
void USART0_puts(UARTvar* s)
{
	char tmp;
	while(*s){
		tmp = *(s++);
		USART0_putch(tmp);
	}
}
/*** Complimentary functions ***/
char* USART0_messageprint(USART0_Handler* uart, char* oneshot, char* msg, const char* endl)
{
	char* ptr;
	uint8_t length;
	uint8_t endlength = strlen(endl);
	if(USART0_flag){ *oneshot = 0; USART0_flag = 0; uart->rxflush();} // the matrix
	ptr = uart->gets();
	length = strlen(ptr);
	if(length >= endlength){
		if( !strcmp(ptr+(length-endlength), endl) ){
			strcpy(oneshot, ptr);
			strcpy(msg, ptr);
			USART0_flag = 0xFF;
		}
	}
	return ptr;
}
/*** Auxiliar ***/
uint8_t USART0_receivecomplete(void)
{
	return (UCSR0A & (1 << RXC0));
}
uint8_t USART0_transmitcomplete(void)
{
	return (UCSR0A & (1 << TXC0));
}
uint8_t USART0_dataregisterempty(void)
{
	return (UCSR0A & (1 << UDRE0));
}
uint8_t USART0_frameerror(void)
{
	return (UCSR0A & (1 << FE0));
}
uint8_t USART0_dataoverrun(void)
{
	return (UCSR0A & (1 << DOR0));
}
uint8_t USART0_parityerror(void)
{
	return (UCSR0A & (1 << FE0));
}
uint8_t USART0_readerrors(void)
{
	return get_reg_block(UCSR0A,3,2);
}
void USART0_clearerrors(void)
{
	set_reg_block(&UCSR0A,3,2,0);
}
void USART0_doubletransmissionspeed(void)
{
	set_reg_block(&UCSR0A,4,1,1);
}
/*** Interrupt ***/
ISR(UART0_RECEIVE_INTERRUPT)
{
	unsigned char bit9;
	unsigned char usr;
	
	usr  = USART0_readerrors();
	bit9 = usart0_reg()->ucsr0b.var;
	bit9 = 0x01 & (bit9 >> 1);
	
	if(usr){ USART0_lastrxerror = usr; }
	
	USART0_rx = usart0_reg()->udr0.var;
	USART0_rxbuff.push(&USART0_rxbuff.par, USART0_rx);
}
ISR(UART0_TRANSMIT_INTERRUPT)
{
	usart0_reg()->ucsr0b.var &= ~(1 << UDRIE0);
}

/*** EOF ***/

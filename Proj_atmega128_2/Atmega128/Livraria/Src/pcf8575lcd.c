/*************************************************************************
	PCF8575LCD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     11042024                
************************************************************************/
/*** Library ***/
#include "pcf8575lcd.h"
#include "pcf8575.h"
#include <util/delay.h>

/*** Constant & Macro **/
#define PCF_INST 0
#define PCF_DATA 1
// ticks depends on CPU frequency 16Mhz -> 0 8Mhz -> 0
#define PCF_LCD_W_N_TICKS 0
#define PCF_LCD_R_N_TICKS 0

/*** Variable ***/
static PCF8575_LCD0 pcf8575_setup_lcd0;
static PCF8575 pcf8575_lcd0_com;
static uint16_t pcf8575_lcd0_detect;

/*** Procedure and Function declaration ***/
void PCF8575_LCD0_inic(void);
void PCF8575_LCD0_write(char c, unsigned short D_I);
char PCF8575_LCD0_read(unsigned short D_I);
void PCF8575_LCD0_BF(void);
void PCF8575_LCD0_putch(char c);
char PCF8575_LCD0_getch(void);
void PCF8575_LCD0_string(const char* s); // RAW
void PCF8575_LCD0_string_size(const char* s, uint8_t size); // RAW
void PCF8575_LCD0_hspace(uint8_t n);
void PCF8575_LCD0_clear(void);
void PCF8575_LCD0_gotoxy(unsigned int y, unsigned int x);
void PCF8575_LCD0_reboot(void);
void PCF8575_LCD_ticks(uint16_t num);

/*** Handler ***/
PCF8575_LCD0 pcf8575_lcd0_enable( uint8_t pcf8575_id, uint8_t twi_prescaler )
{
	// LOCAL VARIABLES
	// ALLOCAÇÂO MEMORIA PARA Estrutura
	//PCF8575_LCD0 pcf8575_setup_lcd0;
	pcf8575_lcd0_com = pcf8575_enable( pcf8575_id, twi_prescaler );
	
	// initialize variables
	pcf8575_lcd0_detect = pcf8575_lcd0_com.readhbits( &pcf8575_lcd0_com.par, ( 1 << PCF_NC) );
	// V-table
	pcf8575_setup_lcd0.write = PCF8575_LCD0_write;
	pcf8575_setup_lcd0.read = PCF8575_LCD0_read;
	pcf8575_setup_lcd0.BF = PCF8575_LCD0_BF;
	pcf8575_setup_lcd0.putch = PCF8575_LCD0_putch;
	pcf8575_setup_lcd0.getch = PCF8575_LCD0_getch;
	pcf8575_setup_lcd0.string = PCF8575_LCD0_string; // RAW
	pcf8575_setup_lcd0.string_size = PCF8575_LCD0_string_size; // RAW
	pcf8575_setup_lcd0.hspace = PCF8575_LCD0_hspace;
	pcf8575_setup_lcd0.clear = PCF8575_LCD0_clear;
	pcf8575_setup_lcd0.gotoxy = PCF8575_LCD0_gotoxy;
	pcf8575_setup_lcd0.reboot = PCF8575_LCD0_reboot;
	// LCD INIC
	PCF8575_LCD0_inic();
	
	return pcf8575_setup_lcd0;
}

PCF8575_LCD0* pcf8575_lcd0(void){ return &pcf8575_setup_lcd0; }

/*** Procedure and Function definition ***/
void PCF8575_LCD0_inic(void)
{
	uint16_t mask;
	mask = ((1 << PCF_DB0) | (1 << PCF_DB1) | (1 << PCF_DB2) | (1 << PCF_DB3) | (1 << PCF_DB4) | (1 << PCF_DB5) | (1 << PCF_DB6) | (1 << PCF_DB7));
	// LCD INIC
	pcf8575_lcd0_com.writehbits( &pcf8575_lcd0_com.par, mask, 1 ); // mcu as input
	pcf8575_lcd0_com.writehbits( &pcf8575_lcd0_com.par, ((1 << PCF_RS) | (1 << PCF_RW) | (1 << PCF_EN)) , 0 ); // lcd as input
	// INICIALIZACAO LCD datasheet/
	_delay_ms(40); // using clock at 16Mhz
	PCF8575_LCD0_write(0x38, PCF_INST); // function set 0x38
	_delay_us(39);
	PCF8575_LCD0_write(0x38, PCF_INST); // function set 0x38
	_delay_us(37);
	PCF8575_LCD0_write(0x0C, PCF_INST);// display on/off control 0x0C
	_delay_us(37);
	PCF8575_LCD0_write(0x01, PCF_INST);// clear display 0x01
	_delay_ms(1.53);
	PCF8575_LCD0_write(0x06, PCF_INST);// entry mode set (crazy settings) 0x06
	PCF8575_LCD0_BF();
	//PCF8575_LCD0_write(0x06, PCF_INST);// entry mode set (crazy settings) 0x06
	//PCF8575_LCD0_BF();
	//}
	// INICIALIZATION END
	// PCF8575_LCD0_write(0x1F, PCF_INST);// cursor or display shift
	// _delay_us(39);
	// PCF8575_LCD0_write(0x03, PCF_INST);// return home
	// _delay_ms(1.53);
	PCF8575_LCD0_gotoxy(0,0);
}
void PCF8575_LCD0_write(char c, unsigned short D_I)
{
	uint16_t mask, data;
	mask = ((1 << PCF_DB0) | (1 << PCF_DB1) | (1 << PCF_DB2) | (1 << PCF_DB3) | (1 << PCF_DB4) | (1 << PCF_DB5) | (1 << PCF_DB6) | (1 << PCF_DB7));
	data = 0;
	
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_RW), 0);
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, mask, 0);
	
	if(D_I) pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_RS) , 1); else pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_RS), 0);
	
	if(c & 0x80) data |= 1 << PCF_DB7; else data &= ~(1 << PCF_DB7);
	if(c & 0x40) data |= 1 << PCF_DB6; else data &= ~(1 << PCF_DB6);
	if(c & 0x20) data |= 1 << PCF_DB5; else data &= ~(1 << PCF_DB5);
	if(c & 0x10) data |= 1 << PCF_DB4; else data &= ~(1 << PCF_DB4);
	if(c & 0x08) data |= 1 << PCF_DB3; else data &= ~(1 << PCF_DB3);
	if(c & 0x04) data |= 1 << PCF_DB2; else data &= ~(1 << PCF_DB2);
	if(c & 0x02) data |= 1 << PCF_DB1; else data &= ~(1 << PCF_DB1);
	if(c & 0x01) data |= 1 << PCF_DB0; else data &= ~(1 << PCF_DB0);
	
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_EN), 1);
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, data, 1);
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_EN), 0);
	
	PCF8575_LCD_ticks(PCF_LCD_W_N_TICKS);
}
char PCF8575_LCD0_read(unsigned short D_I)
{
	uint16_t mask, data; char c = 0;
	mask = ((1 << PCF_DB0) | (1 << PCF_DB1) | (1 << PCF_DB2) | (1 << PCF_DB3) | (1 << PCF_DB4) | (1 << PCF_DB5) | (1 << PCF_DB6) | (1 << PCF_DB7));
	
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, mask, 1);
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_RW), 1);
	
	if(D_I) pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_RS), 1); else pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_RS), 0);
	
	PCF8575_LCD_ticks(PCF_LCD_R_N_TICKS);
	
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_EN), 1);
	data = pcf8575_lcd0_com.readhbits(&pcf8575_lcd0_com.par, mask );
	pcf8575_lcd0_com.writehbits(&pcf8575_lcd0_com.par, (1 << PCF_EN), 0);
	
	if(data & (1 << PCF_DB7)) c |= 1 << 7; else c &= ~(1 << 7);
	if(data & (1 << PCF_DB6)) c |= 1 << 6; else c &= ~(1 << 6);
	if(data & (1 << PCF_DB5)) c |= 1 << 5; else c &= ~(1 << 5);
	if(data & (1 << PCF_DB4)) c |= 1 << 4; else c &= ~(1 << 4);
	if(data & (1 << PCF_DB3)) c |= 1 << 3; else c &= ~(1 << 3);
	if(data & (1 << PCF_DB2)) c |= 1 << 2; else c &= ~(1 << 2);
	if(data & (1 << PCF_DB1)) c |= 1 << 1; else c &= ~(1 << 1);
	if(data & (1 << PCF_DB0)) c |= 1 << 0; else c &= ~(1 << 0);
	
	return c;
}
void PCF8575_LCD0_BF(void)
// it has to read at minimum one equal and exit immediately if not equal, weird property.
{
	uint8_t i;
	char inst = 0x80;
	for(i=0; 0x80 & inst; i++){
		inst = PCF8575_LCD0_read(PCF_INST);
		if(i > 10)
			break;
	}
}
char PCF8575_LCD0_getch(void)
{
	char c;
	c = PCF8575_LCD0_read(PCF_DATA);
	PCF8575_LCD0_BF();
	return c;
}
void PCF8575_LCD0_putch(char c)
{
	PCF8575_LCD0_write(c, PCF_DATA);
	PCF8575_LCD0_BF();
}
void PCF8575_LCD0_string(const char* s)
{
	char tmp;
	while(*s){
		tmp = *(s++);
		PCF8575_LCD0_putch(tmp);
	}
}
void PCF8575_LCD0_string_size(const char* s, uint8_t size)
{
	char tmp;
	uint8_t pos = 0;
	while(*s){
		tmp=*(s++);
		pos++;
		if(pos > size) // 1 TO SIZE+1
			break;
		PCF8575_LCD0_putch(tmp);
	}
	while(pos < size){ // TO SIZE
		PCF8575_LCD0_putch(' ');
		pos++;
	}
}
void PCF8575_LCD0_hspace(uint8_t n)
{
	for(; n; n--){
		PCF8575_LCD0_putch(' ');
	}
}
void PCF8575_LCD0_clear(void)
{
	PCF8575_LCD0_write(0x01, PCF_INST);
    _delay_ms(1.53);
}
void PCF8575_LCD0_gotoxy(unsigned int y, unsigned int x)
{
	switch(y){
		case 0:
			PCF8575_LCD0_write((0x80 + x), PCF_INST);
			PCF8575_LCD0_BF();
		break;
		case 1:
			PCF8575_LCD0_write((0xC0 + x), PCF_INST);
			PCF8575_LCD0_BF();
		break;
		case 2:
			PCF8575_LCD0_write((0x94 + x), PCF_INST); // 0x94
			PCF8575_LCD0_BF();
		break;
		case 3:
			PCF8575_LCD0_write((0xD4 + x), PCF_INST); // 0xD4
			PCF8575_LCD0_BF();
		break;
		default:
		break;
	}
}
void PCF8575_LCD0_reboot(void)
{
	// low high detect pin NC
	uint16_t i;
	uint16_t tmp;
	tmp = pcf8575_lcd0_com.readhbits( &pcf8575_lcd0_com.par, ( 1 << PCF_NC) );
	i = tmp ^ pcf8575_lcd0_detect;
	i &= tmp;
	if(i)
		PCF8575_LCD0_inic();
	pcf8575_lcd0_detect = tmp;
}
void PCF8575_LCD_ticks(uint16_t num)
{
	uint16_t count;
	for(count = 0; count < num; count++) ;
}

/*** EOF ***/


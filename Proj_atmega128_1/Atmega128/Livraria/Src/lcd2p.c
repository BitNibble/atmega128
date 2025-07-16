/*************************************************************************
	LCD2P
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     12072025
************************************************************************/
/*** Library ***/
#include "lcd2p.h"
#include <util/delay.h>
#include <stdio.h>

/*** Constant & Macro ***/
// CMD RS
#define LCD02P_INST 0
#define LCD02P_DATA 1

/*** Variable ***/
volatile uint8_t *lcd02pcmd_DDR;
volatile uint8_t *lcd02pcmd_PIN;
volatile uint8_t *lcd02pcmd_PORT;
volatile uint8_t *lcd02pdata_DDR;
volatile uint8_t *lcd02pdata_PIN;
volatile uint8_t *lcd02pdata_PORT;
static uint8_t DDR_DATA_MASK;
uint8_t lcd02p_detect;

static const uint8_t row_offset[] = { 0x00, 0x40, 0x14, 0x54 };

typedef struct {
	uint8_t row;
	uint8_t col;
} LCDx_Pos;

/*** Procedure & Function declaration ***/
void LCD02P_inic(void);
void LCD02P_write(char c, unsigned short D_I);
char LCD02P_read(unsigned short D_I);
uint8_t LCD02P_BF(void);
void LCD02P_putch(char c);
char LCD02P_getch(void);
void LCD02P_string(const char* s); // RAW
void LCD02P_string_size(const char* s, uint8_t size); // RAW
void LCD02P_hspace(uint8_t n);
void LCD02P_clear(void);
void LCD02P_gotoxy(unsigned int y, unsigned int x);
void lcd02p_set_reg(volatile uint8_t* reg, uint8_t hbits);
void lcd02p_clear_reg(volatile uint8_t* reg, uint8_t hbits);
void LCD02P_reboot(void);

static FILE lcd02p_stdout;

static int lcd02p_putchar(char c, FILE *stream);

/*** Internal State ***/
static LCD02P_Handler setup_lcd02p = {
	// V-table
	.write = LCD02P_write,
	.read = LCD02P_read,
	.BF = LCD02P_BF,
	.putch = LCD02P_putch,
	.getch = LCD02P_getch,
	.string = LCD02P_string, // RAW
	.string_size = LCD02P_string_size, // RAW
	.hspace = LCD02P_hspace,
	.clear = LCD02P_clear,
	.gotoxy = LCD02P_gotoxy,
	.reboot = LCD02P_reboot,	
	.printf = printf
};

/*** Handler ***/
void lcd02p_enable(volatile uint8_t *cmdddr, volatile uint8_t *cmdpin, volatile uint8_t *cmdport, volatile uint8_t *dataddr, volatile uint8_t *datapin, volatile uint8_t *dataport)
{
	// import parameters
	lcd02pcmd_DDR = cmdddr;
	lcd02pcmd_PIN = cmdpin;
	lcd02pcmd_PORT = cmdport;
	lcd02pdata_DDR = dataddr;
	lcd02pdata_PIN = datapin;
	lcd02pdata_PORT = dataport;
	DDR_DATA_MASK = ((1 << LCD02P_DB4) | (1 << LCD02P_DB5) | (1 << LCD02P_DB6) | (1 << LCD02P_DB7));
	// LCD INIC
	LCD02P_inic();
	
	lcd02p_stdout = (FILE) FDEV_SETUP_STREAM(lcd02p_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &lcd02p_stdout;  // Redirect printf to the LCD
}

LCD02P_Handler* lcd02p(void){ return &setup_lcd02p; }

/*** Procedure & Function definition ***/
void LCD02P_inic(void)
{
	// LCD INIC
	*lcd02pcmd_DDR |= (1 << LCD02P_RS) | (1 << LCD02P_RW) | (1 << LCD02P_EN) | (0 << LCD02P_NC);
	*lcd02pcmd_PORT |= (1 << LCD02P_NC);
	*lcd02pdata_DDR &= ~DDR_DATA_MASK;
	*lcd02pdata_PORT |= DDR_DATA_MASK;
	lcd02p_detect = *lcd02pcmd_PIN & (1 << LCD02P_NC);

	// INICIALIZACAO LCD datasheet
	_delay_ms(40); // using clock at 16Mhz
	LCD02P_write(0x30, LCD02P_INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_ms(5);
	LCD02P_write(0x30, LCD02P_INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD02P_write(0x30, LCD02P_INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD02P_write(0x20, LCD02P_INST); // 0x28 4 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD02P_write(0x28, LCD02P_INST); // 0x28 4 bit, 2 line, 5x8, --, --
	_delay_us(50);
	LCD02P_write(0x0C, LCD02P_INST); // 0x0C Display ON, Cursor OFF, Blink ON
	_delay_us(50);
	LCD02P_write(0x01, LCD02P_INST); // 0x01 Display clear
	_delay_ms(2);
	LCD02P_write(0x06, LCD02P_INST); // 0x06 Cursor dir, Display shift
	_delay_us(50);
	LCD02P_clear();
	LCD02P_gotoxy(0,0);
	LCD02P_BF();
}
void LCD02P_write(char c, unsigned short D_I)
{
	lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_RW)); // lcd as input
	lcd02p_set_reg(lcd02pdata_DDR, DDR_DATA_MASK); // mcu as output
	
	if(D_I) lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));  else lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));
	
	lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	if(c & 0x80) *lcd02pdata_PORT |= 1 << LCD02P_DB7; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB7);
	if(c & 0x40) *lcd02pdata_PORT |= 1 << LCD02P_DB6; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB6);
	if(c & 0x20) *lcd02pdata_PORT |= 1 << LCD02P_DB5; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB5);
	if(c & 0x10) *lcd02pdata_PORT |= 1 << LCD02P_DB4; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB4);
	lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	
	if(D_I) lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));  else lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));
	
	lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	if(c & 0x08) *lcd02pdata_PORT |= 1 << LCD02P_DB7; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB7);
	if(c & 0x04) *lcd02pdata_PORT |= 1 << LCD02P_DB6; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB6);
	if(c & 0x02) *lcd02pdata_PORT |= 1 << LCD02P_DB5; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB5);
	if(c & 0x01) *lcd02pdata_PORT |= 1 << LCD02P_DB4; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB4);
	lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	for (uint16_t i = 0; i < 180; i++); // value above 140 for 16Mhz
}
char LCD02P_read(unsigned short D_I)
{
	char c = 0x00;
	lcd02p_clear_reg(lcd02pdata_DDR, DDR_DATA_MASK); // mcu as input
	lcd02p_set_reg(lcd02pdata_PORT, DDR_DATA_MASK); // pull up resistors
	lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_RW)); // lcd as output
	
	if(D_I) lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));  else lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));
	
	lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	if(*lcd02pdata_PIN & (1 << LCD02P_DB7)) c |= 1 << 7; else c &= ~(1 << 7);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB6)) c |= 1 << 6; else c &= ~(1 << 6);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB5)) c |= 1 << 5; else c &= ~(1 << 5);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB4)) c |= 1 << 4; else c &= ~(1 << 4);
	lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	
	if(D_I) lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));  else lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_RS));
	
	lcd02p_set_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	if(*lcd02pdata_PIN & (1 << LCD02P_DB7)) c |= 1 << 3; else c &= ~(1 << 3);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB6)) c |= 1 << 2; else c &= ~(1 << 2);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB5)) c |= 1 << 1; else c &= ~(1 << 1);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB4)) c |= 1 << 0; else c &= ~(1 << 0);
	lcd02p_clear_reg(lcd02pcmd_PORT, (1 << LCD02P_EN));
	for (uint16_t i = 0; i < 180; i++); // value above 140 for 16Mhz
	return c;
}
uint8_t LCD02P_BF(void)
{
	uint8_t i;
	char inst = 0x80;
	for(i=0; 0x80 & inst; i++){
		inst = LCD02P_read(LCD02P_INST);
		if(i > 10)
			break;
	}
	return (inst & 0x7F);
}
char LCD02P_getch(void)
{
	return LCD02P_read(LCD02P_DATA);
}
void LCD02P_putch(char c)
{
	LCD02P_write(c, LCD02P_DATA);
}
void LCD02P_string(const char* s)
{
	while(*s){
		LCD02P_putch(*(s++));
	}
}
void LCD02P_string_size(const char* s, uint8_t size)
{
	uint8_t pos = 0;
	while(*s){
		pos++;
		if(pos > size) // 1 TO SIZE+1
			break;
		LCD02P_putch(*(s++));
	}
	while(pos < size){ // TO SIZE
		LCD02P_putch(' ');
		pos++;
	}
}
void LCD02P_hspace(uint8_t n)
{
	for(; n; n--){
		LCD02P_putch(' ');
	}
}
void LCD02P_clear(void)
{
	LCD02P_write(0x01, LCD02P_INST);
    _delay_ms(1.53);
}
void LCD02P_gotoxy(unsigned int y, unsigned int x)
{
	uint8_t addr = row_offset[y] + x;
	LCD02P_write(0x80 | addr, LCD02P_INST);
}
void lcd02p_set_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg |= hbits;
}
void lcd02p_clear_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg &= ~hbits;
}
void LCD02P_reboot(void)
{
	// low high detect pin NC
	uint8_t tmp = *lcd02pcmd_PIN & (1 << LCD02P_NC);
	uint8_t i = tmp ^ lcd02p_detect;
	i &= tmp;
	if(i)
		LCD02P_inic();
	lcd02p_detect = tmp;
}
// Custom character output function
int lcd02p_putchar(char c, FILE *stream) {
	(void) stream;
	
	LCD02P_putch(c);
	uint8_t pos = LCD02P_BF();
	
	if (pos == row_offset[0]) {
		LCD02P_gotoxy(0, 0);
	}
	else if (pos == row_offset[2]) {
		LCD02P_gotoxy(1, 0);
	}
	else if (pos == row_offset[3]) {
		LCD02P_gotoxy(2, 0);
	}
	else if (pos == row_offset[1]) {
		LCD02P_gotoxy(3, 0);
	}
	return 0;
}

/*** EOF ***/


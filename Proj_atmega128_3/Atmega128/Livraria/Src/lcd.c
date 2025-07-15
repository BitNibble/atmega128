/*************************************************************************
	LCD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     04072025              
************************************************************************/
/*** Library ***/
#include "lcd.h"
#include <util/delay.h>
#include <stdio.h>

/*** Constant & Macro ***/
// CMD RS
#define INST 0
#define DATA 1

/*** Variable ***/
volatile uint8_t *lcd0_DDR;
volatile uint8_t *lcd0_PIN;
volatile uint8_t *lcd0_PORT;
uint8_t lcd0_detect;
volatile uint8_t *lcd1_DDR;
volatile uint8_t *lcd1_PIN;
volatile uint8_t *lcd1_PORT;
uint8_t lcd1_detect;

static const uint8_t row_offset[] = { 0x00, 0x40, 0x14, 0x54 };

typedef struct {
	uint8_t row;
	uint8_t col;
} LCDx_Pos;

/*** Procedure & Function declaration ***/
void LCD0_inic(void);
void LCD0_write(char c, unsigned short D_I);
char LCD0_read(unsigned short D_I);
uint8_t LCD0_BF(void);
void LCD0_putch(char c);
char LCD0_getch(void);
void LCD0_string(const char* s); // RAW
void LCD0_string_size(const char* s, uint8_t size); // RAW
void LCD0_hspace(uint8_t n);
void LCD0_clear(void);
void LCD0_gotoxy(unsigned int y, unsigned int x);
void LCD0_reboot(void);
void LCD1_inic(void);
void LCD1_write(char c, unsigned short D_I);
char LCD1_read(unsigned short D_I);
uint8_t LCD1_BF(void);
void LCD1_putch(char c);
char LCD1_getch(void);
void LCD1_string(const char* s); // RAW
void LCD1_string_size(const char* s, uint8_t size); // RAW
void LCD1_hspace(uint8_t n);
void LCD1_clear(void);
void LCD1_gotoxy(unsigned int y, unsigned int x);
void LCD1_reboot(void);
void lcd_set_reg(volatile uint8_t* reg, uint8_t hbits);
void lcd_clear_reg(volatile uint8_t* reg, uint8_t hbits);

static FILE lcd0_stdout;
static FILE lcd1_stdout;

static int lcd0_putchar(char c, FILE *stream);
static int lcd1_putchar(char c, FILE *stream);

/*** Internal State ***/
static LCD0 lcd0_setup = {
	// V-table
	.write = LCD0_write,
	.read = LCD0_read,
	.BF = LCD0_BF,
	.putch = LCD0_putch,
	.getch = LCD0_getch,
	.string = LCD0_string, // RAW
	.string_size = LCD0_string_size, // RAW
	.hspace = LCD0_hspace,
	.clear = LCD0_clear,
	.gotoxy = LCD0_gotoxy,
	.reboot = LCD0_reboot,
	.printf = printf
};
static LCD1 lcd1_setup = {
	// V-table
	.write = LCD1_write,
	.read = LCD1_read,
	.BF = LCD1_BF,
	.putch = LCD1_putch,
	.getch = LCD1_getch,
	.string = LCD1_string, // RAW
	.string_size = LCD1_string_size, // RAW
	.hspace = LCD1_hspace,
	.clear = LCD1_clear,
	.gotoxy = LCD1_gotoxy,
	.reboot = LCD1_reboot,
	.printf  = printf
};

/*** Handler ***/
void lcd0_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port)
{
	// import parameters
	lcd0_DDR = ddr;
	lcd0_PIN = pin;
	lcd0_PORT = port;
	// initialize variables
	*lcd0_DDR = 0x00;
	*lcd0_PORT = 0xFF;
	lcd0_detect = *lcd0_PIN & (1 << NC);
	// LCD INIC
	LCD0_inic();
	
	lcd0_stdout = (FILE) FDEV_SETUP_STREAM(lcd0_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &lcd0_stdout;  // Redirect printf to the LCD
}

LCD0* lcd0(void){ return &lcd0_setup; }

/*** Procedure & Function definition ***/
void LCD0_inic(void)
{
	// LCD INIC
	*lcd0_DDR = ((1 << RS) | (1 << RW) | (1 << EN) | (0 << NC));
	*lcd0_PORT = (1 << NC);

	// INICIALIZACAO LCD datasheet
	_delay_ms(40); // using clock at 16Mhz
	LCD0_write(0x30, INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_ms(5);
	LCD0_write(0x30, INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD0_write(0x30, INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD0_write(0x20, INST); // 0x20 4 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD0_write(0x28, INST); // 0x28 4 bit, 2 line, 5x8, --, --
	_delay_us(50);
	LCD0_write(0x0C, INST); // 0x0C Display ON, Cursor OFF, Blink ON
	_delay_us(50);
	LCD0_write(0x01, INST); // 0x01 Display clear
	_delay_ms(2);
	LCD0_write(0x06, INST); // 0x04 Cursor dir, Display shift
	_delay_us(50);
	LCD0_clear();
	LCD0_gotoxy(0,0);
	LCD0_BF();
}
void LCD0_write(char c, unsigned short D_I)
{
	lcd_clear_reg(lcd0_PORT, (1 << RW)); // lcd as input
	lcd_set_reg(lcd0_DDR, (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7)); // mcu as output
	
	if(D_I) lcd_set_reg(lcd0_PORT, (1 << RS));  else lcd_clear_reg(lcd0_PORT, (1 << RS));
	
	lcd_set_reg(lcd0_PORT, (1 << EN));
	if(c & 0x80) *lcd0_PORT |= 1 << DB7; else *lcd0_PORT &= ~(1 << DB7);
	if(c & 0x40) *lcd0_PORT |= 1 << DB6; else *lcd0_PORT &= ~(1 << DB6);
	if(c & 0x20) *lcd0_PORT |= 1 << DB5; else *lcd0_PORT &= ~(1 << DB5);
	if(c & 0x10) *lcd0_PORT |= 1 << DB4; else *lcd0_PORT &= ~(1 << DB4);
	lcd_clear_reg(lcd0_PORT, (1 << EN));
	
	if(D_I) lcd_set_reg(lcd0_PORT, (1 << RS));  else lcd_clear_reg(lcd0_PORT, (1 << RS));
	
	lcd_set_reg(lcd0_PORT, (1 << EN));
	if(c & 0x08) *lcd0_PORT |= 1 << DB7; else *lcd0_PORT &= ~(1 << DB7);
	if(c & 0x04) *lcd0_PORT |= 1 << DB6; else *lcd0_PORT &= ~(1 << DB6);
	if(c & 0x02) *lcd0_PORT |= 1 << DB5; else *lcd0_PORT &= ~(1 << DB5);
	if(c & 0x01) *lcd0_PORT |= 1 << DB4; else *lcd0_PORT &= ~(1 << DB4);
	lcd_clear_reg(lcd0_PORT, (1 << EN));
	for (uint16_t i = 0; i < 180; i++); // value above 140 for 16Mhz
}
char LCD0_read(unsigned short D_I)
{
	char c = 0x00;
	lcd_clear_reg(lcd0_DDR, (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7)); // mcu as input
	lcd_set_reg(lcd0_PORT, (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7)); // pull up resistors
	lcd_set_reg(lcd0_PORT, (1 << RW)); // lcd as output
	
	if(D_I) lcd_set_reg(lcd0_PORT, (1 << RS));  else lcd_clear_reg(lcd0_PORT, (1 << RS));
	
	lcd_set_reg(lcd0_PORT, (1 << EN));
	if(*lcd0_PIN & (1 << DB7)) c |= 1 << 7; else c &= ~(1 << 7);
	if(*lcd0_PIN & (1 << DB6)) c |= 1 << 6; else c &= ~(1 << 6);
	if(*lcd0_PIN & (1 << DB5)) c |= 1 << 5; else c &= ~(1 << 5);
	if(*lcd0_PIN & (1 << DB4)) c |= 1 << 4; else c &= ~(1 << 4);
	lcd_clear_reg(lcd0_PORT, (1 << EN));
	
	if(D_I) lcd_set_reg(lcd0_PORT, (1 << RS));  else lcd_clear_reg(lcd0_PORT, (1 << RS));
	
	lcd_set_reg(lcd0_PORT, (1 << EN));
	if(*lcd0_PIN & (1 << DB7)) c |= 1 << 3; else c &= ~(1 << 3);
	if(*lcd0_PIN & (1 << DB6)) c |= 1 << 2; else c &= ~(1 << 2);
	if(*lcd0_PIN & (1 << DB5)) c |= 1 << 1; else c &= ~(1 << 1);
	if(*lcd0_PIN & (1 << DB4)) c |= 1 << 0; else c &= ~(1 << 0);
	lcd_clear_reg(lcd0_PORT, (1 << EN));
	for (uint16_t i = 0; i < 180; i++); // value above 140 for 16Mhz
	return c;
}
uint8_t LCD0_BF(void)
{
	uint8_t i;
	char inst = 0x80;
	for(i=0; 0x80 & inst; i++){
		inst = LCD0_read(INST);
		if(i > 10)
			break;
	}
	return (inst & 0x7F);
}
char LCD0_getch(void)
{
	return LCD0_read(DATA);
}
void LCD0_putch(char c)
{
	LCD0_write(c, DATA);
}
void LCD0_string(const char* s)
{
	while(*s){
		LCD0_putch(*(s++));
	}
}
void LCD0_string_size(const char* s, uint8_t size)
{
	uint8_t pos = 0;
	while(*s){
		pos++;
		if(pos > size) // 1 TO SIZE+1
			break;
		LCD0_putch(*(s++));
	}
	while(pos < size){ // TO SIZE
		LCD0_putch(' ');
		pos++;
	}
}
void LCD0_hspace(uint8_t n)
{
	for(; n; n--){
		LCD0_putch(' ');
	}
}
void LCD0_clear(void)
{
	LCD0_write(0x01, INST);
    _delay_ms(1.53);
}
void LCD0_gotoxy(unsigned int y, unsigned int x)
{
	uint8_t addr = row_offset[y] + x;
	LCD0_write(0x80 | addr, INST);
}
void LCD0_reboot(void)
{
	// low high detect pin NC
	uint8_t tmp = *lcd0_PIN & (1 << NC);
	uint8_t i = tmp ^ lcd0_detect;
	i &= tmp;
	if(i)
		LCD0_inic();
	lcd0_detect = tmp;
}
// Custom character output function
int lcd0_putchar(char c, FILE *stream) {
	(void) stream;
	
	LCD0_putch(c);
	uint8_t pos = LCD0_BF();
	
	if (pos == row_offset[0]) {
		LCD0_gotoxy(0, 0);
	}
	else if (pos == row_offset[2]) {
		LCD0_gotoxy(1, 0);
	}
	else if (pos == row_offset[3]) {
		LCD0_gotoxy(2, 0);
	}
	else if (pos == row_offset[1]) {
		LCD0_gotoxy(3, 0);
	}
	return 0;
}

// LCD 1
void lcd1_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port)
{
	// import parameters
	lcd1_DDR = ddr;
	lcd1_PIN = pin;
	lcd1_PORT = port;
	// initialize variables
	*lcd1_DDR = 0x00;
	*lcd1_PORT = 0xFF;
	lcd1_detect = *lcd1_PIN & (1 << NC);
	// LCD INIC
	LCD1_inic();
	
	lcd1_stdout = (FILE) FDEV_SETUP_STREAM(lcd1_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &lcd1_stdout;  // Redirect printf to the LCD
}

LCD1* lcd1(void){ return &lcd1_setup; }

void LCD1_inic(void)
{
	// LCD INIC
	*lcd1_DDR = (1 << RS) | (1 << RW) | (1 << EN) | (0 << NC);
	*lcd1_PORT = (1 << NC);

	// INICIALIZACAO LCD datasheet
	_delay_ms(40); // using clock at 16Mhz
	LCD1_write(0x30, INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_ms(5);
	LCD1_write(0x30, INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD1_write(0x30, INST); // 0x30 8 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD1_write(0x20, INST); // 0x28 4 bit, 1 line, 5x8, --, --
	_delay_us(150);
	LCD1_write(0x28, INST); // 0x28 4 bit, 2 line, 5x8, --, --
	_delay_us(50);
	LCD1_write(0x0C, INST); // 0x0C Display ON, Cursor OFF, Blink ON
	_delay_us(50);
	LCD1_write(0x01, INST); // 0x01 Display clear
	_delay_ms(2);
	LCD1_write(0x06, INST); // 0x04 Cursor dir, Display shift
	_delay_us(50);
	LCD1_clear();
	LCD1_gotoxy(0,0);
	LCD1_BF();
}
void LCD1_write(char c, unsigned short D_I)
{
	lcd_clear_reg(lcd1_PORT, (1 << RW)); // lcd as input
	lcd_set_reg(lcd1_DDR, (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7)); // mcu as output
	
	if(D_I) lcd_set_reg(lcd1_PORT, (1 << RS));  else lcd_clear_reg(lcd1_PORT, (1 << RS));
	
	lcd_set_reg(lcd1_PORT, (1 << EN));
	if(c & 0x80) *lcd1_PORT |= 1 << DB7; else *lcd1_PORT &= ~(1 << DB7);
	if(c & 0x40) *lcd1_PORT |= 1 << DB6; else *lcd1_PORT &= ~(1 << DB6);
	if(c & 0x20) *lcd1_PORT |= 1 << DB5; else *lcd1_PORT &= ~(1 << DB5);
	if(c & 0x10) *lcd1_PORT |= 1 << DB4; else *lcd1_PORT &= ~(1 << DB4);
	lcd_clear_reg(lcd1_PORT, (1 << EN));
	
	if(D_I) lcd_set_reg(lcd1_PORT, (1 << RS));  else lcd_clear_reg(lcd1_PORT, (1 << RS));
	
	lcd_set_reg(lcd1_PORT, (1 << EN));
	if(c & 0x08) *lcd1_PORT |= 1 << DB7; else *lcd1_PORT &= ~(1 << DB7);
	if(c & 0x04) *lcd1_PORT |= 1 << DB6; else *lcd1_PORT &= ~(1 << DB6);
	if(c & 0x02) *lcd1_PORT |= 1 << DB5; else *lcd1_PORT &= ~(1 << DB5);
	if(c & 0x01) *lcd1_PORT |= 1 << DB4; else *lcd1_PORT &= ~(1 << DB4);
	lcd_clear_reg(lcd1_PORT, (1 << EN));
	for (uint16_t i = 0; i < 300; i++); // value above 140 for 16Mhz
}
char LCD1_read(unsigned short D_I)
{
	char c = 0x00;
	lcd_clear_reg(lcd1_DDR, (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7)); // mcu as input
	lcd_set_reg(lcd1_PORT, (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7)); // pull up resistors
	lcd_set_reg(lcd1_PORT, (1 << RW)); // lcd as output
	
	if(D_I) lcd_set_reg(lcd1_PORT, (1 << RS));  else lcd_clear_reg(lcd1_PORT, (1 << RS));
	
	lcd_set_reg(lcd1_PORT, (1 << EN));
	if(*lcd1_PIN & (1 << DB7)) c |= 1 << 7; else c &= ~(1 << 7);
	if(*lcd1_PIN & (1 << DB6)) c |= 1 << 6; else c &= ~(1 << 6);
	if(*lcd1_PIN & (1 << DB5)) c |= 1 << 5; else c &= ~(1 << 5);
	if(*lcd1_PIN & (1 << DB4)) c |= 1 << 4; else c &= ~(1 << 4);
	lcd_clear_reg(lcd1_PORT, (1 << EN));
	
	if(D_I) lcd_set_reg(lcd1_PORT, (1 << RS));  else lcd_clear_reg(lcd1_PORT, (1 << RS));
	
	lcd_set_reg(lcd1_PORT, (1 << EN));
	if(*lcd1_PIN & (1 << DB7)) c |= 1 << 3; else c &= ~(1 << 3);
	if(*lcd1_PIN & (1 << DB6)) c |= 1 << 2; else c &= ~(1 << 2);
	if(*lcd1_PIN & (1 << DB5)) c |= 1 << 1; else c &= ~(1 << 1);
	if(*lcd1_PIN & (1 << DB4)) c |= 1 << 0; else c &= ~(1 << 0);
	lcd_clear_reg(lcd1_PORT, (1 << EN));
	for (uint16_t i = 0; i < 300; i++); // value above 140 for 16Mhz
	return c;
}
uint8_t LCD1_BF(void)
{
	uint8_t i;
	char inst = 0x80;
	for(i=0; (0x80 & inst); i++){
		inst = LCD1_read(INST);
		if(i > 10)
			break;
	}
	return (inst & 0x7F);
}
char LCD1_getch(void)
{
	return LCD1_read(DATA);
}
void LCD1_putch(char c)
{
	LCD1_write(c, DATA);
}
void LCD1_string(const char* s)
{
	while(*s){
		LCD1_putch(*(s++));
	}
}
void LCD1_string_size(const char* s, uint8_t size)
{
	uint8_t pos = 0;
	while(*s){
		pos++;
		if(pos > size)
			break;
		LCD1_putch(*(s++));
	}
	while(pos < size){
		LCD1_putch(' ');
		pos++;
	}
}
void LCD1_hspace(uint8_t n)
{
	for(; n; n--){
		LCD1_putch(' ');
	}
}
void LCD1_clear(void)
{
	LCD1_write(0x01, INST);
	_delay_ms(1.53);
}
void LCD1_gotoxy(unsigned int y, unsigned int x)
{
	uint8_t addr = row_offset[y] + x;
	LCD0_write(0x80 | addr, INST);
}
void LCD1_reboot(void)
{
	// low high detect pin NC
	uint8_t tmp = *lcd1_PIN & (1 << NC);
	uint8_t i = tmp ^ lcd1_detect;
	i &= tmp;
	if(i)
		LCD1_inic();
	lcd1_detect = tmp;
}
// Custom character output function
int lcd1_putchar(char c, FILE *stream) {
	(void) stream;
	
	LCD1_putch(c);
	uint8_t pos = LCD1_BF();
	
	if (pos == row_offset[0]) {
		LCD1_gotoxy(0, 0);
	}
	else if (pos == row_offset[2]) {
		LCD1_gotoxy(1, 0);
	}
	else if (pos == row_offset[3]) {
		LCD1_gotoxy(2, 0);
	}
	else if (pos == row_offset[1]) {
		LCD1_gotoxy(3, 0);
	}
	return 0;
}
void lcd_set_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg |= hbits;
}
void lcd_clear_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg &= ~hbits;
}

/*** EOF ***/


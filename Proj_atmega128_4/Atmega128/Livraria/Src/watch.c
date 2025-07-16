/*************************************************************************
Title:    watch.c
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Date:     03072025
*************************************************************************/
/*** Library ***/
#include "watch.h"

/*** Constant & Macro***/
#define N_DELAY 16
#define N_DELAY_MASK 0X0F
#define W12HOUR 12
#define W24HOUR 24
#define W12_HOUR_SECONDS 43200
#define W24_HOUR_SECONDS 86400
const uint32_t w12_hour_seconds = (W12_HOUR_SECONDS - 1);
const uint32_t w24_hour_seconds = (W24_HOUR_SECONDS - 1);

/*** Variable ***/
static WATCH_Time wtime;
static char WATCH_vector[9] = {0};
static uint32_t WATCH_trigger[N_DELAY] = {0};
static uint8_t WATCH_delay_flag[N_DELAY] = {0};

/*** Procedure and Function declaration ***/
void WATCH_set_hour(uint8_t hour);
void WATCH_set_minute(uint8_t min);
void WATCH_set_second(uint8_t sec);
void WATCH_preset(uint8_t hour, uint8_t minute, uint8_t second);
uint8_t WATCH_start_delay(uint8_t n_delay, uint32_t seconds);
void WATCH_result(void);
char* WATCH_show(void);

/*** Handler ***/
WATCH_Handler watch_enable(void)
{
	wtime.hour = 0;
	wtime.minute = 0;
	wtime.second = 0;
	wtime.seconds = 0;
	
	WATCH_Handler watch;
	watch.preset = WATCH_preset;
	watch.start_delay = WATCH_start_delay;
	watch.increment = WATCH_increment;
	watch.show = WATCH_show;
	return watch;
}

/*** Procedure and Function definition ***/
uint8_t WATCH_start_delay(uint8_t delay_n, uint32_t seconds){
	uint32_t segundos = 0;
	uint8_t ret = 0; // one shot repeat
	delay_n &= N_DELAY_MASK;
	if(WATCH_delay_flag[delay_n]) {
		segundos = wtime.seconds;
		if( segundos >= WATCH_trigger[delay_n] ) {
			WATCH_delay_flag[delay_n] = 0;
			ret = 1;
		}
	}else {
		segundos = wtime.seconds + seconds;
		if(segundos > w24_hour_seconds)
			WATCH_trigger[delay_n] = segundos - W24_HOUR_SECONDS;
		else
			WATCH_trigger[delay_n] = segundos;
		WATCH_delay_flag[delay_n] = 1;
	}
	return ret;
}

void WATCH_preset(uint8_t hour, uint8_t minute, uint8_t second)
{
	if(hour < W24HOUR)
		wtime.hour = hour;
	else
		wtime.hour = 0;
		
	if( minute < 60 )
		wtime.minute = minute;
	else
		wtime.minute = 0;
		
	if( second < 60 )
		;
	else
		wtime.second = 0;
	wtime.seconds = (uint32_t) (hour * 3600. + minute * 60. + second);
}

void WATCH_set_second(uint8_t sec){
	if( sec < 60 )
	wtime.seconds = wtime.hour * 3600. + wtime.minute * 60. + sec;
}

void WATCH_set_minute(uint8_t min){
	if( min < 60 )
		wtime.seconds = wtime.hour * 3600. + min * 60. + wtime.second;
}

void WATCH_set_hour(uint8_t hour){
	if( hour < W24HOUR )
		wtime.seconds = hour * 3600. + wtime.minute * 60. + wtime.second;
}

void WATCH_increment(void)
{
	if(wtime.seconds < w24_hour_seconds ) {
		wtime.seconds++;
	}else {
		wtime.seconds = 0;
	}
}

void WATCH_decrement(void)
{
	if(wtime.seconds)
		wtime.seconds--;
	else
		wtime.seconds = w24_hour_seconds;
}

void WATCH_result(void)
{
	uint16_t remainder = wtime.seconds % 3600;
	wtime.hour = wtime.seconds / 3600;
	wtime.minute = remainder / 60;
	wtime.second = remainder % 60;
}

char* WATCH_show(void)
{
	uint8_t tmp;
	WATCH_result();
	WATCH_vector[8] = '\0';
	WATCH_vector[7] = wtime.second % 10 + '0';
	tmp = wtime.second / 10;
	WATCH_vector[6] = tmp % 10 + '0';
	WATCH_vector[5] = ':';
	WATCH_vector[4] = wtime.minute % 10 + '0';
	tmp = wtime.minute / 10;
	WATCH_vector[3] = tmp % 10 + '0';
	WATCH_vector[2] = ':';
	WATCH_vector[1] = wtime.hour % 10 + '0';
	tmp = wtime.hour / 10;
	WATCH_vector[0] = tmp % 10 + '0';
	return WATCH_vector;
}

/*** EOF ***/


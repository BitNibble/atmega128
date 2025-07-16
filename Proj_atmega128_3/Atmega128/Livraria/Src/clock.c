/*************************************************************************
	CLOCK
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     25102020
*************************************************************************/
/*** Library ***/
#include "clock.h"

/*** Variable ***/
struct CLOCK_Time time;
struct CLOCK_Time laptime;
struct CLOCK_Time alarmtime;
char CLOCK_timp[9];
uint8_t CLOCK_alarm_flag;
uint8_t CLOCK_compare_active;

/*** Procedure and Function declaration ***/
void CLOCK_set(uint8_t hour, uint8_t minute, uint8_t second);
void CLOCK_increment(void);
void CLOCK_decrement(void);
uint8_t CLOCK_alarm(uint8_t hour, uint8_t minute, uint8_t second);
uint8_t CLOCK_second_count(uint16_t second);
void CLOCK_second_count_reset(void);
void CLOCK_second_count_stop(void);
void CLOCK_alarm_reset(void);
void CLOCK_alarm_stop(void);
char* CLOCK_show(void);

/*** Handler ***/
CLOCK_Handler clock_enable(uint8_t hour, uint8_t minute, uint8_t second)
{
	CLOCK_Handler setup_clock;
	
	time.hour = hour;
	time.minute = minute;
	time.second = second;
	CLOCK_alarm_flag = 0X0F;
	CLOCK_compare_active = 0X0F;
	// V-table
	setup_clock.set = CLOCK_set;
	setup_clock.increment = CLOCK_increment;
	setup_clock.decrement = CLOCK_decrement;
	setup_clock.alarm = CLOCK_alarm;
	setup_clock.second_count = CLOCK_second_count;
	setup_clock.second_count_reset = CLOCK_second_count_reset;
	setup_clock.second_count_stop = CLOCK_second_count_stop;
	setup_clock.alarm_reset = CLOCK_alarm_reset;
	setup_clock.alarm_stop = CLOCK_alarm_stop;
	setup_clock.show = CLOCK_show;
	
	return setup_clock;
}

/*** Procedure and Function definition ***/
void CLOCK_set(uint8_t hour, uint8_t minute, uint8_t second)
{
	time.hour = hour;
	time.minute = minute;
	time.second = second;
}
void CLOCK_increment(void)
{
	time.second++;
	if(time.second > 59){
		time.minute++;
		time.second = 0;
		if(time.minute > 59){
			time.hour++;
			time.minute = 0;
			switch (HORA){
				case 24:
					if(time.hour > 23)
						time.hour = 0;
				break;
				case 12:
					if (time.hour > 12)
						time.hour = 1;
				break;
				default:
					if(time.hour > 23)
						time.hour = 0;
				break;
			}
		}
	}
	
	if(time.hour == laptime.hour && CLOCK_compare_active == 4)
		CLOCK_compare_active = 3;
	if(time.hour == laptime.hour && time.minute == laptime.minute && CLOCK_compare_active == 3)
		CLOCK_compare_active = 2;
	if(time.hour == laptime.hour && time.minute == laptime.minute && time.second == laptime.second && CLOCK_compare_active == 2)
		CLOCK_compare_active = 1;
	
	if(time.hour == alarmtime.hour && CLOCK_alarm_flag == 4)
		CLOCK_alarm_flag = 3;
	if(time.hour == alarmtime.hour && time.minute == alarmtime.minute && CLOCK_alarm_flag == 3)
		CLOCK_alarm_flag = 2;
	if(time.hour == alarmtime.hour && time.minute == alarmtime.minute && time.second == alarmtime.second && CLOCK_alarm_flag == 2)
		CLOCK_alarm_flag = 1;
}
void CLOCK_decrement(void)
{
	time.second--;
	if(time.second < 0){
		time.minute--;
		time.second = 59;
		if(time.minute < 0){
			time.hour--;
			time.minute = 59;
			switch (HORA){
				case 24:
					if(time.hour < 0)
						time.hour= 23;
				break;
				case 12:
					if (time.hour < 1)
						time.hour = 12;
				break;
				default:
					if(time.hour < 0)
						time.hour = 23;
				break;
			}
		}
	}
	
	if(time.hour == alarmtime.hour && CLOCK_alarm_flag == 4)
		CLOCK_alarm_flag = 3;
	if(time.hour == alarmtime.hour && time.minute == alarmtime.minute && CLOCK_alarm_flag == 3)
		CLOCK_alarm_flag = 2;
	if(time.hour == alarmtime.hour && time.minute == alarmtime.minute && time.second == alarmtime.second && CLOCK_alarm_flag == 2)
		CLOCK_alarm_flag = 1;		
}
uint8_t CLOCK_alarm(uint8_t hour, uint8_t minute, uint8_t second)
{
	if(!CLOCK_alarm_flag){
		alarmtime.hour = hour;
		alarmtime.minute = minute;
		alarmtime.second = second;
		CLOCK_alarm_flag = 4;
	}
	return CLOCK_alarm_flag;
}
uint8_t CLOCK_second_count(uint16_t second)
{
	uint16_t tmp;
	uint8_t tmp_1;
	uint8_t tmp_2;
	uint8_t tmp_3;
	if(!CLOCK_compare_active){
		tmp_3 = (int8_t)(second % 60);
		tmp = second / 60;
		tmp_2 = (int8_t)(tmp % 60);
		tmp = tmp / 60;
		tmp_1 = (int8_t)tmp;
		laptime.second = (time.second + tmp_3) % 60;
		tmp_3 = (time.second + tmp_3) / 60;
		laptime.minute = (time.minute + tmp_2 + tmp_3) % 60;
		tmp_2 = (time.minute + tmp_2 + tmp_3) / 60;
		tmp_1 = time.hour+ tmp_1 + tmp_2;
		if(tmp_1 > HORA)
			laptime.hour = HORA - tmp_1;
		else
			laptime.hour = tmp_1;
		CLOCK_compare_active = 4;
	}
	return CLOCK_compare_active;
}
void CLOCK_alarm_reset(void)
{
	CLOCK_alarm_flag = 0;
}
void CLOCK_alarm_stop(void)
{
	CLOCK_alarm_flag = 0X0F;
}
void CLOCK_second_count_reset(void)
{
	CLOCK_compare_active = 0;
}
void CLOCK_second_count_stop(void)
{
	CLOCK_compare_active = 0X0F;
}
char* CLOCK_show(void)
{
	uint8_t tmp;
	CLOCK_timp[8] = '\0';
	CLOCK_timp[7] = time.second % 10 + '0';
	tmp = time.second / 10;
	CLOCK_timp[6] = tmp % 10 + '0';
	CLOCK_timp[5] = ':';
	CLOCK_timp[4] = time.minute % 10 + '0';
	tmp = time.minute / 10;
	CLOCK_timp[3] = tmp % 10 + '0';
	CLOCK_timp[2] = ':';
	CLOCK_timp[1] = time.hour % 10 + '0';
	tmp = time.hour / 10;
	CLOCK_timp[0] = tmp % 10 + '0';
	return CLOCK_timp;
}

/*** EOF ***/


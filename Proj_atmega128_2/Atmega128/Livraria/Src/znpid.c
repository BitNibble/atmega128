/*************************************************************************
	ZNPID
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License     
Hardware: Atmega 128
Date:     17022021_start
************************************************************************/
/*** Library ***/
#include "znpid.h"

/*** Procedure and Function declaration ***/
void ZNPID_set_kc(ZNPID_Parameter* par, double kc);
void ZNPID_set_ki(ZNPID_Parameter* par, double ki);
void ZNPID_set_kd(ZNPID_Parameter* par, double kd);
void ZNPID_set_SP(ZNPID_Parameter* par, double setpoint);
double ZNPID_output(ZNPID_Parameter* par, double PV, double timelapse);
double ZNPID_integral(ZNPID_Parameter* par, double PV, double timelapse);
double ZNPID_derivative(ZNPID_Parameter* par, double PV, double timelapse);
double ZNPID_delta(double present_value, double past_value);
double ZNPID_sum(double value_1, double value_2);
double ZNPID_product(double value_1, double value_2);

/*** Handler ***/
ZNPID_Handler znpid_enable(void)
{
	// LOCAL VARIABLES
	ZNPID_Handler setup_znpid;
	
	// initialize variables
	setup_znpid.par.kc = 1;
	setup_znpid.par.ki = 0;
	setup_znpid.par.kd = 0;
	setup_znpid.par.SetPoint = 0;
	setup_znpid.par.Err_past = 0;
	setup_znpid.par.dy = 0;
	setup_znpid.par.dx = 0;
	setup_znpid.par.integral = 0;
	setup_znpid.par.derivative = 0;
	setup_znpid.par.PV = 0;
	setup_znpid.par.OP = 0;
	// V-table
	setup_znpid.set_kc = ZNPID_set_kc;
	setup_znpid.set_ki = ZNPID_set_ki;
	setup_znpid.set_kd = ZNPID_set_kd;
	setup_znpid.set_SP = ZNPID_set_SP;
	setup_znpid.output = ZNPID_output;
	
	return setup_znpid;
}

/*** Procedure and Function definition ***/
void ZNPID_set_kc(ZNPID_Parameter* par, double kc)
{
	par->kc = kc;
}
void ZNPID_set_ki(ZNPID_Parameter* par, double ki)
{
	par->ki = ki;
}
void ZNPID_set_kd(ZNPID_Parameter* par, double kd)
{	
	par->kd = kd;
}
void ZNPID_set_SP(ZNPID_Parameter* par, double setpoint)
{
	par->SetPoint = setpoint;
}

double ZNPID_output(ZNPID_Parameter* par, double PV, double timelapse)
{
	double result;
	double tmp;

	par->PV = PV;
	par->dy = ZNPID_delta(par->SetPoint, PV);
	par->dx = timelapse;

	result = ZNPID_product(par->kc, par->dy);
	
	tmp = ZNPID_product(par->ki, ZNPID_integral(par, PV, timelapse));
	result = ZNPID_sum(result, tmp);
	
	tmp = ZNPID_product(par->kd, ZNPID_derivative(par, PV, timelapse));
	result = ZNPID_sum(result, tmp);
	
	par->Err_past = par->dy;
	par->OP = result;

	if(result > ZNPID_outMAX)
		par->integral = ZNPID_outMAX - (par->dy * par->dx) - (par->derivative * par->dx * par->dx);
	else if(result < ZNPID_outMIN)
		par->integral = ZNPID_outMIN + (par->dy * par->dx) + (par->derivative * par->dx * par->dx);
	
	return result;
}

double ZNPID_integral(ZNPID_Parameter* par, double PV, double timelapse)
{
	double tmp = ZNPID_product(ZNPID_sum(ZNPID_delta(par->SetPoint, PV), par->Err_past), timelapse) / 2.0;
	return (par->integral += tmp);
}

double ZNPID_derivative(ZNPID_Parameter* par, double PV, double timelapse)
{
	double tmp = ZNPID_delta(ZNPID_delta(par->SetPoint, PV), par->Err_past);
	return (par->derivative = (tmp / timelapse));
}

double ZNPID_delta(double present_value, double past_value)
{
	return (present_value - past_value);
}

double ZNPID_sum(double value_1, double value_2)
{
	return (value_1 + value_2);
}

double ZNPID_product(double value_1, double value_2)
{
	return (value_1 * value_2);
}

/*** EOF ***/

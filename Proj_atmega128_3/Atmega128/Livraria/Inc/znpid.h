/************************************************************************
	ZNPID – Ziglor and Nichol PID Controller
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128
Date:     17022021_start
************************************************************************/
#ifndef _ZNPID_H_
#define _ZNPID_H_

/*** Library ***/
#include <inttypes.h>

/*** Constant & Macro ***/
#define ZNPID_outMAX 1023
#define ZNPID_outMIN -1023

/*** Parameter ***/
typedef struct {
	double kc;           // Proportional gain
	double ki;           // Integral gain
	double kd;           // Derivative gain
	double SetPoint;     // Desired target value
	double Err_past;     // Previous error
	double dy;           // Error difference (current - previous)
	double dx;           // Time difference (delta t)
	double integral;     // Integrated error (I term)
	double derivative;   // Derivative of error (D term)
	double PV;           // Process variable (measured value)
	double OP;           // Output signal
} ZNPID_Parameter;

/*** Handler ***/
typedef struct {
	ZNPID_Parameter par;

	// Function pointer table (v-table)
	void (*set_kc)(ZNPID_Parameter* par, double kc);
	void (*set_ki)(ZNPID_Parameter* par, double ki);
	void (*set_kd)(ZNPID_Parameter* par, double kd);
	void (*set_SP)(ZNPID_Parameter* par, double setpoint);
	double (*output)(ZNPID_Parameter* par, double PV, double timelapse);
} ZNPID_Handler;

ZNPID_Handler znpid_enable(void);

#endif
/*** EOF ***/

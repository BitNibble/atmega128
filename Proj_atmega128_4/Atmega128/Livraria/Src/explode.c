/********************************************************************
	EXPLODE
Author:   <sergio.salazar.santos@gmail.com> 
License:  GNU General Public License
Hardware: all
Date:     16032021
********************************************************************/
/*** Library ***/
#include"explode.h"

/*** Procedure and Function declaration ***/
void EXPLODEupdate(explode_parameter* par, IO_var x);
IO_var EXPLODEhh(explode_parameter* par);
IO_var EXPLODEll(explode_parameter* par);
IO_var EXPLODElh(explode_parameter* par);
IO_var EXPLODEhl(explode_parameter* par);

/*** Handler ***/
EXPLODE explode_enable( void )
{
	// Parameter
	EXPLODE setup_explode = {
		.par = {
			.HH = 0,
			.HL = 0,
			.LH = 0,
			.LL = 0,
			.XF = 0,
			.XI = 0
		},
		// V-table
		.update = EXPLODEupdate
	};

	return setup_explode;
}
/*** Procedure and Function definition ***/
void EXPLODEupdate(explode_parameter* par, IO_var x)
{
	par->XI = par->XF;
	par->XF = x;
	par->HH = EXPLODEhh(par);
	par->LL = EXPLODEll(par);
	par->LH = EXPLODElh(par);
	par->HL = EXPLODEhl(par);
}
IO_var EXPLODEhh(explode_parameter* par)
{
	return (par->XI & par->XF);
}
IO_var EXPLODEll(explode_parameter* par)
{
	return ~(par->XI | par->XF);
}
IO_var EXPLODElh(explode_parameter* par)
{
	IO_var i;
	i = par->XI ^ par->XF;
	i &= par->XF;
	return i;
}
IO_var EXPLODEhl(explode_parameter* par)
{
	IO_var i;
	i = par->XF ^ par->XI;
	i &= par->XI;
	return i;
}

/*** EOF ***/


/*************************************************************************
	LFSM
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:     25102020
Comment:
	page=1 is dedicated for global logic, if page>1 is for local logic.
	purpose is for machine programming, and encoders. General purpose algorithm.
*************************************************************************/
/*** library ***/
#include "lfsm.h"

/*** Constant & Macro ***/
#define NPAGES 255 // 1 to 254 possible pages
#define BYTEL 0
#define BYTEH 65535

/*** Variable ***/
struct LFSM_Data data, tmp1, tmp2;
const uint16_t tmask = 0xFFFF;

/*** Procedure and Function declaration ***/
uint8_t LFSMread(struct model_lfsm *r, uint8_t input);
uint8_t LFSMlearn(struct model_lfsm *r, const uint8_t input, const uint16_t next, const uint16_t mask, const uint8_t page);
uint16_t LFSMquant(struct model_lfsm *r);
uint8_t LFSMremove(struct model_lfsm *r, uint8_t input);
uint8_t LFSMpremove(struct model_lfsm *r, uint8_t input, uint8_t page);
uint8_t LFSMdeleteall(struct model_lfsm *r);
uint16_t LFSMgetoutput(struct model_lfsm *r);
uint8_t LFSMgetstatus(struct model_lfsm *r);
void LFSMsetoutput(struct model_lfsm *r, uint16_t output);
uint8_t LFSMgetpage(struct model_lfsm *r);
void LFSMsetpage(struct model_lfsm *r, uint8_t page);
uint16_t LFSMlh(uint16_t xi, uint16_t xf);
uint16_t LFSMhl(uint16_t xi, uint16_t xf);
uint16_t LFSMoutputcalc(uint16_t feedback, uint16_t hl, uint16_t lh, uint16_t mask);

/*** Handler ***/
LFSM_Handler lfsm_enable(EEPROM0_Handler* eeprom, const uint16_t sizeeeprom)
{
	// Local Variable
	const uint8_t sizeblock = sizeof(struct LFSM_Data);
	// Create Object
	LFSM_Handler r;
	// Initialize variables
	r.eprom = eeprom;
	r.sizeeeprom = sizeeeprom;
	r.sizeblock = sizeblock;
	r.page = 0;
	r.output = 0; // Output
	// V-table
	r.read = LFSMread;
	r.learn = LFSMlearn;
	r.quant = LFSMquant;
	r.remove = LFSMremove;
	r.premove = LFSMpremove;
	r.deleteall = LFSMdeleteall;
	r.getoutput = LFSMgetoutput;
	r.getstatus = LFSMgetstatus;
	r.setoutput = LFSMsetoutput;
	r.getpage = LFSMgetpage;
	r.setpage = LFSMsetpage;
	
	return r;
}

/*** Procedure and Function definition ***/
// uint8_t LFSMread(struct lfsm *r, uint8_t input)
uint8_t LFSMread(struct model_lfsm *r, uint8_t input)
{
	uint16_t i1;
	uint8_t status = 0;
	uint8_t sizeblock = r->sizeblock;
	struct LFSM_Data* pdata = &data;
	int16_t n = 0;
	uint8_t n1 = NPAGES, n2 = NPAGES;
	uint8_t page = r->page;
	uint16_t HL, LH;
	HL = LFSMhl(r->input, input);
	LH = LFSMlh(r->input, input);
	if(HL || LH){ // To not waste time
		status = 1; // New entry
		for(i1 = 0; i1 < r->sizeeeprom; i1++){
			r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
			switch(pdata->page){
				case 0:
					// Do nothing, continue search in status=1.
				break;
				case 1: // Global logic
					if( pdata->inhl == HL && pdata->inlh == LH ){
							i1 = r->sizeeeprom; // exit search
							status = 2; // Global logic exist
					}
				break;
				default: // Local logic
					if( (pdata->feedback & pdata->mask) == (r->output & pdata->mask) && pdata->inhl == HL && pdata->inlh == LH ){
						n = pdata->page - page;	
						if(!n){
							i1 = r->sizeeeprom; // exit search
							status = 3; // Local logic exist in present page
							break;
						}
						if(n > 0){
							if(n < n1){
								tmp1 = *pdata;
								n1 = n;
								status = 4; // Local logic exist in page above
							}
						}
						if(n < 0){
							if(n > -n2){
								tmp2 = *pdata;
								n2 = -n;
								status = 4; // Local logic exist in page bellow
							}
						}
					}
				break;
			}
		}
	}
	// status confirmation
	switch (status){
		case 0: // No entry
		break;
		case 1: // New entry
			// Give warning and pause waiting for decision (very important).
			// To be implemented Learning on flight mode.
			// Serves has flag to main function.
			r->input = input; // Update
		break;
		case 2: // Global logic exist
			r->page = pdata->page;
			r->input = input; // Update
			r->output = LFSMoutputcalc(r->output, pdata->outhl, pdata->outlh, pdata->mask);
			r->status = 2;
		break;
		case 3: // Local logic exist in present page
			r->page = pdata->page;
			r->input = input; // Update
			r->output = LFSMoutputcalc(r->output, pdata->outhl, pdata->outlh, pdata->mask);
			r->status = 3;
		break;
		case 4: // Local logic exist in page above versus local logic exist in page bellow
			// Choose closest page above otherwise closest bellow.
			r->input = input; // update
			if(n1 > 0 && n1 < NPAGES){
				r->page = tmp1.page;
				r->output = LFSMoutputcalc(r->output, tmp1.outhl, tmp1.outlh, tmp1.mask);
				r->status = 41;
			}
			else if(n2 > 0 && n2 < NPAGES){
				r->page = tmp2.page;
				r->output = LFSMoutputcalc(r->output, tmp2.outhl, tmp2.outlh, tmp2.mask);
				r->status = 42;
			}
		break;
		default:
		break;
	}
	return r->status;
}
// uint8_t LFSMlearn(struct lfsm *r, const uint8_t input, const uint16_t next, const uint16_t mask, const uint8_t page)
uint8_t LFSMlearn(struct model_lfsm *r, const uint8_t input, const uint16_t next, const uint16_t mask, const uint8_t page)
{
	uint16_t i1;
	uint8_t status = 0;
	uint8_t sizeblock = r->sizeblock;
	struct LFSM_Data* pdata = &data;
	struct LFSM_Data* ptmp1 = &tmp1;
	uint16_t HL, LH;
	HL = LFSMhl(r->input, input);
	LH = LFSMlh(r->input, input);
	if(page > 0){ // Enable
		if(HL || LH){ // There is a change ?
			for(i1 = 0; i1 < r->sizeeeprom; i1++){
				r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
				if(pdata->page){ // Find if it exists already
					if( (pdata->page == 1 && pdata->inhl == HL && pdata->inlh == LH)
					|| (pdata->page == page && (pdata->feedback & mask) == (r->output & mask) && pdata->inhl == HL && pdata->inlh == LH) ){
						status = 4; // Already exists
						break;
					}
				}
				status = 1; // New entry
			}
		}
	}
	switch (status){
		case 0: // Not enabled
		break;
		case 1: // Record new entry
			ptmp1->page = page;
			ptmp1->feedback = r->output;
			ptmp1->inhl = HL;
			ptmp1->inlh = LH;
			ptmp1->mask = mask;
			if(page > 1){ // Local logic
				ptmp1->outhl = LFSMhl(r->output, next) & mask;
				ptmp1->outlh = LFSMlh(r->output, next) & mask;
			}else{ // Global logic
				ptmp1->outhl = LFSMhl(BYTEH, next) & mask;
				ptmp1->outlh = LFSMlh(BYTEL, next) & mask;
			}
			for(i1 = 0; i1 < r->sizeeeprom; i1++){ // upload to free space in memory
				r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
				if(pdata->page == 0){
					r->eprom->update_block(ptmp1, (void*)(i1 * sizeblock), sizeblock);
					status = 2; // Uploaded to eeprom
					break;
				}
				status = 3; // Memory full
			}
		case 2: // Added new entry
		break;
		case 3: // Memory full
		break;
		case 4: // Already exists
		break;
		default:
		break;
	}// End switch
	return status;
}
// uint16_t LFSMquant(struct lfsm *r)
uint16_t LFSMquant(struct model_lfsm *r)
{
	uint16_t i1;
	uint8_t sizeblock = r->sizeblock;
	struct LFSM_Data* pdata = &data;
	uint16_t programmed;
	for(i1 = 0, programmed = 0; i1 < r->sizeeeprom; i1++){
		r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
		if(pdata->page != 0){ // Count memory used
			programmed++;
		}
	}
	return programmed;
}
// uint8_t LFSMremove(struct lfsm *r, uint8_t input)
uint8_t LFSMremove(struct model_lfsm *r, uint8_t input)
{
	uint16_t k, k1, k2, i1;
	k = k1 = k2 = 0;
	uint8_t status = 0;
	uint8_t sizeblock = r->sizeblock;
	struct LFSM_Data* pdata = &data;
	struct LFSM_Data* ptmp1 = &tmp1;
	ptmp1->page = 0;
	int16_t n = 0;
	uint8_t n1 = NPAGES, n2 = NPAGES;
	uint8_t page = r->page;
	uint16_t HL, LH;
	HL = LFSMhl(r->input, input);
	LH = LFSMlh(r->input, input);
	if(HL || LH){ // To not waste time
		status = 1; // New entry
		for(i1 = 0; i1 < r->sizeeeprom; i1++){
			r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
			switch(pdata->page){
				case 0:
					// Do nothing, continue search in status=1.
				break;
				case 1: // Global logic
					if( pdata->inhl == HL && pdata->inlh == LH ){
						k = i1;
						i1 = r->sizeeeprom;
						status = 2; // Global logic exist
					}
				break;
				default: // Local logic
					if( (pdata->feedback & pdata->mask) == (r->output & pdata->mask) && pdata->inhl == HL && pdata->inlh == LH ){
						n = pdata->page - page;
						if(!n){
							k = i1;
							i1 = r->sizeeeprom;
							status = 3; // Local logic exist in present page
							break;
						}
						if(n > 0){
							if(n < n1){
								k1 = i1;
								n1 = n;
								status = 4; // Local logic exist in page above
							}
						}
						if(n < 0){
							if(n > -n2){
								k2 = i1;
								n2 = -n;
								status = 4; // Local logic exist in page bellow
							}
						}
					}
				break;
			}
		}
	}
	// status confirmation
	switch (status){
		case 0: // No entry
		break;
		case 1: // Unknown entry
		break;
		case 2: // Global logic exist
			r->eprom->update_block(ptmp1, (void*)(k * sizeblock), sizeblock);
		break;
		case 3: // Local logic exist in present page
			r->eprom->update_block(ptmp1, (void*)(k * sizeblock), sizeblock);
		break;
		case 4: // Local logic exist in page above versus local logic exist in page bellow
			// choose closest page above otherwise closest bellow.
			if(n1 > 0 && n1 < NPAGES){
				r->eprom->update_block(ptmp1, (void*)(k1 * sizeblock), sizeblock);
				status = 41;
			}
			else if(n2 > 0 && n2 < NPAGES){
				r->eprom->update_block(ptmp1, (void*)(k2 * sizeblock), sizeblock);
				status = 42;
			}
		break;
		default:
		break;
	}
	return status;
}
// uint8_t LFSMpremove(struct lfsm *r, uint8_t input)
uint8_t LFSMpremove(struct model_lfsm *r, uint8_t input, uint8_t page)
{
	uint16_t k, k1, k2, i1;
	k = k1 = k2 = 0;
	uint8_t status = 0;
	uint8_t sizeblock = r->sizeblock;
	struct LFSM_Data* pdata = &data;
	struct LFSM_Data* ptmp1 = &tmp1;
	ptmp1->page = 0;
	uint16_t HL,LH;
	HL = LFSMhl(r->input, input);
	LH = LFSMlh(r->input, input);
	if(page > 0){ // filter
		if(HL || LH){ // input
			status = 1;
			for(i1 = 0; i1 < r->sizeeeprom; i1++){
				r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
				if(pdata->page == page){ // skip not desired
					if( pdata->inhl == HL && pdata->inlh == LH && page == 1){
						k = i1;
						i1 = r->sizeeeprom;
						status = 2; // Global logic exist
					}else if( (pdata->feedback & pdata->mask) == (r->output & pdata->mask) && pdata->inhl == HL && pdata->inlh == LH && pdata->page == page){
						k = i1;
						i1 = r->sizeeeprom;
						status = 3; // Local logic exist in present page
					}
				}
			}
		}
	}
	// status confirmation
	switch (status){
		case 0: // No entry
		break;
		case 1: // Unknown entry
		break;
		case 2: // Global logic exist in present page
		r->eprom->update_block(ptmp1, (void*)(k * sizeblock), sizeblock);
		break;
		case 3: // Local logic exist in present page
		r->eprom->update_block(ptmp1, (void*)(k * sizeblock), sizeblock);
		break;
		default:
		break;
	}
	return status;
}
// uint8_t LFSMdeleteall(struct lfsm *r)
uint8_t LFSMdeleteall(struct model_lfsm *r)
{
	uint16_t i1;
	uint8_t status = 0;
	uint8_t sizeblock = r->sizeblock;
	struct LFSM_Data* pdata = &data;
	for(i1 = 0; i1 < r->sizeeeprom; i1++){
		r->eprom->read_block(pdata, (struct LFSM_Data*)(i1 * sizeblock), sizeblock);
		if(pdata->page){
			pdata->page = 0;
			r->eprom->update_block(pdata, (void*)(i1 * sizeblock), sizeblock);
			status = 1; // All deleted
		}
	}
	r->output = 0;
	r->status = 0;
	return status;
}
// uint16_t LFSMgetoutput(struct lfsm *r)
uint16_t LFSMgetoutput(struct model_lfsm *r)
{
	return r->output;
}
// uint8_t LFSMgetstatus(struct lfsm *r)
uint8_t LFSMgetstatus(struct model_lfsm *r)
{
	return r->status;
}
// void LFSMsetoutput(struct lfsm *r, uint16_t output)
void LFSMsetoutput(struct model_lfsm *r, uint16_t output)
{
	r->output = output;
	r->status = 0;
}
// uint8_t LFSMgetpage(struct lfsm *r)
uint8_t LFSMgetpage(struct model_lfsm *r)
{
	return r->page;
}
// void LFSMsetpage(struct lfsm *r, uint8_t page)
void LFSMsetpage(struct model_lfsm *r, uint8_t page)
{
	r->page=page;
}
// uint16_t LFSMlh(uint16_t xi, uint16_t xf)***/
uint16_t LFSMlh(uint16_t xi, uint16_t xf)
{
	uint16_t i;
	i = xf ^ xi;
	i &= xf;
	return i;
}
// uint16_t LFSMhl(uint16_t xi, uint16_t xf)
uint16_t LFSMhl(uint16_t xi, uint16_t xf)
{
	uint16_t i;
	i = xf ^ xi;
	i &= xi;
	return i;
}
/**************************************************************************
// uint16_t LFSMoutputcalc(uint16_t feedback, uint16_t hl, uint16_t lh)
uint16_t LFSMoutputcalc(uint16_t feedback, uint16_t hl, uint16_t lh)
{
	feedback |= lh;
	feedback &= ~hl;
	return feedback;
}
**************************************************************************/
// uint16_t LFSMoutputcalc(uint16_t feedback, uint16_t hl, uint16_t lh, uint16_t mask)
uint16_t LFSMoutputcalc(uint16_t feedback, uint16_t hl, uint16_t lh, uint16_t mask)
{
	feedback |= (lh & mask);
	feedback &= ~(hl & mask);
	return feedback;
}

/*** EOF ***/


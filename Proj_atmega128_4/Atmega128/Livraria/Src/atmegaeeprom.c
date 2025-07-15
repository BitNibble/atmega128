/*************************************************************************
	eeprom wrapper
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     07/01/2024
*************************************************************************/
/*** Library ***/
#include "atmegaeeprom.h"

/*** Internal State ***/
static EEPROM0 atmega128_eeprom = {
	.read_byte = eeprom_read_byte,
	.write_byte = eeprom_write_byte,
	.update_byte = eeprom_update_byte,
	.read_word = eeprom_read_word,
	.write_word = eeprom_write_word,
	.update_word = eeprom_update_word,
	//.read_dword = eeprom_read_dword,
	.write_dword = eeprom_write_dword,
	.update_dword = eeprom_update_dword,
	//.read_float = eeprom_read_float,
	.write_float = eeprom_write_float,
	.update_float = eeprom_update_float,
	.read_block = eeprom_read_block,
	.write_block = eeprom_write_block,
	.update_block = eeprom_update_block
};

/*** Handler ***/
EEPROM0* eeprom(void){ return &atmega128_eeprom; }

/*** EOF ***/


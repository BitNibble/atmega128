#include "l293d.h"

/*** Internal ***/
void l293d_set_dir(L293D_Param *par, uint8_t mode);
void l293d_set_en(L293D_Param *par, uint8_t state);

/*** Instance ***/
L293D l293d_enable(volatile IO_var *ddr, volatile IO_var *port, uint8_t pin1, uint8_t pin2, uint8_t en_pin) {
    L293D dev = {
        .par = {
            .DDR = ddr,
            .PORT = port,
            .pin1 = pin1,
            .pin2 = pin2,
            .en_pin = en_pin
        },
        .dir = l293d_set_dir,
        .enable = l293d_set_en
    };

    #if defined (STM32F4)
        *ddr &= ~((3 << (pin1 * 2)) | (3 << (pin2 * 2)) | (3 << (en_pin * 2)));
        *ddr |=  ((1 << (pin1 * 2)) | (1 << (pin2 * 2)) | (1 << (en_pin * 2)));
    #else
        *ddr |= (1 << pin1) | (1 << pin2) | (1 << en_pin);
    #endif

    *port &= ~((1 << pin1) | (1 << pin2) | (1 << en_pin));
    return dev;
}

/*** Function ***/
void l293d_set_dir(L293D_Param *par, uint8_t mode) {
    switch (mode) {
        case L293D_FORWARD:
            *par->PORT |= (1 << par->pin1);
            *par->PORT &= ~(1 << par->pin2);
            break;
        case L293D_REVERSE:
            *par->PORT &= ~(1 << par->pin1);
            *par->PORT |= (1 << par->pin2);
            break;
        case L293D_STOP:
        default:
            *par->PORT &= ~((1 << par->pin1) | (1 << par->pin2));
            break;
    }
}

void l293d_set_en(L293D_Param *par, uint8_t state) {
    if (state)
        *par->PORT |= (1 << par->en_pin);
    else
        *par->PORT &= ~(1 << par->en_pin);
}

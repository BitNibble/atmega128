#ifndef L293D_H
#define L293D_H

#include <inttypes.h>

/*** Constant and Macro ***/
#define L293D_FORWARD  1
#define L293D_REVERSE  2
#define L293D_STOP     0
#define L293D_ENABLE   1
#define L293D_DISABLE  0

#if defined (STM32F4)
    #define IO_var uint32_t
#else
    #define IO_var uint8_t
#endif

/*** Parameters ***/
typedef struct {
    volatile IO_var *DDR;
    volatile IO_var *PORT;
    uint8_t pin1;
    uint8_t pin2;
    uint8_t en_pin;
} L293D_Param;

/*** Handler ***/
typedef struct {
    L293D_Param par;
    void (*dir)(L293D_Param *par, uint8_t mode);
    void (*enable)(L293D_Param *par, uint8_t state);
} L293D;

L293D l293d_enable(volatile IO_var *ddr, volatile IO_var *port, uint8_t pin1, uint8_t pin2, uint8_t en_pin);

#endif

/*************************************************************************
	RINGBUFFER
@Author:   <sergio.salazar.santos@gmail.com>
@License:  GNU General Public License
@Hardware: Atmega128 by ETT ET-BASE
@Date:     17/07/2025
*************************************************************************/
#ifndef RINGBUFFER_H
	#define RINGBUFFER_H

/*** Library ***/
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

/*** Handler Parameter Typedef***/
typedef struct {
    uint8_t *buffer;
    uint16_t size;
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t count;
} RingBuffer_Parameter;

/*** Handler Typedef***/
typedef struct {
    RingBuffer_Parameter par;
    // V-table
	bool (* const IsFull)(RingBuffer_Parameter *rb);
	bool (* const IsEmpty)(RingBuffer_Parameter *rb);
	bool (* const Put)(RingBuffer_Parameter *rb, uint8_t data);
	bool (* const Get)(RingBuffer_Parameter *rb, uint8_t *data);
	bool (* const Peek)(RingBuffer_Parameter *par, uint8_t *data);
    void (* const PrintStatus)(RingBuffer_Parameter *par);
    void (* const Clear)(RingBuffer_Parameter *par);
    uint16_t (* const Capacity)(RingBuffer_Parameter *par);
    uint16_t (* const Available)(RingBuffer_Parameter *par);
} RingBuffer_TypeDef;

/*** Handler Generator ***/
RingBuffer_TypeDef RingBuffer_Init(uint8_t *buf, uint16_t size);

#endif


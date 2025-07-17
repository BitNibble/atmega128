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
	bool (*IsFull)(RingBuffer_Parameter *rb);
	bool (*IsEmpty)(RingBuffer_Parameter *rb);
	bool (*Put)(RingBuffer_Parameter *rb, uint8_t data);
	bool (*Get)(RingBuffer_Parameter *rb, uint8_t *data);
	bool (*Peek)(RingBuffer_Parameter *par, uint8_t *data);
    void (*PrintStatus)(RingBuffer_Parameter *par);
    void (*Clear)(RingBuffer_Parameter *par);
    uint16_t (*Capacity)(RingBuffer_Parameter *par);
    uint16_t (*Available)(RingBuffer_Parameter *par);
} RingBuffer_Handler;

/*** Handler Generator ***/
RingBuffer_Handler RingBuffer_Init(uint8_t *buf, uint16_t size);

#endif


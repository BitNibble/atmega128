/*************************************************************************
	RINGBUFFER
@Author:   <sergio.salazar.santos@gmail.com>
@License:  GNU General Public License
@Hardware: Atmega128 by ETT ET-BASE
@Date:     17/07/2025
*************************************************************************/
#include "ringbuffer.h"
#include <stdio.h>

bool RingBuffer_IsFull(RingBuffer_Parameter *par) {
    return par->count == par->size;
}

bool RingBuffer_IsEmpty(RingBuffer_Parameter *par) {
    return par->count == 0;
}

bool RingBuffer_Put(RingBuffer_Parameter *par, uint8_t data) {
    if (RingBuffer_IsFull(par)) return false;
    par->buffer[par->head++] = data;
    if (par->head > par->size) par->head = 0;
    par->count++;
    return true;
}

bool RingBuffer_Get(RingBuffer_Parameter *par, uint8_t *data) {
    if (RingBuffer_IsEmpty(par)) return false;
    *data = par->buffer[par->tail++];
    if (par->tail > par->size) par->tail = 0;
    par->count--;
    return true;
}

bool RingBuffer_Peek(RingBuffer_Parameter *par, uint8_t *data) {
    if (RingBuffer_IsEmpty(par)) return false;
    *data = par->buffer[par->tail];
    return true;
}

void RingBuffer_PrintStatus(RingBuffer_Parameter *par) {
    printf("Head: %u, Tail: %u, Count: %u, Size: %u\n", par->head, par->tail, par->count, par->size);
}

void RingBuffer_Clear(RingBuffer_Parameter *par) {
    par->head = 0;
    par->tail = 0;
    par->count = 0;
}

uint16_t RingBuffer_Capacity(RingBuffer_Parameter *par) {
    return par->size;
}

uint16_t RingBuffer_Available(RingBuffer_Parameter *par) {
    return par->size - par->count;
}

/*** Handler Generator ***/
RingBuffer_Handler RingBuffer_Init(uint8_t *buf, uint16_t size) {
	RingBuffer_Handler setup = {
		.par = {
			.buffer = buf,
			.size = (size - 1),
			.head = 0,
			.tail = 0,
			.count = 0
		},
		.IsFull = RingBuffer_IsFull,
		.IsEmpty = RingBuffer_IsEmpty,
		.Put = RingBuffer_Put,
		.Get = RingBuffer_Get,
		.Peek = RingBuffer_Peek,
        .PrintStatus = RingBuffer_PrintStatus,
        .Clear = RingBuffer_Clear,
        .Capacity = RingBuffer_Capacity,
        .Available = RingBuffer_Available
	};
    return setup;
}


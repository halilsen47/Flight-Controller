/*
 * buffer.c
 *
 *  Created on: 9 May 2026
 *      Author: senha
 */

#include "boundedBuffer.h"


void buffer_init(buffer_t *buffer)
{
	buffer->count = 0;
	buffer->head = 0;
	buffer->tail = 0;
}

void buffer_push(buffer_t *buffer ,uint8_t data)
{
	buffer->buffer[buffer->head] = data;
	buffer->head++;
	if(buffer->head == 512)
		buffer->head = 0;
	if(buffer->count < 512)
		buffer->count++;
}

uint8_t buffer_pop(buffer_t *buffer)
{
	if(buffer->count == 0)
		return 0;
	uint8_t res = buffer->buffer[buffer->tail];
	buffer->tail++;
	if(buffer->tail >= 512)
		buffer->tail = 0;
	buffer->count--;
	return res;
}

uint8_t buffer_peek(buffer_t *buffer)
{
	if (buffer->count == 0) {
	        return 0;
	}
	uint8_t res = buffer->buffer[buffer->tail];
	return res;
}
void buffer_peek_multiple(buffer_t *buffer,uint8_t *data,uint8_t size)
{
	for(int i = 0; i < size; i++)
	{
		uint16_t idx = (buffer->tail + i) % 512;
		data[i] = buffer->buffer[idx];
	}
}

/*
 * boundedBuffer.h
 *
 *  Created on: 9 May 2026
 *      Author: senha
 */

#ifndef INC_BOUNDEDBUFFER_H_
#define INC_BOUNDEDBUFFER_H_
#include "main.h"
#include <stdint.h>

typedef struct
{
	uint8_t buffer[512];
	uint8_t head;
	uint8_t tail;
	uint8_t count;

}buffer_t;

void buffer_init(buffer_t *buffer);
void buffer_push(buffer_t *buffer ,uint8_t data);
uint8_t buffer_pop(buffer_t *buffer);
uint8_t buffer_peek(buffer_t *buffer);
void buffer_peek_multiple(buffer_t *buffer,uint8_t *data,uint8_t size);
#endif /* INC_BOUNDEDBUFFER_H_ */

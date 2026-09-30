/*
 * crsf_receiver.h
 *
 *  Created on: 9 May 2026
 *      Author: senha
 */

#ifndef INC_CRSF_RECEIVER_H_
#define INC_CRSF_RECEIVER_H_
#include "main.h"
#include <stdint.h>
#include "boundedBuffer.h"
typedef struct
{
	//there have a 16 channels but ı need only 8
	uint16_t rc_channels[8];
	uint16_t rc_values[8];
	int16_t rc_deg[8];
	uint8_t is_failsafe;
	uint16_t last_packet_time;

}CRSF_Data_t;


void CRSF_Init(CRSF_Data_t *data);
void CRSF_Process(buffer_t *buf, CRSF_Data_t *data);
void CRSF_DecodeChannels(uint8_t *payload, uint16_t *destChannels);
void CRSF_To_Us(CRSF_Data_t *data);
void CRSF_To_deg(CRSF_Data_t *data);
#endif /* INC_CRSF_RECEIVER_H_ */

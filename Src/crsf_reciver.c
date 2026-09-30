/*
 * crsf_reciver.c
 *
 *  Created on: 9 May 2026
 *      Author: senha
 */


#include "crsf_receiver.h"
#include <string.h>
#include "boundedBuffer.h"
#include "main.h"

void CRSF_Init(CRSF_Data_t *data)
{
	data->is_failsafe = 1;
	memset(data->rc_channels, 0, sizeof(data->rc_channels));
	memset(data->rc_values, 0, sizeof(data->rc_values));memset(data->rc_channels, 0, sizeof(data->rc_channels));
}

void CRSF_Process(buffer_t *buf, CRSF_Data_t *data)
{

	//Header ara -> Uzunluk oku -> Veri Topla -> paket çöz

		if(buf->count == 0)
		{
			return;
		}

		static uint8_t state = 0;
		static uint8_t index = 2;
		static uint8_t frame[26];

		switch (state) {
			case 0:
				// Start Header Ara
				if(buffer_peek(buf) == 0xC8)
				{
					buffer_pop(buf);
					state = 1;
				}
				else
					buffer_pop(buf);

				break;

			case 1:
				// Uzunluk ve header oku
				if(buf->count > 2)
				{
					frame[0] = buffer_pop(buf);
					frame[1] = buffer_pop(buf);
				}

				if(frame[1] == 0x16 && frame[0] == 24)
					state = 2;
				else
					state = 0;
				break;

			case 2:
				// Veri Topla
				frame[index] = buffer_pop(buf);
				index++;

				if(index == 26)
				{
					state = 3;
				}
				break;

			case 3:
				// Paket Çöz
				data->is_failsafe = 0;
				CRSF_DecodeChannels(&frame[2], data->rc_channels);
				CRSF_To_Us(data);
				data->last_packet_time = HAL_GetTick();


				state = 0;
				index = 2;
				break;

			default:
				state = 0;
				index = 2;
				break;
		}

}
void CRSF_DecodeChannels(uint8_t *payload, uint16_t *destChannels)
{
		// Kanal 0 (Genellikle Roll)
	    destChannels[0]  = (uint16_t)((payload[0]       | payload[1] << 8) & 0x07FF);
	    // Kanal 1 (Genellikle Pitch)
	    destChannels[1]  = (uint16_t)((payload[1] >> 3  | payload[2] << 5) & 0x07FF);
	    // Kanal 2 (Genellikle Throttle)
	    destChannels[2]  = (uint16_t)((payload[2] >> 6  | payload[3] << 2 | payload[4] << 10) & 0x07FF);
	    // Kanal 3 (Genellikle Yaw)
	    destChannels[3]  = (uint16_t)((payload[4] >> 1  | payload[5] << 7) & 0x07FF);
	    // Kanal 4 (Aux 1)
		destChannels[4]  = (uint16_t)((payload[5] >> 4  | payload[6] << 4) & 0x07FF);


		//unused channels maybe ı use in later
		destChannels[5]  = (uint16_t)((payload[6] >> 7  | payload[7] << 1 | payload[8] << 9) & 0x07FF);
		destChannels[6]  = (uint16_t)((payload[8] >> 2  | payload[9] << 6) & 0x07FF);
		destChannels[7]  = (uint16_t)((payload[9] >> 5  | payload[10] << 3) & 0x07FF);

		/*
		 * Other channels
		destChannels[8]  = (uint16_t)((payload[11]      | payload[12] << 8) & 0x07FF);
		destChannels[9]  = (uint16_t)((payload[12] >> 3 | payload[13] << 5) & 0x07FF);
		destChannels[10] = (uint16_t)((payload[13] >> 6 | payload[14] << 2 | payload[15] << 10) & 0x07FF);
		destChannels[11] = (uint16_t)((payload[15] >> 1 | payload[16] << 7) & 0x07FF);
		destChannels[12] = (uint16_t)((payload[16] >> 4 | payload[17] << 4) & 0x07FF);
		destChannels[13] = (uint16_t)((payload[17] >> 7 | payload[18] << 1 | payload[19] << 9) & 0x07FF);
		destChannels[14] = (uint16_t)((payload[19] >> 2 | payload[20] << 6) & 0x07FF);
		destChannels[15] = (uint16_t)((payload[20] >> 5 | payload[21] << 3) & 0x07FF);
		*/
}

void CRSF_To_Us(CRSF_Data_t *data) {
    // Formül: (crsf_val - 172) * (2000 - 1000) / (1811 - 172) + 1000
	for(int i = 0; i < 8; i++)
	{
		uint16_t ham_veri = data->rc_channels[i];
		if (ham_veri <= 172)
			data->rc_values[i] = 1000;
		else if (ham_veri >= 1811)
			data->rc_values[i] = 2000;

		else {
			data->rc_values[i] = (uint16_t)(((ham_veri - 172) * 1000) / 1639) + 1000;
		}
	}
	CRSF_To_deg(data);

}
void CRSF_To_deg(CRSF_Data_t *data)
{
	float rate = 720.00f / 500.00f;
	int16_t roll 	 = (int16_t)((data->rc_values[0] - 1500) * rate);
	int16_t pitch 	 = (int16_t)((data->rc_values[1] - 1500) * rate);
	int16_t throttle = data->rc_values[2] - 1000;
	int16_t yaw 	 = (int16_t)((data->rc_values[3] - 1500)  * rate);

	data->rc_deg[0] = (roll < -715 ? -720 : roll);
	data->rc_deg[1] = (pitch < -715 ? -720 : pitch);
	data->rc_deg[2] = (throttle < 2 ? 0 : throttle);
	data->rc_deg[3] = (yaw < -715 ? -720 : yaw);
	data->rc_deg[4] = (data->rc_values[4] > 1500 ? 1 : 0);
}

/*******************************************************************************
 * Message structure for Egret Throttle Controller                             *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include "msg.h"
#include "uart.h"

#define MSG_BYTE_START    0
#define MSG_BYTE_SPEED   1
#define MSG_BYTE_CONST1   2 /* always 0xf6 */
#define MSG_BYTE_FLAGS    3
#define MSG_BYTE_CONST2   4 /* always 0x00 */
#define MSG_BYTE_MODE     5
#define MSG_BYTE_CHKSUM   6

static throttle_msg_t msg;
static uint8_t data[THROTTLE_MSG_SIZE];
static uint8_t data_ptr;


throttle_msg_t *msg_parse(void)
{
	while (uart0_bytes_available())
	{
		uint8_t tmp = uart0_read();
		if (tmp == THROTTLE_MSG_START)
		{
			data_ptr = 0;
		}
		if (data_ptr < sizeof(data))
		{
			data[data_ptr++] = tmp;
		}
		if (data_ptr == sizeof(data))
		{
			data_ptr++; /* do not parse same message twice */
			if (data[MSG_BYTE_CHKSUM] == msg_checksum(&data[1], sizeof(data) - 2))
			{
				msg.speed = data[MSG_BYTE_SPEED];
				msg.flags = data[MSG_BYTE_FLAGS] & THROTTLE_FLAG_MASK;
				msg.mode = data[MSG_BYTE_MODE];
				return &msg;
			}
		}
	}
	return 0;
}

void msg_send(const throttle_msg_t *msg)
{
	uint8_t data[THROTTLE_MSG_SIZE];
	if (!msg)
	{
		return;
	}

	data[MSG_BYTE_START]  = THROTTLE_MSG_START;
	data[MSG_BYTE_SPEED]  = msg->speed;
	data[MSG_BYTE_CONST1] = 0xf6; /* always constant, no clue what it means */
	data[MSG_BYTE_FLAGS]  = (msg->flags & THROTTLE_FLAG_MASK) | 0x06; /* lower nibble always set to 0x06, meaning??? */
	data[MSG_BYTE_CONST2] = 0; /* always zero, meaning??? */
	data[MSG_BYTE_MODE]   = msg->mode;
	data[MSG_BYTE_CHKSUM] = msg_checksum(&data[1], sizeof(data) - 2);
	uart0_write_buffer(data, sizeof(data));
}


uint8_t msg_checksum(const uint8_t *data, uint8_t size)
{
	if (data && size)
	{
		uint8_t sum = data[0];
		for (uint8_t i = 1; i < size; ++i)
		{
			sum ^= data[i];
		}
		return sum;
	}
	return 0;
}

/*******************************************************************************
 * Message structure for Egret Throttle Controller                             *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include "msg.h"
#include "uart.h"
#include "config.h"

#define THROTTLE_V4     0
#define THROTTLE_V3X    1

#define MSG_BYTE_START    0
#define MSG_BYTE_SPEED    1
#define MSG_BYTE_WMODE    2 /* mode and wheel size on V4 FW9.3, always 0xf6 for old V4 and V3X */
#define MSG_BYTE_FLAGS    3
#define MSG_BYTE_CONST    4 /* always 0x00 */
#define MSG_BYTE_TORQUE   5
#define MSG_BYTE_CHKSUM   6

#define THROTTLE_MSG_START 0xFD
#define THROTTLE_MSG_SIZE  7

#define MSG_WMODE_OLD 0xf6 // for old V4 (FW_VER <9.3) and V3X

/**
 * MSG_BYTE_TORQUE (5) seems to be related to max. torque but can also be used
 * to determine which mode/gear is currently configured by the user.
 * These values differ slightly on V4 vs. V3X throttle control.
 * Since FW_Ver.9.3 (V4 only) mode can be read from MSG_BYTE_WMODE (upper 4 Bit)
 */
#if THROTTLE_VERSION == THROTTLE_V4
static const uint8_t THROTTLE_TORQUE_MODE0 = 0x00;
static const uint8_t THROTTLE_TORQUE_MODE1 = 0x05;
static const uint8_t THROTTLE_TORQUE_MODE2 = 0x07;
static const uint8_t THROTTLE_TORQUE_MODE3 = 0x09;
static const uint8_t THROTTLE_TORQUE_MODE4 = 0x0B;
static const uint8_t THROTTLE_TORQUE_MODE5 = 0x0D;
#endif
#if THROTTLE_VERSION == THROTTLE_V3X
static const uint8_t THROTTLE_TORQUE_MODE0 = 0x00;
static const uint8_t THROTTLE_TORQUE_MODE1 = 0x05;
static const uint8_t THROTTLE_TORQUE_MODE2 = 0x06;
static const uint8_t THROTTLE_TORQUE_MODE3 = 0x07;
static const uint8_t THROTTLE_TORQUE_MODE4 = 0x09;
static const uint8_t THROTTLE_TORQUE_MODE5 = 0x0D;
#endif

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
				msg.torque = data[MSG_BYTE_TORQUE];
				// V4 FW_VER 9.3: try to parse wheel size and mode...
				switch (data[MSG_BYTE_WMODE] & 0x0f)
				{
					case THROTTLE_WHEEL_SIZE08: // fall
					case THROTTLE_WHEEL_SIZE10:
						msg.wsize = (throttle_wsize_t)(data[MSG_BYTE_WMODE] & 0x0f);
						//msg.mode = (throttle_mode_t)(data[MSG_BYTE_WMODE] & 0xf0);
						break;
					default:
						msg.wsize = THROTTLE_WHEEL_UNKNOWN;
						//msg.mode = THROTTLE_MODE_UNKNOWN;
				}
				/* Stay compatible with older firmware versions and use torque
				 * to determine mode (with FW_VER 9.3 this could be read from
				 * MSG_BYTE_WMODE as well)
				 */
				switch (data[MSG_BYTE_TORQUE])
				{
					case THROTTLE_TORQUE_MODE0:
						msg.mode = THROTTLE_MODE0;
						break;
					case THROTTLE_TORQUE_MODE1:
						msg.mode = THROTTLE_MODE1;
						break;
					case THROTTLE_TORQUE_MODE2:
						msg.mode = THROTTLE_MODE2;
						break;
					case THROTTLE_TORQUE_MODE3:
						msg.mode = THROTTLE_MODE3;
						break;
					case THROTTLE_TORQUE_MODE4:
						msg.mode = THROTTLE_MODE4;
						break;
					case THROTTLE_TORQUE_MODE5:
						msg.mode = THROTTLE_MODE5;
						break;
					default:
						msg.mode = THROTTLE_MODE_UNKNOWN;
				}
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
	const uint8_t fw_ver93 = (msg->wsize != THROTTLE_WHEEL_UNKNOWN);

	data[MSG_BYTE_START]  = THROTTLE_MSG_START;
	data[MSG_BYTE_SPEED]  = msg->speed;
	data[MSG_BYTE_WMODE]  = fw_ver93 ? (msg->mode | msg->wsize) : MSG_WMODE_OLD;
	data[MSG_BYTE_FLAGS]  = (msg->flags & THROTTLE_FLAG_MASK) | 0x06; /* lower nibble always set to 0x06, meaning??? */
	data[MSG_BYTE_CONST]  = 0; /* always zero, meaning??? */
	data[MSG_BYTE_TORQUE]  = msg->torque;
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

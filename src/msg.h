/*******************************************************************************
 * Message structure for Egret Throttle Controller                             *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef MSG_H_
#define MSG_H_

#include <stdint.h>
#include "timer.h"


typedef enum
{
	THROTTLE_MODE0 = 0x00,
	THROTTLE_MODE1 = 0x10,
	THROTTLE_MODE2 = 0x20,
	THROTTLE_MODE3 = 0x30,
	THROTTLE_MODE4 = 0x40,
	THROTTLE_MODE5 = 0x50,
	THROTTLE_MODE_UNKNOWN = 0xf0
} throttle_mode_t;

typedef enum
{
	THROTTLE_WHEEL_UNKNOWN = 0x06, // old V4 (<FW_VER 9.3) or V3X
	THROTTLE_WHEEL_SIZE08 =  0x07,
	THROTTLE_WHEEL_SIZE10 =  0x09
} throttle_wsize_t;

/**
 * @brief throttle_flags_t Byte 3 of throttle control holds flags for controlling
 *                         light, cruise control and whether the scooter needs
 *                         to be pushed before the motor can be started. This
 *                         byte is not yet fully deciphered, so there may be
 *                         more to come...
 */
typedef enum
{
	FLAG_NONE           = 0x00,
	FLAG_START_WNOPUSH  = 0x20,
	FLAG_CRUISE_CONTROL = 0x40,
	FLAG_LIGHT_ENABLED  = 0x80
} throttle_flags_t;

#define THROTTLE_FLAG_MASK 0xE0

/**
 * @brief throttle_msg_t holds all information of a message sent by the throttle
 *                       control unit
 * @param speed defines max. speed and torque
 * @param mode  seems to have some effects to torque at low speed, but we make
 *              use of it for detecting mode/gear, @see throttle_mode_t
 * @param flags light on/off, cruise control enabled, start without push
 * @see throttle_flags_t
 */
typedef struct
{
	uint8_t speed;
	uint8_t torque;
	throttle_mode_t mode;
	throttle_wsize_t wsize;
	throttle_flags_t flags;
} throttle_msg_t;


/**
 * @brief msg_parse parses data received on uart0
 * @return pointer to message, stays vaild until msg_parse is called again
 */
throttle_msg_t *msg_parse(void);

/**
 * @brief msg_data converts throttle message into raw data (including start
 *                 delimiter, to be send on uart)
 * @param msg pointer to message
 */
void msg_send(const throttle_msg_t *msg);

/**
 * @brief msg_checksum calculates Xor8 checksum
 * @param data pointer (should not include message start delimiter)
 * @param size of data
 * @return xor8 checksum
 */
uint8_t msg_checksum(const uint8_t *data, uint8_t size);


#endif /* MSG_H_ */

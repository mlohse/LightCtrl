/*******************************************************************************
 * Message structure for Egret Throttle Controller                             *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef MSG_H_
#define MSG_H_

#include <stdint.h>
#include "config.h"
#include "timer.h"

#define THROTTLE_MSG_START 0xFD
#define THROTTLE_MSG_SIZE  7

#define THROTTLE_V4     0
#define THROTTLE_V3X    1

/**
 * @brief throttle_mode_t Byte 5 of throttle control seems to be related to max.
 *                        torque but can also be used to determine which mode/gear
 *                        is currently configured by the user. These values
 *                        differ slightly on V4 vs. V3X throttle control.
 */
typedef enum
{
#if THROTTLE_VERSION == THROTTLE_V4
	THROTTLE_MODE0 = 0x00,
	THROTTLE_MODE1 = 0x05,
	THROTTLE_MODE2 = 0x07,
	THROTTLE_MODE3 = 0x09,
	THROTTLE_MODE4 = 0x0B,
	THROTTLE_MODE5 = 0x0D
#endif
#if THROTTLE_VERSION == THROTTLE_V3X
	THROTTLE_MODE0 = 0x00,
	THROTTLE_MODE1 = 0x05,
	THROTTLE_MODE2 = 0x06,
	THROTTLE_MODE3 = 0x07,
	THROTTLE_MODE4 = 0x09,
	THROTTLE_MODE5 = 0x0D
#endif
} throttle_mode_t;

/**
 * @brief throttle_flags_t Byte 3 of throttle control holds flags for controlling
 *                         light, cruise control and whether the scooter needs
 *                         to be pushed before the motor can be started. This
 *                         byte is not yet fully deciphered, so there may be
 *                         more to come...
 */
typedef enum
{
	START_WNOPUSH_FLAG  = 0x20,
	CRUISE_CONTROL_FLAG = 0x40,
	LIGHT_ENABLED_FLAG  = 0x80
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
	uint8_t mode;
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

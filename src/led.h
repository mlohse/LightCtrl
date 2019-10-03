/*******************************************************************************
 * LED driver                                                                  *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef LED_H_
#define LED_H_

#include <stdint.h>

/**
 * @brief led_flash flashes all LEDs on startup
 */
void led_flash(void); /* must be called after timer has been initialized! */

/**
 * @brief led_on switches a led on
 * @param num number of led, see config.h
 */
void led_on(uint8_t num);

/**
 * @brief led_off switches a led off
 * @param num number of led, see config.h
 */
void led_off(uint8_t num);

/**
 * @brief led_toggle toggles a led
 * @param num number of led, see config.h
 */
void led_toggle(uint8_t num);

/**
 * @brief led_set switches a led on/off
 * @param num number of led, see config.h
 * @param enabled 0 = off, 1 = on
 */
void led_set(uint8_t num, uint8_t enabled);

#endif /* LED_H_ */

/*******************************************************************************
 * Sleep/Power Management                                                      *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef SLEEP_H_
#define SLEEP_H_

#include <stdint.h>

/**
 * @brief sleep_ready ckecks whether device can enter sleep mode (to save power)
 *                    which is the case when 'SLEEP_WAKEUP_PCINT'has been cleared
 *                    for more than 'SLEEP_ACT_TIMEOUT' msecs (see config.h)
 * @return 0 = stay awake, 1 = device can enter sleep
 */
uint8_t sleep_ready(void);

/**
 * @brief sleep_powerdown enables pin change interrupt for 'SLEEP_WAKEUP_PCINT'
 *                        and the brings MCU into powerdown mode
 * @note MCU will shortly wake up if any other irq then pin change is triggered,
 *       but will then fall asleep again
 */
void sleep_powerdown(void);

#endif /* SLEEP_H_ */

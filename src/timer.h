/*******************************************************************************
 * Timer providing simple sleep and uptime functions                           *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

/**
 * @brief timer_init initializes timer0 and uptime
 */
void timer_init(void);

/**
 * @brief timer_msleep waits/blocks for 'ms' number of msecs
 * @param ms timeout in msecs
 * @note irq handlers are still being executed
 * @note do not call @see timer_clear() from within an irq-handler or
 *       timer_msleep() may take longer than expected, way longer!
 */
void timer_msleep(uint32_t ms);

/**
 * @brief timer_uptime Provides the uptime in msecs
 * @return uptime in msecs since powerup/last @see timer_clear() invokation
 * @note timer resulution is 5ms @3.6864MHz
 * @note please keep in mind: uptime will overflow every ~49.7 days!
 */
uint32_t timer_uptime(void); // uptime in ms

/**
 * @brief timer_clear
 */
void timer_clear(void);

#endif /* TIMER_H_ */

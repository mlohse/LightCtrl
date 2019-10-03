/*******************************************************************************
 * Light controller application                                                *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include <stdint.h>
#include "config.h"
#include "ctrl.h"
#include "msg.h"
#include "led.h"
#include "adc.h"
#include "uart.h"
#include "sleep.h"


static uint8_t ctrl_autolight(void)
{
	static uint8_t lighton = 0;
	uint16_t val = adc_read();
	if (val < LIGHT_SEND_THRES_ON)
	{
		lighton = 1;
	}
	if (val > LIGHT_SENS_THRES_OFF)
	{
		lighton = 0;
	}
	return lighton;
}

void ctrl_run(void)
{
	throttle_msg_t *msg;
	static uint8_t autolight = 1;

	if (uart0_bytes_available())
	{
		led_on(LED_ACT);
	}

	msg = msg_parse();
	if (msg)
	{
		uint8_t light = (msg->flags & LIGHT_ENABLED_FLAG) ? 1 : 0;
		switch(msg->mode)
		{
			case THROTTLE_MODE0:
				if (!light) /* (re-)enable autolight by switching to Mode0 */
				{
					autolight = 1;
				}
				break;
			default: /* THROTTLE_MODE1..5 */
				if (light) /* disable autolight if user switches it on manually */
				{
					autolight = 0;
				}
				break;
		}
		if (autolight)
		{
			light = ctrl_autolight(); /* overwrite light setting of throttle */
		}
		msg->flags = light ? (msg->flags | LIGHT_ENABLED_FLAG) : (msg->flags & ~LIGHT_ENABLED_FLAG);
		msg_send(msg); /* forward (modified) message to control unit */

		led_set(LED_OUT, light);
		led_set(LED_LIGHT, light); /* set front light */
		led_off(LED_ACT);
	}
	if (sleep_ready())
	{
		ctrl_sleep();
		autolight = 1; /* enable autolight when scooter is switch on */
	}
}

void ctrl_sleep(void)
{
	adc_stop();
	led_off(LED_ACT);
	led_off(LED_PWR);
	led_off(LED_OUT);
	led_off(LED_LIGHT);
	sleep_powerdown();
	adc_start();
}

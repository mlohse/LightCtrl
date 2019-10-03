/*******************************************************************************
 * LED driver                                                                  *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include "led.h"
#include "timer.h"
#include "config.h"
#include <avr/io.h>

#define LED_FLASH_INTERVAL 175 /* ms */

/* Helper Macros */
#define bit_set(PORT, PIN)		((PORT) |= (1 << (PIN)))
#define bit_clear(PORT, PIN)	((PORT) &= (uint8_t)~(1 << (PIN)))
#define bit_toggle(PORT, PIN)	((PORT) ^= (1 << (PIN)))

void led_flash(void)
{
	for (uint8_t i = 1; i < LED_NUM; ++i)
	{
		led_on(i);
		timer_msleep(LED_FLASH_INTERVAL);
	}
	
	for (uint8_t i = 1; i < LED_NUM; ++i)
	{
		led_off(i);
		timer_msleep(LED_FLASH_INTERVAL);
	}
}

void led_set(uint8_t num, uint8_t enabled)
{
	enabled ? led_on(num) : led_off(num);
}

void led_on(uint8_t num)
{
	switch (num)
	{
#if defined PORT_LED0 && defined PIN_LED0
		case 0: bit_set(PORT_LED0, PIN_LED0); break;
#endif
#if defined PORT_LED1 && defined PIN_LED1
		case 1: bit_set(PORT_LED1, PIN_LED1); break;
#endif
#if defined PORT_LED2 && defined PIN_LED2
		case 2: bit_set(PORT_LED2, PIN_LED2); break;
#endif
#if defined PORT_LED3 && defined PIN_LED3
		case 3: bit_set(PORT_LED3, PIN_LED3); break;
#endif
#if defined PORT_LED4 && defined PIN_LED4
		case 4: bit_set(PORT_LED4, PIN_LED4); break;
#endif
#if defined PORT_LED5 && defined PIN_LED5
		case 5: bit_set(PORT_LED5, PIN_LED5); break;
#endif
#if defined PORT_LED6 && defined PIN_LED6
		case 6: bit_set(PORT_LED6, PIN_LED6); break;
#endif
#if defined PORT_LED7 && defined PIN_LED7
		case 7: bit_set(PORT_LED7, PIN_LED7); break;
#endif
#if defined PORT_LED8 && defined PIN_LED8
		case 8: bit_set(PORT_LED8, PIN_LED8); break;
#endif
#if defined PORT_LED9 && defined PIN_LED9
		case 9: bit_set(PORT_LED9, PIN_LED9); break;
#endif
		default:
			break;
	}
}

void led_off(uint8_t num)
{
	switch (num)
	{
#if defined PORT_LED0 && defined PIN_LED0
		case 0: bit_clear(PORT_LED0, PIN_LED0); break;
#endif
#if defined PORT_LED1 && defined PIN_LED1
		case 1: bit_clear(PORT_LED1, PIN_LED1); break;
#endif
#if defined PORT_LED2 && defined PIN_LED2
		case 2: bit_clear(PORT_LED2, PIN_LED2); break;
#endif
#if defined PORT_LED3 && defined PIN_LED3
		case 3: bit_clear(PORT_LED3, PIN_LED3); break;
#endif
#if defined PORT_LED4 && defined PIN_LED4
		case 4: bit_clear(PORT_LED4, PIN_LED4); break;
#endif
#if defined PORT_LED5 && defined PIN_LED5
		case 5: bit_clear(PORT_LED5, PIN_LED5); break;
#endif
#if defined PORT_LED6 && defined PIN_LED6
		case 6: bit_clear(PORT_LED6, PIN_LED6); break;
#endif
#if defined PORT_LED7 && defined PIN_LED7
		case 7: bit_clear(PORT_LED7, PIN_LED7); break;
#endif
#if defined PORT_LED8 && defined PIN_LED8
		case 8: bit_clear(PORT_LED8, PIN_LED8); break;
#endif
#if defined PORT_LED9 && defined PIN_LED9
		case 9: bit_clear(PORT_LED9, PIN_LED9); break;
#endif
		default:
			break;
	}
}

void led_toggle(uint8_t num)
{
	switch (num)
	{
#if defined PORT_LED0 && defined PIN_LED0
		case 0: bit_toggle(PORT_LED0, PIN_LED0); break;
#endif
#if defined PORT_LED1 && defined PIN_LED1
		case 1: bit_toggle(PORT_LED1, PIN_LED1); break;
#endif
#if defined PORT_LED2 && defined PIN_LED2
		case 2: bit_toggle(PORT_LED2, PIN_LED2); break;
#endif
#if defined PORT_LED3 && defined PIN_LED3
		case 3: bit_toggle(PORT_LED3, PIN_LED3); break;
#endif
#if defined PORT_LED4 && defined PIN_LED4
		case 4: bit_toggle(PORT_LED4, PIN_LED4); break;
#endif
#if defined PORT_LED5 && defined PIN_LED5
		case 5: bit_toggle(PORT_LED5, PIN_LED5); break;
#endif
#if defined PORT_LED6 && defined PIN_LED6
		case 6: bit_toggle(PORT_LED6, PIN_LED6); break;
#endif
#if defined PORT_LED7 && defined PIN_LED7
		case 7: bit_toggle(PORT_LED7, PIN_LED7); break;
#endif
#if defined PORT_LED8 && defined PIN_LED8
		case 8: bit_toggle(PORT_LED8, PIN_LED8); break;
#endif
#if defined PORT_LED9 && defined PIN_LED9
		case 9: bit_toggle(PORT_LED9, PIN_LED9); break;
#endif
		default:
			break;
	}
}

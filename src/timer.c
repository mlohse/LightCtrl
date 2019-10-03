/*******************************************************************************
 * Timer providing simple sleep and uptime functions                           *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include "timer.h"
#include "config.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


static volatile uint32_t uptime = 0; /* uptime in ms */

/*******************************************************************************
 * Timer0
 * 3.8684 MHz: pre: 256->14400 inc/sec -> OCR0A: 72->200irq/sec (5ms resolution)
 ******************************************************************************/
ISR(TIMER0_COMPA_vect)
{
	uptime += 5;
}

void timer_init(void)
{
	uptime = 0;

	TCCR0A = (1 << WGM01); /* CTC-Mode */
	TCCR0B = (1 << CS02); /* clk/256 prescaler (*/

	TIMSK0 &= (uint8_t)~(1 << TOIE0); /* TOIE0 = 0 (Timer0 overflow IRQ) */
	TIMSK0 |= (1 << OCIE0A);  /* OCIE0 = 1 (Timer0 Compare IRQ) */

	OCR0A = 0x48; /* timer compare irq every 5ms@3.6864MHz */
}

void timer_msleep(uint32_t ms)
{
	ms += timer_uptime();
	while (timer_uptime() < ms);
}

uint32_t timer_uptime(void)
{
	uint32_t tmp;
	cli();
	tmp = uptime;
	sei();
	return tmp;
}

void timer_clear(void)
{
	cli();
	uptime = 0;
	sei();
}

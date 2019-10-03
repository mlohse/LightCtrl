/*******************************************************************************
 * Sleep/Power Management                                                      *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "sleep.h"
#include "config.h"
#include "timer.h"

#if   SLEEP_WAKEUP_PCINT ==  0
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT0
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      0
#elif SLEEP_WAKEUP_PCINT ==  1
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT1
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      1
#elif SLEEP_WAKEUP_PCINT ==  2
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT2
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      2
#elif SLEEP_WAKEUP_PCINT ==  3
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT3
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      3
#elif SLEEP_WAKEUP_PCINT ==  4
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT4
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      4
#elif SLEEP_WAKEUP_PCINT ==  5
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT5
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      5
#elif SLEEP_WAKEUP_PCINT ==  6
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT6
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      6
#elif SLEEP_WAKEUP_PCINT ==  7
#define WAKEUP_MASK_REG PCMSK0
#define WAKEUP_VECT_IRQ PCINT0_vect
#define WAKEUP_MASK_PIN PCINT7
#define WAKEUP_PORT_IN  PINA
#define WAKEUP_PIN      7
#elif SLEEP_WAKEUP_PCINT ==  8
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT8
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      0
#elif SLEEP_WAKEUP_PCINT ==  9
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT9
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      1
#elif SLEEP_WAKEUP_PCINT == 10
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT10
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      2
#elif SLEEP_WAKEUP_PCINT == 11
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT11
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      3
#elif SLEEP_WAKEUP_PCINT == 12
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT12
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      4
#elif SLEEP_WAKEUP_PCINT == 13
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT13
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      5
#elif SLEEP_WAKEUP_PCINT == 14
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT14
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      6
#elif SLEEP_WAKEUP_PCINT == 15
#define WAKEUP_MASK_REG PCMSK1
#define WAKEUP_VECT_IRQ PCINT1_vect
#define WAKEUP_MASK_PIN PCINT15
#define WAKEUP_PORT_IN  PINB
#define WAKEUP_PIN      7
#elif SLEEP_WAKEUP_PCINT == 16
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT16
#define WAKEUP_PORT_IN  PINC
#define WAKEUP_PIN      0
#elif SLEEP_WAKEUP_PCINT == 17
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT17
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      1
#elif SLEEP_WAKEUP_PCINT == 18
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT18
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      2
#elif SLEEP_WAKEUP_PCINT == 19
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT19
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      3
#elif SLEEP_WAKEUP_PCINT == 20
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT20
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      4
#elif SLEEP_WAKEUP_PCINT == 21
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT21
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      5
#elif SLEEP_WAKEUP_PCINT == 22
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT22
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      6
#elif SLEEP_WAKEUP_PCINT == 23
#define WAKEUP_MASK_REG PCMSK2
#define WAKEUP_VECT_IRQ PCINT2_vect
#define WAKEUP_MASK_PIN PCINT23
#define WAKEUP_PORT_IN  PORTC
#define WAKEUP_PIN      7
#elif SLEEP_WAKEUP_PCINT == 24
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT24
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      0
#elif SLEEP_WAKEUP_PCINT == 25
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT25
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      1
#elif SLEEP_WAKEUP_PCINT == 26
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT26
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      2
#elif SLEEP_WAKEUP_PCINT == 27
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT27
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      3
#elif SLEEP_WAKEUP_PCINT == 28
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT28
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      4
#elif SLEEP_WAKEUP_PCINT == 29
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT29
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      5
#elif SLEEP_WAKEUP_PCINT == 30
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT30
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      6
#elif SLEEP_WAKEUP_PCINT == 31
#define WAKEUP_MASK_REG PCMSK3
#define WAKEUP_VECT_IRQ PCINT3_vect
#define WAKEUP_MASK_PIN PCINT31
#define WAKEUP_PORT_IN  PIND
#define WAKEUP_PIN      7
#endif

static volatile uint8_t do_sleep = 0;
static uint32_t lastawake = 0;

ISR(WAKEUP_VECT_IRQ)
{
	do_sleep = 0;
}

uint8_t sleep_ready(void)
{
	if (WAKEUP_PORT_IN & (1 << WAKEUP_PIN))
	{
		lastawake = timer_uptime();
	}
	return ((timer_uptime() - lastawake) > SLEEP_ACT_TIMEOUT);
}

void sleep_powerdown(void)
{
	PCICR = (1 << PCIE0) | (1 << PCIE1) | (1 << PCIE2) | (1 << PCIE3); /* enable pin change irqs [0..3] */
	WAKEUP_MASK_REG = (1 << WAKEUP_MASK_PIN); /* enable pin change interupt for wakeup pin */
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	cli();
	do_sleep = 1;
	while (do_sleep)
	{
		sleep_enable();
#ifdef SLEEP_BOD_DISABLE
		sleep_bod_disable();
#endif
		sei();
		sleep_cpu();
		sleep_disable();
	}
	lastawake = timer_uptime();
}

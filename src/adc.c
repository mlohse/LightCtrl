/*******************************************************************************
 * ADC driver for light sensor                                                 *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include "adc.h"
#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint16_t adcval = 0;

ISR(ADC_vect)
{
	adcval = ADCL;
	adcval |= (uint16_t)(ADCH << 8);
}

void adc_init(void)
{
	ADMUX = (1 << REFS0) | (LIGHT_SENS_PIN & 0x1f); /* 0x1f: MUX[0..4] */
	DIDR0 = 1 << LIGHT_SENS_PIN; /* disable digital input buffer */
}

void adc_start(void)
{
	/* enable ADC, irq, auto-conv. trigger, clk prescaler: 128 */
	ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) |
	         (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRB = 0; /* select free running mode */
	ADCSRA |= (1 << ADSC); /* trigger first adc conv. */
}

void adc_stop(void)
{
	ADCSRA &= ~(1 << ADEN);
	adcval = 0;
}

uint16_t adc_read(void)
{
	uint16_t tmp;
	cli();
	tmp = adcval;
	sei();
	return tmp;
}

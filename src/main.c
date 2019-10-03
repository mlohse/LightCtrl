/*******************************************************************************
 * main.c                                                                      *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include <avr/io.h>
#include "adc.h"
#include "led.h"
#include "uart.h"
#include "config.h"
#include "ctrl.h"
#include "timer.h"


void init(void)
{
	/* configure & initialize io ports */
	DDRA = IO_DDRA;
	DDRB = IO_DDRB;
	DDRC = IO_DDRC;
	DDRD = IO_DDRD;
	
	PORTA = INIT_PORTA;
	PORTB = INIT_PORTB;
	PORTC = INIT_PORTC;
	PORTD = INIT_PORTD;

	timer_init();
	uart_init();
	led_flash();
	uart0_flush();
	adc_init();
}

int main(void)
{
	init();
	adc_start();

	while(1)
	{
		ctrl_run();
		led_set(LED_PWR, (timer_uptime() % 2000) < 1000);
	}
}

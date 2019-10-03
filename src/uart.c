/*******************************************************************************
 * UART driver                                                                 *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include <avr/interrupt.h>
#include "led.h"
#include "uart.h"
#include "buffer.h"

#ifdef UART0_ENABLE
static buffer_t buffer_tx0;
static buffer_t buffer_rx0;
static uint8_t buffer_data_tx0[BUFFER_SIZE_TX0];
static uint8_t buffer_data_rx0[BUFFER_SIZE_RX0];
#endif

#ifdef UART1_ENABLE
static buffer_t buffer_tx1;
static buffer_t buffer_rx1;
static uint8_t buffer_data_tx1[BUFFER_SIZE_TX1];
static uint8_t buffer_data_rx1[BUFFER_SIZE_RX1];
#endif


void uart_init(void)
{
	cli();
#ifdef UART0_ENABLE
	buffer_init(&buffer_tx0, buffer_data_tx0, BUFFER_SIZE_TX0);
	buffer_init(&buffer_rx0, buffer_data_rx0, BUFFER_SIZE_RX0);

	UBRR0H = UART0_UBRR >> 8;
	UBRR0L = UART0_UBRR & 0xff;
	/* Enable receiver and transmitter and receive/transmit complete interrupt */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << TXCIE0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
#endif
#ifdef UART1_ENABLE
	buffer_init(&buffer_tx1, buffer_data_tx1, BUFFER_SIZE_TX1);
	buffer_init(&buffer_rx1, buffer_data_rx1, BUFFER_SIZE_RX1);

	UBRR1H = UART1_UBRR >> 8;
	UBRR1L = UART1_UBRR & 0xff;
	/* Enable receiver and transmitter and receive/transmit complete interrupt */
	UCSR1B = (1 << TXEN1) | (1 << RXEN1) | (1 << TXCIE1) | (1 << RXCIE1);
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
#endif
	sei();
}

#ifdef UART0_ENABLE
ISR(USART0_TX_vect)
{
	if (buffer_count(&buffer_tx0))
	{
		UDR0 = buffer_read(&buffer_tx0);
	}
}

ISR(USART0_RX_vect)
{
	if (buffer_is_full(&buffer_rx0))
	{
		/* todo: handle this somehow... (send error message?) */
		return;
	}
	
	buffer_write(&buffer_rx0, UDR0);
}

void uart0_int_enable(void)
{
	/* enable receive/transmit complete interrupt */
	UCSR0B |= (1 << TXCIE0) | (1<<RXCIE0);
}

void uart0_int_disable(void)
{
	/* disable receive/transmit complete interrupt */
	UCSR0B &= (uint8_t)~((1 << TXCIE0) | (1<<RXCIE0));
}

void uart0_flush(void)
{
	uart0_int_disable();
	buffer_clear(&buffer_rx0);
	buffer_clear(&buffer_tx0);
	uart0_int_enable();
}

void uart0_write_buffer(const uint8_t *data, uint8_t size)
{
	uint8_t i = 0;
	if (buffer_count(&buffer_tx0) == 0 && (UCSR0A & (1 << UDRE0)))
	{
		if (size > 1)
		{
			/* disable tx interrupt to have at least 1 byte in tx buffer before
			 * interrupt handler is called */
			UCSR0B &= (uint8_t)~((1 << TXCIE0));
		}
		/* write buffer empty and no uart write in progress -> write out first
		 * byte directly */
		UDR0 = data[i++];
	}

	while (i < size)
	{
		while (buffer_is_full(&buffer_tx0))
		{
			/* write buffer full, enable tx irq to send out data and wait... */
			UCSR0B |= (1 << TXCIE0);
		}
		/* disable transmit interrupt to protect tx buffer */
		UCSR0B &= (uint8_t)~((1 << TXCIE0));
		buffer_write(&buffer_tx0, data[i]);
		++i;
	}
	UCSR0B |= (1 << TXCIE0); /* enable transmit interrupt */
}

void uart0_write(uint8_t byte)
{
	if (buffer_count(&buffer_tx0) == 0 && (UCSR0A & (1 << UDRE0)))
	{
		/* write buffer empty and no uart write in progress -> write out directly */
		UDR0 = byte;
		return;
	}

	while (buffer_is_full(&buffer_tx0)); /* write buffer full - wait! */

	uart0_int_disable();
	buffer_write(&buffer_tx0, byte);
	uart0_int_enable();
}

uint8_t uart0_read(void)
{
	uint8_t byte;
	uart0_int_disable();
	byte = buffer_read(&buffer_rx0);
	uart0_int_enable();
	return byte;
}

uint8_t uart0_bytes_available(void)
{
	uint8_t count;
	uart0_int_disable();
	count = buffer_count(&buffer_rx0);
	uart0_int_enable();
	return count;
}

uint8_t uart0_peek(uint8_t position)
{
	uint8_t byte;
	uart0_int_disable();
	byte = buffer_peek(&buffer_rx0, position);
	uart0_int_enable();
	return byte;
}
#endif /* UART0_ENABLE */


#ifdef UART1_ENABLE
ISR(USART1_TX_vect)
{
	if (buffer_count(&buffer_tx1))
	{
		UDR1 = buffer_read(&buffer_tx1);
	}
}

ISR(USART1_RX_vect)
{
	if (buffer_is_full(&buffer_rx1))
	{
		/* todo: handle this somehow... (send error message?) */
		return;
	}
	
	buffer_write(&buffer_rx1, UDR1);
}

void uart1_int_enable(void)
{
	/* enable receive/transmit complete interrupt */
	UCSR1B |= (1 << TXCIE1) | (1<<RXCIE1);
}

void uart1_int_disable(void)
{
	/* disable receive/transmit complete interrupt */
	UCSR1B &= (uint8_t)~((1 << TXCIE1) | (1<<RXCIE1));
}

void uart1_flush(void)
{
	uart1_int_disable();
	buffer_clear(&buffer_rx1);
	buffer_clear(&buffer_tx1);
	uart1_int_enable();
}

void uart1_write(uint8_t byte)
{
	if ((UCSR1A & (1 << UDRE1)) && buffer_count(&buffer_tx1) == 0)
	{
		/* write buffer empty and no uart write in progress -> write out directly */
		UDR1 = byte;
		return;
	}

	while (buffer_is_full(&buffer_tx1)); /* write buffer full - wait! */

	uart1_int_disable();
	buffer_write(&buffer_tx1, byte);
	uart1_int_enable();
}

uint8_t uart1_read(void)
{
	uint8_t byte;
	uart1_int_disable();
	byte = buffer_read(&buffer_rx1);
	uart1_int_enable();
	return byte;
}

uint8_t uart1_bytes_available(void)
{
	uint8_t count;
	uart1_int_disable();
	count = buffer_count(&buffer_rx1);
	uart1_int_enable();
	return count;
}

uint8_t uart1_peek(uint8_t position)
{
	uint8_t byte;
	uart1_int_disable();
	byte = buffer_peek(&buffer_rx1, position);
	uart1_int_enable();
	return byte;
}
#endif // UART1_ENABLE

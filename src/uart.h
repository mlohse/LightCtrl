/*******************************************************************************
 * UART driver                                                                 *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "config.h"

#if defined UART0_ENABLE || defined UART1_ENABLE
/**
 * @brief uart_init initializes UART0 and/or UART1 if enabled
 */
void uart_init(void);
#endif

#ifdef UART0_ENABLE
/**
 * @brief uart0_flush empties both read and write buffers of UART0
 */
void uart0_flush(void);

/**
 * @brief uart0_read reads/takes 1 byte from UART0 read buffer
 * @return first byte in buffer or '0' if buffer is empty
 * @see uart0_bytes_available()
 */
uint8_t uart0_read(void);

/**
 * @brief uart0_bytes_available tests if data can be read from UART0
 * @return number of bytes available in read buffer
 */
uint8_t uart0_bytes_available(void);

/**
 * @brief uart0_peek reads 1 byte from UART0 read buffer, but leaves it in buffer
 * @param position to read
 * @return byte read ot '0' if position does not exist in buffer
 */
uint8_t uart0_peek(uint8_t position);

/**
 * @brief uart0_write write 1 byte out on UART0
 * @param byte to be written
 * @note if UART0 is busy data is written into the tx buffer, if the buffer is
 *       full, then this function blocks until the byte can be written
 */
void uart0_write(uint8_t byte);

/**
 * @brief uart0_write_buffer write on UART0
 * @param data to be written (copied, can be deleted as soon as function returns)
 * @param size of data
 * @note  if 'data' does not fit into tx buffer then this function will block
 *        until enough space becomes available
 */
void uart0_write_buffer(const uint8_t *data, uint8_t size);
#endif

#ifdef UART1_ENABLE
void uart1_flush(void);
uint8_t uart1_read(void);
uint8_t uart1_bytes_available(void);
uint8_t uart1_peek(uint8_t position);
void uart1_write(uint8_t byte);
#endif

/* Check whether buffer size is within range... */
#ifdef UART0_ENABLE
	#if (BUFFER_SIZE_TX0 > 254 || BUFFER_SIZE_RX0 > 254)
		#error Maximum uart buffer size is 254 bytes!
	#endif

	#if (BUFFER_SIZE_TX0 < 1 || BUFFER_SIZE_RX0 < 1)
		#error Minimum uart buffer size is 1 byte!
	#endif
#endif

#ifdef UART1_ENABLE
	#if (BUFFER_SIZE_TX1 > 254 || BUFFER_SIZE_RX1 > 254)
		#error Maximum uart buffer size is 254 bytes!
	#endif

	#if (BUFFER_SIZE_TX1 < 1 || BUFFER_SIZE_RX1 < 1)
		#error Minimum uart buffer size is 1 byte!
	#endif
#endif

#endif /* UART_H_ */

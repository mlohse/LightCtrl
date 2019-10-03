/*******************************************************************************
 * Ring buffer                                                                 *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>

typedef struct
{
	uint8_t *data;
	uint8_t capacity;
	uint8_t ri;
	uint8_t wi;
} buffer_t;

/**
 * @brief buffer_init initializes the buffer
 * @param buffer pointer to object to be initialized
 * @param data pointer to raw buffer, must stay valid as long as buffer is used!
 * @param size of buffer
 */
void buffer_init(buffer_t *buffer, uint8_t *data, uint8_t size);

/**
 * @brief buffer_clear removes all data from buffer
 * @param buffer pointer to object
 */
void buffer_clear(buffer_t *buffer);

/**
 * @brief buffer_write writes 1 byte to the buffer
 * @param buffer pointer to object
 * @param ch byte to be written
 * @note  if buffer is full 'ch' will be discarded, @see buffer_is_full(...)
 */
void buffer_write(buffer_t *buffer, uint8_t ch);

/**
 * @brief buffer_read reads/takes first byte from buffer
 * @param buffer pointer to object
 * @return byte read or '0' if buffer is empty
 */
uint8_t buffer_read(buffer_t *buffer);

/**
 * @brief buffer_peek reads 1 byte from buffer without removing it
 * @param buffer pointer to object
 * @param pos position to be read, will wrap araound if beyond capacity
 * @return returns byte read
 */
uint8_t buffer_peek(buffer_t *buffer, uint8_t pos);

/**
 * @brief buffer_count counts how many bytes are currently stored in buffer
 * @param buffer pointer to object
 * @return number of bytes in buffer
 */
uint8_t buffer_count(buffer_t *buffer);

/**
 * @brief buffer_is_full tests if buffer is full
 * @param buffer pointer to object
 * @return '1' if buffer is full, '0' if at least 1 more byte fits into buffer
 */
uint8_t buffer_is_full(buffer_t *buffer);

/*uint8_t buffer_is_empty(buffer_t *buffer);*/

#endif /* BUFFER_H_ */

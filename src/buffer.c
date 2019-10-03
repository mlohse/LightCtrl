/*******************************************************************************
 * Ring buffer                                                                 *
 * author:  Markus Lohse <mlohse@gmx.net>                                      *
 * date:    2019-09-24                                                         *
 * license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt              *
 ******************************************************************************/
#include "buffer.h"

void buffer_init(buffer_t *buffer, uint8_t *data, uint8_t size)
{
	buffer->data = data;
	buffer->capacity = size;
	buffer->ri = 0;
	buffer->wi = 0;
}

void buffer_clear(buffer_t *buffer)
{
	buffer->ri = 0;
	buffer->wi = 0;
}

void buffer_write(buffer_t *buffer, uint8_t ch)
{	
	if ((buffer->wi + 1) == buffer->ri ||
	   ((buffer->wi + 1) == buffer->capacity && buffer->ri == 0))
	{
		return; /* buffer full */
	}
	
	buffer->data[buffer->wi++] = ch;

	if (buffer->capacity == buffer->wi)
	{
		buffer->wi = 0;
	}
}

uint8_t buffer_read(buffer_t *buffer)
{
	if (buffer->ri == buffer->wi)
	{
		return 0; /* buffer empty */
	}

	if ((buffer->ri + 1) == buffer->capacity)
	{
		buffer->ri = 0;
		return buffer->data[buffer->capacity - 1];
	}

	return buffer->data[buffer->ri++];
}

uint8_t buffer_peek(buffer_t *buffer, uint8_t pos)
{
	uint16_t index = pos + buffer->ri;
	while (index >= buffer->capacity)
	{
		index -= buffer->capacity;
	}

	return buffer->data[index];
}

uint8_t buffer_count(buffer_t *buffer)
{
	return ((buffer->wi < buffer->ri) ?
	         buffer->capacity - buffer->ri + buffer->wi : buffer->wi - buffer->ri);
}

uint8_t buffer_is_full(buffer_t *buffer)
{
	if (buffer->wi < buffer->ri)
	{
		return ((buffer->wi + 1) == buffer->ri);
	}
	
	return (buffer->ri == 0 && (buffer->wi + 1) == buffer->capacity);
}

/*
uint8_t buffer_is_empty(buffer_t *buffer)
{
	return (buffer->ri == buffer->wi);
}
*/

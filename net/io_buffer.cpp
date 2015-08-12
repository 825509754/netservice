#include "io_buffer.h"
#include "Common.h"
#include "malloc.h"

iobuffer::iobuffer()
{
	cap = 1;
	buffer_begin = 0;
	buffer_end = 0;
	buffe_length = 0;
	total_size = 0;
	create_buffer();
}

iobuffer::~iobuffer()
{
	release_buffer();
}

void iobuffer::create_buffer()
{
	total_size = cap * MAX_SEND_BUFFER;

	msg_buffer = (char *)MALLOC(total_size);
	memset(msg_buffer, 0, total_size);
}

void iobuffer::release_buffer()
{
	FREE(msg_buffer);
}


unsigned short iobuffer::write_buffer(const char *buffer, unsigned short size)
{
	mLock.lock();

	if ((total_size - buffe_length) < size )
	{
		expand_buffer();
	}

	if ((buffer_end + size) <= total_size)
	{
		memcpy(&msg_buffer[buffer_end], buffer, size);
		buffer_end += size;
	}
	else
	{
		unsigned short copy_size = total_size - buffer_end;
		memcpy(&msg_buffer[buffer_end], buffer, copy_size);
		memcpy(msg_buffer, buffer+copy_size, size-copy_size);
		buffer_end = size-copy_size;
	}

	buffe_length += size;

	mLock.unlock();
	return 0;
}

unsigned int iobuffer::read_buffer(char *buffer, unsigned short size)
{
	int send_size = 0;

	mLock.lock();
	if (size > buffe_length)
	{
		if (buffer_end >= buffer_begin)
		{
			memcpy(buffer, &msg_buffer[buffer_begin], buffe_length);
		}
		else
		{
			int tailsize = total_size-buffer_begin;
			memcpy(buffer, &msg_buffer[buffer_begin], tailsize);
			memcpy(buffer+tailsize, msg_buffer, buffer_end);
		}

		send_size = buffe_length;
		buffe_length = 0;
		buffer_begin = 0;
		buffer_end = 0;
	}
	else
	{
		if (buffer_end >= buffer_begin)
		{
			memcpy(buffer, &msg_buffer[buffer_begin], size);
		}
		else
		{
			int tailsize = total_size-buffer_begin;
			memcpy(buffer, &msg_buffer[buffer_begin], tailsize);
			memcpy(buffer+tailsize, msg_buffer, size-tailsize);
		}

		send_size = size;
	}
	mLock.unlock();

	return send_size;
}

void iobuffer::expand_buffer()
{
	cap += 1;
	total_size = cap * MAX_SEND_BUFFER;

	char *new_buffer = (char *)MALLOC(total_size);
	memset(new_buffer, 0, total_size);

	if (buffer_end >= buffer_begin)
	{
		memcpy(new_buffer, msg_buffer+buffer_begin, buffe_length);
	}
	else
	{
		int tailsize = total_size-buffer_begin;
		memcpy(new_buffer, &msg_buffer[buffer_begin], tailsize);
		memcpy(new_buffer+tailsize, msg_buffer, buffer_end);
	}
	FREE(msg_buffer);
	msg_buffer = new_buffer;

	buffer_begin = 0;
	buffer_end = buffe_length;
}
#ifndef IO_BUFFER_H
#define IO_BUFFER_H

#include <string.h>
#include "Lock.h"

class iobuffer
{
public:
	iobuffer();
	~iobuffer();

public:
	unsigned short write_buffer(const char *buffer, unsigned short size);
	unsigned int read_buffer(char *buffer, unsigned short size);
	
private:
	void create_buffer();
	void release_buffer();
	void expand_buffer();


private:
	char *msg_buffer;

	unsigned short buffer_begin;
	unsigned short buffer_end;

	unsigned short buffe_length;

	unsigned short cap;
	unsigned short total_size;

	Lock mLock;
};
#endif
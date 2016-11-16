#ifndef __CIO_H
#define __CIO_H

#include "types.h"

#define NAL_BUFFER_MAX 8192

typedef struct _nal_buffer {
	int pos;
	int bitpos;
	int posmax;
	uint8 data[NAL_BUFFER_MAX];
} nal_buffer_t;

void dump_nal_buffer(nal_buffer_t * pnal_buffer);
void copy_to_nal_buf(nal_buffer_t * pnal_buffer, uint8 c);

#endif
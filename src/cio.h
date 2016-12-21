#ifndef __CIO_H
#define __CIO_H
#include <stdio.h>
#include "types.h"

#define NAL_BUFFER_MAX (10*1024*1024) 
//8192

typedef struct _nal_buffer {
	int pos;
	int bitpos;
	int posmax;
	uint8* data;
} nal_buffer_t;

void dump_nal_buffer(nal_buffer_t * pnal_buffer);
void copy_to_nal_buf(nal_buffer_t * pnal_buffer, uint8 c);
void copy_nal_to_file(nal_buffer_t * pnal_buffer, FILE * f);
void write_nal_data_to_file(nal_buffer_t* pnal_buffer, FILE * f);

#endif
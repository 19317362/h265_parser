#include <stdio.h>

#include "cio.h"

void dump_nal_buffer(nal_buffer_t * pnal_buffer) {
	for (int i = 0; i < pnal_buffer->posmax; i++) {
		if (i % 16 == 0) {
			fprintf(stdout, "\t%04x:  ", i);
		}
		fprintf(stdout, "%02x ", pnal_buffer->data[i]);
		if (i % 16 == 15) {
			fprintf(stdout, "\n");
		}
	}
	fprintf(stdout, "\n");
}

void copy_to_nal_buf(nal_buffer_t * pnal_buffer, uint8 c) {
	pnal_buffer->data[pnal_buffer->pos] = c;
	// fprintf(stdout, "\t[%d] = %02x\n", nal_buffer.pos, buffer[i]);
	pnal_buffer->pos++;
}


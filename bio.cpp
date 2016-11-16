#include <stdio.h>

#include "types.h"
#include "cio.h"

void align_to_byte(nal_buffer_t * pnal_buffer) {
	if (pnal_buffer->bitpos != 8) {
		pnal_buffer->pos++;
		pnal_buffer->bitpos = 8;
	}
}

uint8 read_bit(nal_buffer_t * pnal_buffer) {
	if (pnal_buffer->bitpos == 0) {
		pnal_buffer->pos++;
		pnal_buffer->bitpos = 8;
	}

	if (pnal_buffer->pos >= pnal_buffer->posmax) {
		fprintf(stdout, "!!! nal buffer overrun !!!\n");
	}

	uint8 ret = pnal_buffer->data[pnal_buffer->pos] & (1 << (pnal_buffer->bitpos - 1));
	pnal_buffer->bitpos--;
	//fprintf(stdout, "\t\t%02x[%d] = %d\n", nal_buffer.data[nal_buffer.pos], nal_buffer.bitpos, ret);
	fprintf(stdout, "%d", ret > 0 ? 1 : 0);
	return ret > 0 ? 1 : 0;
}

uint32 read_bits(nal_buffer_t * pnal_buffer, int nbits) {
	uint32 ret = 0;
	for (int i = 0; i < nbits; i++) {
		ret = (ret << 1) | read_bit(pnal_buffer);
	}
	return ret;
}

uint32 read_uev(nal_buffer_t * pnal_buffer) { // read exp-golomb code
	fprintf(stdout, "[");
	int zero_leading_bits = -1;
	uint8 b = 0;
	for (b = 0; !b; zero_leading_bits++) {
		b = read_bit(pnal_buffer);
	}
	uint32 ret = (1 << zero_leading_bits) - 1 + read_bits(pnal_buffer, zero_leading_bits);
	fprintf(stdout, "]");
	return ret;
}

sint32 read_sev(nal_buffer_t * pnal_buffer) { // read signed exp-golomb code
	uint32 val = read_uev(pnal_buffer);
	sint32 ret = 0;
	if (val > 0) {
		ret = ((val % 2) > 0 ? 1 : -1) * CEIL(val, 2);
	}
	return ret;
}


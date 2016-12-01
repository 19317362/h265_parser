#include "cio.h"
#include <cstring>

#define BUFFER_MAX 1024

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

//write nal_buffer_t to file as is
void copy_nal_to_file(nal_buffer_t* pnal_buffer, FILE* f)
{
	fwrite(pnal_buffer->data, pnal_buffer->posmax, 1, f);
}

//write nal_buffer data with encapsulation of an SODB 

void write_nal_data_to_file(nal_buffer_t* pnal_buffer, FILE* f)
{
	printf("Write to file: \n");
	const uint8 SODB_CODE[1] = { 0x03 };
	const uint8 START_CODE[3] = { 0x00, 0x00, 0x01 };
	const uint8 ZERO_BITS[1] = { 0x00 };

	fwrite(START_CODE, sizeof(START_CODE), 1, f);
	printf("%02X %02X %02X ", START_CODE[0],START_CODE[1], START_CODE[2]);
	for (int i = 0; i <= pnal_buffer->posmax; i++)
	{
		if (i + 2 < pnal_buffer->posmax &&
			(
				(pnal_buffer->data[i] == 0 && pnal_buffer->data[i + 1] == 0 && pnal_buffer->data[i + 2] == 0) ||
				(pnal_buffer->data[i] == 0 && pnal_buffer->data[i + 1] == 0 && pnal_buffer->data[i + 2] == 1) ||
				(pnal_buffer->data[i] == 0 && pnal_buffer->data[i + 1] == 0 && pnal_buffer->data[i + 2] == 2) ||
				(pnal_buffer->data[i] == 0 && pnal_buffer->data[i + 1] == 0 && pnal_buffer->data[i + 2] == 3)
			)
		)
		{
			fwrite(&pnal_buffer->data[i], sizeof(uint8), 1, f);
			printf("%02X ", pnal_buffer->data[i]);
			fwrite(&pnal_buffer->data[i + 1], sizeof(uint8), 1, f);
			printf("%02X ", pnal_buffer->data[i + 1]);
			fwrite(SODB_CODE, sizeof(SODB_CODE), 1, f);
			printf("%02X ", SODB_CODE[0]);
			i += 1;
		}
		else
		{
			fwrite(&pnal_buffer->data[i], sizeof(uint8), 1, f);
			printf("%02X ", pnal_buffer->data[i]);
		}
	}

	if (pnal_buffer->data[pnal_buffer->posmax] == 0)
	{
		fwrite(SODB_CODE, sizeof(SODB_CODE), 1, f);
		printf("%02X ", SODB_CODE[0]);
	}
    fwrite(ZERO_BITS, sizeof(ZERO_BITS), 1, f);
	printf("%02X ", ZERO_BITS[0]);

}


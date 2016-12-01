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
//TODO не корректно пишетс€ начало (не хватае 00 перед 40) не корректно вставл€ютс€ 03 и в конце в оригинале стоит 08 а не 03

void write_nal_data_to_file(nal_buffer_t* pnal_buffer, FILE* f)
{
	const uint8 SODB_CODE[1] = { 0x03 };
	const uint8 START_CODE[3] = { 0x00, 0x00, 0x01 };

	fwrite(START_CODE, sizeof(START_CODE), 1, f);

	for (int i = 0; i < pnal_buffer->posmax; i++)
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
			fwrite(&pnal_buffer->data[i + 1], sizeof(uint8), 1, f);
			fwrite(SODB_CODE, sizeof(SODB_CODE), 1, f);
			fwrite(&pnal_buffer->data[i + 2], sizeof(uint8), 1, f);
			i += 2;
		}
		else
		{
			fwrite(&pnal_buffer->data[i], sizeof(uint8), 1, f);
		}
	}
	if (pnal_buffer->data[pnal_buffer->posmax] == 0)
	{
		fwrite(SODB_CODE, sizeof(SODB_CODE), 1, f);
	}
}


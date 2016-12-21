#ifndef __NAL_IDR_N_LP_H
#define __NAL_IDR_N_LP_H
#include "cio.h"

void nal_idr_n_lp_init();
void nal_idr_n_lp_parse(nal_buffer_t * pnal_buffer);
void nal_idr_n_lp_pps(nal_buffer_t * pnal_buffer);

#endif
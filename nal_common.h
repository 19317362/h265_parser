#ifndef __NAL_COMMON_H
#define __NAL_COMMON_H

#define EXTENDED_SAR 255

void profile_tier_level(nal_buffer_t * pnal_buffer, int maxNumSubLayersMinus1);
void short_term_ref_pic_set(nal_buffer_t * pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets);
void sub_layer_hrd_parameters(nal_buffer_t * pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag);
void hrd_parameters(nal_buffer_t * pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1);
void vui_parameters(nal_buffer_t * pnal_buffer, uint8 sps_max_sub_layers_minus1);
void scaling_list_data(nal_buffer_t * pnal_buffer);

#endif
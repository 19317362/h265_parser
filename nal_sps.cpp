#include <stdio.h>

#include "types.h"
#include "bio.h"
#include "cio.h"
#include "nal_common.h"

void nal_sps_init() {

}

void nal_sps_parse(nal_buffer_t * pnal_buffer) {
	dump_nal_buffer(pnal_buffer);
	pnal_buffer->pos += 2;

	fprintf(stdout, "\tsps_video_parameter_set_id=%d\n", read_bits(pnal_buffer, 4));
	uint8 sps_max_sub_layers_minus1 = read_bits(pnal_buffer, 3);
	fprintf(stdout, "\tsps_max_sub_layers_minus1=%d\n", sps_max_sub_layers_minus1);
	fprintf(stdout, "\tsps_temporal_id_nesting_flag=%d\n", read_bit(pnal_buffer));
	profile_tier_level(pnal_buffer, sps_max_sub_layers_minus1);
	fprintf(stdout, "\tsps_seq_parameter_set_id=%d\n", read_uev(pnal_buffer));
	uint8 chroma_format_idc = read_uev(pnal_buffer);														read_uev(pnal_buffer); read_uev(pnal_buffer); read_uev(pnal_buffer); read_bit(pnal_buffer); // somewhere lost 3 uev + 1 bit, does not work w/o this
	fprintf(stdout, "\tchroma_format_idc=%d\n", chroma_format_idc);
	if (chroma_format_idc == 3) {
		fprintf(stdout, "\tseparate_colour_plane_flag=%d\n", chroma_format_idc);
	}
	fprintf(stdout, "\tpic_width_in_luma_samples=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tpic_height_in_luma_samples=%d\n", read_uev(pnal_buffer));
	uint8 conformance_window_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tconformance_window_flag=%d\n", conformance_window_flag);
	if (conformance_window_flag) {
		fprintf(stdout, "\tconf_win_left_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tconf_win_right_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tconf_win_top_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tconf_win_bottom_offset=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tbit_depth_luma_minus8=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tbit_depth_chroma_minus8=%d\n", read_uev(pnal_buffer));
	uint8 log2_max_pic_order_cnt_lsb_minus4 = read_uev(pnal_buffer);
	fprintf(stdout, "\tlog2_max_pic_order_cnt_lsb_minus4=%d\n", log2_max_pic_order_cnt_lsb_minus4);

	uint8 sps_sub_layer_ordering_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tsps_sub_layer_ordering_info_present_flag=%d\n", sps_sub_layer_ordering_info_present_flag);

	for (int i = (sps_sub_layer_ordering_info_present_flag ? 0 : sps_max_sub_layers_minus1);
		i <= sps_max_sub_layers_minus1; i++) {
		fprintf(stdout, "\tsps_max_dec_pic_buffering_minus1[i]=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tsps_max_num_reorder_pics[i]=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tsps_max_latency_increase_plus1[i]=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tlog2_min_luma_coding_block_size_minus3=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tlog2_diff_max_min_luma_coding_block_size=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tlog2_min_transform_block_size_minus2=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tlog2_diff_max_min_transform_block_size=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tmax_transform_hierarchy_depth_inter=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tmax_transform_hierarchy_depth_intra=%d\n", read_uev(pnal_buffer));
	uint8 scaling_list_enabled_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tscaling_list_enabled_flag=%d\n", scaling_list_enabled_flag);
	if (scaling_list_enabled_flag) {
		uint8 sps_scaling_list_data_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tsps_scaling_list_data_present_flag=%d\n", sps_scaling_list_data_present_flag);
		if (sps_scaling_list_data_present_flag) {
			scaling_list_data(pnal_buffer);
		}
	}
	fprintf(stdout, "\tamp_enabled_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tsample_adaptive_offset_enabled_flag=%d\n", read_bit(pnal_buffer));

	uint8 pcm_enabled_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tpcm_enabled_flag=%d\n", pcm_enabled_flag);
	if (pcm_enabled_flag) {
		fprintf(stdout, "\tpcm_sample_bit_depth_luma_minus1=%d\n", read_bits(pnal_buffer, 4));
		fprintf(stdout, "\tpcm_sample_bit_depth_chroma_minus1=%d\n", read_bits(pnal_buffer, 4));
		fprintf(stdout, "\tlog2_min_pcm_luma_coding_block_size_minus3=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tlog2_diff_max_min_pcm_luma_coding_block_size=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tpcm_loop_filter_disabled_flag=%d\n", read_bit(pnal_buffer));
	}
	uint32 num_short_term_ref_pic_sets = read_uev(pnal_buffer);
	fprintf(stdout, "\tnum_short_term_ref_pic_sets=%d\n", num_short_term_ref_pic_sets);
	for (int i = 0; i < num_short_term_ref_pic_sets; i++) {
		short_term_ref_pic_set(pnal_buffer, i, num_short_term_ref_pic_sets);
	}
	uint8 long_term_ref_pics_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tlong_term_ref_pics_present_flag=%d\n", long_term_ref_pics_present_flag);
	if (long_term_ref_pics_present_flag) {
		uint32 num_long_term_ref_pics_sps = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_long_term_ref_pics_sps=%d\n", num_long_term_ref_pics_sps);
		for (int i = 0; i < num_long_term_ref_pics_sps; i++) {
			fprintf(stdout, "\tlt_ref_pic_poc_lsb_sps[%d]=%d\n", i, read_bits(pnal_buffer, log2_max_pic_order_cnt_lsb_minus4)); // u(v)
			fprintf(stdout, "\tused_by_curr_pic_lt_sps_flag[i]=%d\n", read_bit(pnal_buffer));
		}
	}
	fprintf(stdout, "\tsps_temporal_mvp_enabled_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tstrong_intra_smoothing_enabled_flag=%d\n", read_bit(pnal_buffer));
	uint8 vui_parameters_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvui_parameters_present_flag=%d\n", vui_parameters_present_flag);
	if (vui_parameters_present_flag) {
		vui_parameters(pnal_buffer, sps_max_sub_layers_minus1);
	}
	fprintf(stdout, "\tsps_extension_flag=%d\n", read_bit(pnal_buffer));
}

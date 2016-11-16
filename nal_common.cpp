#include <stdio.h>

#include "types.h"
#include "cio.h"
#include "bio.h"
#include "nal_common.h"

void profile_tier_level(nal_buffer_t * pnal_buffer, int maxNumSubLayersMinus1) {
	fprintf(stdout, "\t--- profile_tier(%d) ---\n", maxNumSubLayersMinus1);
	fprintf(stdout, "\tgeneral_profile_space=%d\n", read_bits(pnal_buffer, 2));
	fprintf(stdout, "\tgeneral_tier_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_profile_idc=%d\n", read_bits(pnal_buffer, 5));
	for (int j = 0; j < 32; j++) {
		fprintf(stdout, "\tgeneral_profile_compatibility_flag[%d]=%d\n", j, read_bit(pnal_buffer));
	}
	fprintf(stdout, "\tgeneral_progressive_source_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_interlaced_source_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_non_packed_constraint_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_frame_only_constraint_flag=%d\n", read_bit(pnal_buffer));
	//general_reserved_zero_44bits u(44)
	read_bits(pnal_buffer, 44);
	fprintf(stdout, "\tskipping 44 reserved bits...\n");

	fprintf(stdout, "\tgeneral_level_idc=%d\n", read_bits(pnal_buffer, 8));
	uint8 sub_layer_profile_present_flag[8];
	uint8 sub_layer_level_present_flag[8];
	for (int i = 0; i < maxNumSubLayersMinus1; i++) {
		sub_layer_profile_present_flag[i] = read_bit(pnal_buffer);
		fprintf(stdout, "\tsub_layer_profile_present_flag[%d]=%d\n", i, sub_layer_profile_present_flag[i]);
		sub_layer_level_present_flag[i] = read_bit(pnal_buffer);
		fprintf(stdout, "\tsub_layer_level_present_flag[%d]=%d\n", i, sub_layer_level_present_flag[i]);
	}

	if (maxNumSubLayersMinus1 > 0) {
		for (int i = maxNumSubLayersMinus1; i < 8; i++) {
			fprintf(stdout, "\treserved_zero_2bits[%d]=%d\n", i, read_bits(pnal_buffer, 2));
		}
	}

	for (int i = 0; i < maxNumSubLayersMinus1; i++) {
		if (sub_layer_profile_present_flag[i]) {
			fprintf(stdout, "\tsub_layer_profile_space[%d]=%d\n", i, read_bits(pnal_buffer, 2));
			fprintf(stdout, "\tsub_layer_tier_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_profile_idc[%d]=%d\n", i, read_bits(pnal_buffer, 5));
			for (int j = 0; j < 32; j++) {
				fprintf(stdout, "\tsub_layer_profile_compatibility_flag[%d][%d]=%d\n", i, j, read_bit(pnal_buffer));
			}
			fprintf(stdout, "\tsub_layer_progressive_source_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_interlaced_source_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_non_packed_constraint_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_frame_only_constraint_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			//sub_layer_reserved_zero_44bits[i] u(44)
			read_bits(pnal_buffer, 44);
			fprintf(stdout, "\tskipping sub_layer_reserved_zero_44bits[%d]\n", i);
		}
		if (sub_layer_level_present_flag[i]) {
			fprintf(stdout, "\tsub_layer_level_idc[%d]=%d\n", i, read_bits(pnal_buffer, 8));
		}
	}
}

void short_term_ref_pic_set(nal_buffer_t * pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets) {
	fprintf(stdout, "\t--- short_term_ref_pic_set(%d, %d) ---\n", stRpsIdx, num_short_term_ref_pic_sets);
	uint8 inter_ref_pic_set_prediction_flag = 0;
	if (stRpsIdx != 0) {
		inter_ref_pic_set_prediction_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tinter_ref_pic_set_prediction_flag=%d\n", inter_ref_pic_set_prediction_flag);
	}

	if (inter_ref_pic_set_prediction_flag) {
		int delta_idx_minus1 = read_uev(pnal_buffer);
		if (stRpsIdx == num_short_term_ref_pic_sets) {
			fprintf(stdout, "\tdelta_idx_minus1=%d\n", delta_idx_minus1);
		}
		fprintf(stdout, "\tdelta_rps_sign=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\tabs_delta_rps_minus1=%d\n", read_uev(pnal_buffer));
		int RefRpsIdx = stRpsIdx - (delta_idx_minus1 + 1);
		printf("need NuDeltaPocs here !!!\n");
		/*
		for (j = 0; j <= NumDeltaPocs[RefRpsIdx]; j++) {
		uint8 used_by_curr_pic_flag_j = read_bit(pnal_buffer);
		fprintf(stdout, "\tused_by_curr_pic_flag[j]=%d\n", used_by_curr_pic_flag_j);
		if (!used_by_curr_pic_flag_j)
		fprintf(stdout, "\tuse_delta_flag[j]=%d\n", read_bit(pnal_buffer));
		}
		*/
	}
	else {
		uint32 num_negative_pics = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_negative_pics=%d\n", num_negative_pics);
		uint32 num_positive_pics = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_positive_pics=%d\n", num_positive_pics);
		for (int i = 0; i < num_negative_pics; i++) {
			fprintf(stdout, "\tdelta_poc_s0_minus1[i]=%d\n", read_uev(pnal_buffer));
			fprintf(stdout, "\tused_by_curr_pic_s0_flag[i]=%d\n", read_bit(pnal_buffer));
		}
		for (int i = 0; i < num_positive_pics; i++) {
			fprintf(stdout, "\tdelta_poc_s1_minus1[i]=%d\n", read_uev(pnal_buffer));
			fprintf(stdout, "\tused_by_curr_pic_s1_flag[i]=%d\n", read_bit(pnal_buffer));
		}
	}
}

void sub_layer_hrd_parameters(nal_buffer_t * pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag) {
	fprintf(stdout, "\t--- sub_layer_hrd_parameters(%d, %d) ---\n", CpbCnt, sub_pic_hrd_params_present_flag);
	for (int i = 0; i <= CpbCnt; i++) {
		fprintf(stdout, "\tbit_rate_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		fprintf(stdout, "\tcpb_size_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		if (sub_pic_hrd_params_present_flag) {
			fprintf(stdout, "\tcpb_size_du_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
			fprintf(stdout, "\tbit_rate_du_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		}
		fprintf(stdout, "\tcbr_flag[%d]=%d\n", i, read_bit(pnal_buffer));
	}
}

void hrd_parameters(nal_buffer_t * pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1) {
	fprintf(stdout, "\t--- hrd_parameters(%d, %d) ---\n", commonInfPresentFlag, maxNumSubLayersMinus1);
	uint8 nal_hrd_parameters_present_flag = 0;
	uint8 vcl_hrd_parameters_present_flag = 0;
	uint8 sub_pic_hrd_params_present_flag = 0;
	if (commonInfPresentFlag) {
		nal_hrd_parameters_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tnal_hrd_parameters_present_flag=%d\n", nal_hrd_parameters_present_flag);
		vcl_hrd_parameters_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvcl_hrd_parameters_present_flag=%d\n", vcl_hrd_parameters_present_flag);
		if (nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag) {
			sub_pic_hrd_params_present_flag = read_bit(pnal_buffer);
			fprintf(stdout, "\tsub_pic_hrd_params_present_flag=%d\n", sub_pic_hrd_params_present_flag);
			if (sub_pic_hrd_params_present_flag) {
				fprintf(stdout, "\ttick_divisor_minus2=%d\n", read_bits(pnal_buffer, 8));
				fprintf(stdout, "\tdu_cpb_removal_delay_increment_length_minus1=%d\n", read_bits(pnal_buffer, 5));
				fprintf(stdout, "\tsub_pic_cpb_params_in_pic_timing_sei_flag=%d\n", read_bit(pnal_buffer));
				fprintf(stdout, "\tdpb_output_delay_du_length_minus1=%d\n", read_bits(pnal_buffer, 5));
			}
			fprintf(stdout, "\tbit_rate_scale=%d\n", read_bits(pnal_buffer, 4));
			fprintf(stdout, "\tcpb_size_scale=%d\n", read_bits(pnal_buffer, 4));
			if (sub_pic_hrd_params_present_flag) {
				fprintf(stdout, "\tcpb_size_du_scale=%d\n", read_bits(pnal_buffer, 4));
			}
			fprintf(stdout, "\tinitial_cpb_removal_delay_length_minus1=%d\n", read_bits(pnal_buffer, 5));
			fprintf(stdout, "\tau_cpb_removal_delay_length_minus1=%d\n", read_bits(pnal_buffer, 5));
			fprintf(stdout, "\tdpb_output_delay_length_minus1=%d\n", read_bits(pnal_buffer, 5));
		}
	}

	for (int i = 0; i <= maxNumSubLayersMinus1; i++) {
		uint8 fixed_pic_rate_general_flag_i = read_bit(pnal_buffer);
		fprintf(stdout, "\tfixed_pic_rate_general_flag[%d]=%d\n", i, fixed_pic_rate_general_flag_i);
		uint8 fixed_pic_rate_within_cvs_flag_i = 0;
		if (!fixed_pic_rate_general_flag_i) {
			fixed_pic_rate_within_cvs_flag_i = read_bit(pnal_buffer);
		}
		fprintf(stdout, "\tfixed_pic_rate_within_cvs_flag[%d]=%d\n", i, read_bit(pnal_buffer));
		uint8 low_delay_hrd_flag_i = 0;
		if (fixed_pic_rate_within_cvs_flag_i) {
			fprintf(stdout, "\telemental_duration_in_tc_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		}
		else {
			low_delay_hrd_flag_i = read_bit(pnal_buffer);
		}
		fprintf(stdout, "\tlow_delay_hrd_flag[%d]=%d\n", i, low_delay_hrd_flag_i);
		uint8 cpb_cnt_minus1_i = 0;
		if (!low_delay_hrd_flag_i) {
			cpb_cnt_minus1_i = read_uev(pnal_buffer);
		}
		fprintf(stdout, "\tcpb_cnt_minus1[%d]=%d\n", i, cpb_cnt_minus1_i);
		if (nal_hrd_parameters_present_flag) {
			sub_layer_hrd_parameters(pnal_buffer, cpb_cnt_minus1_i, sub_pic_hrd_params_present_flag);
		}
		if (vcl_hrd_parameters_present_flag) {
			sub_layer_hrd_parameters(pnal_buffer, cpb_cnt_minus1_i, sub_pic_hrd_params_present_flag);
		}
	}
}

void vui_parameters(nal_buffer_t * pnal_buffer, uint8 sps_max_sub_layers_minus1) {
	fprintf(stdout, "\t--- vui_parameters(%d) ---\n", sps_max_sub_layers_minus1);
	uint8 aspect_ratio_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\taspect_ratio_info_present_flag=%d\n", aspect_ratio_info_present_flag);
	if (aspect_ratio_info_present_flag) {
		uint8 aspect_ratio_idc = read_bits(pnal_buffer, 8);
		fprintf(stdout, "\taspect_ratio_idc=%d\n", aspect_ratio_idc);
		if (aspect_ratio_idc == EXTENDED_SAR) {
			fprintf(stdout, "\tsar_width=%d\n", read_bits(pnal_buffer, 16));
			fprintf(stdout, "\tsar_height=%d\n", read_bits(pnal_buffer, 16));
		}
	}
	uint8 overscan_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\toverscan_info_present_flag=%d\n", overscan_info_present_flag);
	if (overscan_info_present_flag) {
		fprintf(stdout, "\toverscan_appropriate_flag=%d\n", read_bit(pnal_buffer));
	}
	uint8 video_signal_type_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvideo_signal_type_present_flag=%d\n", video_signal_type_present_flag);
	if (video_signal_type_present_flag) {
		fprintf(stdout, "\tvideo_format=%d\n", read_bits(pnal_buffer, 3));
		fprintf(stdout, "\tvideo_full_range_flag=%d\n", read_bit(pnal_buffer));
		uint8 colour_description_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tcolour_description_present_flag=%d\n", colour_description_present_flag);
		if (colour_description_present_flag) {
			fprintf(stdout, "\tcolour_primaries=%d\n", read_bits(pnal_buffer, 8));
			fprintf(stdout, "\ttransfer_characteristics=%d\n", read_bits(pnal_buffer, 8));
			fprintf(stdout, "\tmatrix_coeffs=%d\n", read_bits(pnal_buffer, 8));
		}
	}
	uint8 chroma_loc_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tchroma_loc_info_present_flag=%d\n", chroma_loc_info_present_flag);
	if (chroma_loc_info_present_flag) {
		fprintf(stdout, "\tchroma_sample_loc_type_top_field=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tchroma_sample_loc_type_bottom_field=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tneutral_chroma_indication_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tfield_seq_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tframe_field_info_present_flag=%d\n", read_bit(pnal_buffer));
	uint8 default_display_window_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tdefault_display_window_flag=%d\n", default_display_window_flag);
	if (default_display_window_flag) {
		fprintf(stdout, "\tdef_disp_win_left_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tdef_disp_win_right_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tdef_disp_win_top_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tdef_disp_win_bottom_offset=%d\n", read_uev(pnal_buffer));
	}
	uint8 vui_timing_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvui_timing_info_present_flag=%d\n", vui_timing_info_present_flag);
	if (vui_timing_info_present_flag) {
		fprintf(stdout, "\tvui_num_units_in_tick=%d\n", read_bits(pnal_buffer, 32));
		fprintf(stdout, "\tvui_time_scale=%d\n", read_bits(pnal_buffer, 32));
		uint8 vui_poc_proportional_to_timing_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvui_poc_proportional_to_timing_flag=%d\n", vui_poc_proportional_to_timing_flag);
		if (vui_poc_proportional_to_timing_flag) {
			fprintf(stdout, "\tvui_num_ticks_poc_diff_one_minus1=%d\n", read_uev(pnal_buffer));
		}
		uint8 vui_hrd_parameters_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvui_hrd_parameters_present_flag=%d\n", vui_hrd_parameters_present_flag);
		if (vui_hrd_parameters_present_flag) {
			hrd_parameters(pnal_buffer, 1, sps_max_sub_layers_minus1);
		}
	}
	uint8 bitstream_restriction_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tbitstream_restriction_flag=%d\n", bitstream_restriction_flag);
	if (bitstream_restriction_flag) {
		fprintf(stdout, "\ttiles_fixed_structure_flag=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\tmotion_vectors_over_pic_boundaries_flag=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\trestricted_ref_pic_lists_flag=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\tmin_spatial_segmentation_idc=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tmax_bytes_per_pic_denom=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tmax_bits_per_min_cu_denom=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tlog2_max_mv_length_horizontal=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tlog2_max_mv_length_vertical=%d\n", read_uev(pnal_buffer));
	}
}

void scaling_list_data(nal_buffer_t * pnal_buffer) {
	fprintf(stdout, "\tscaling_list_data() ---\n");
	for (int size_id = 0; size_id < 4; size_id++) {
		for (int matrix_id = 0; matrix_id < (size_id == 3) ? 2 : 6; matrix_id++) {
			uint8 scaling_list_pred_mode_flag = read_bit(pnal_buffer); // [sizeId][matrixId]
			fprintf(stdout, "\t\tscaling_list_pred_mode_flag[%d][%d] = %d\n", size_id, matrix_id, scaling_list_pred_mode_flag);

			uint32 scaling_list_pred_matrix_id_delta = 0;
			if (!scaling_list_pred_mode_flag) { // [sizeId][matrixId] 
				scaling_list_pred_matrix_id_delta = read_uev(pnal_buffer); // [sizeId][matrixId] 
				fprintf(stdout, "\t\tscaling_list_pred_matrix_id_delta[%d][%d] = %d\n", size_id, matrix_id, scaling_list_pred_matrix_id_delta);
			}
			else {
				uint8 next_coef = 8;
				uint32 coef_num = MIN(64, (1 << (4 + (size_id << 1))));

				if (size_id > 1) {
					uint32 scaling_list_dc_coef_minus8 = read_uev(pnal_buffer); // [size_id − 2][matrix_id]
					fprintf(stdout, "\t\tscaling_list_dc_coef_minus8[%d][%d] = %d\n", size_id, matrix_id, scaling_list_dc_coef_minus8);
					next_coef = scaling_list_dc_coef_minus8; //  [sizeId − 2][matrixId] + 8
				}
				for (int i = 0; i < coef_num; i++) {
					sint32 scaling_list_delta_coef = read_sev(pnal_buffer); // se(v)
					fprintf(stdout, "\t\tscaling_list_delta_coef = %d\n", scaling_list_delta_coef);

					next_coef = (next_coef + scaling_list_delta_coef + 256) % 256;
					//scaling_list[size_id][matrix_id][i] = next_coef;
				}
			}
		}
	}
}



#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "simple_log.h"
#include "simple_errno.h"
#include "simple_pcm_format_conversion.h"

#define PERIOD_FRAME_SIZE		640

static int int16_to_float32_process(const void *src, void *dest, int point_num)
{
	int index;
	if (!src || !dest) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "src or dest error");
		return SIMPLE_EPERM;
	}

	int16_t *input_src = (int16_t *)src;
	float *output_dest = (float *)dest;

	// normalization processing
	for (index = 0; index < point_num; index++) {
		output_dest[index] = input_src[index] / 32767.0;
	}
	return 0;
}

static int int32_to_float32_process(const void *src, void *dest, int point_num)
{
	int index;
	if (!src || !dest) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "src or dest error");
		return SIMPLE_EPERM;
	}

	int32_t *input_src = (int32_t *)src;
	float *output_dest = (float *)dest;

	// normalization processing
	for (index = 0; index < point_num; index++) {
		output_dest[index] = input_src[index] / 2147483647.0;
	}
	return 0;
}

static int float32_to_int16_process(const void *src, void *dest, int point_num)
{
	int index;
	if (!src || !dest) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "src or dest error");
		return SIMPLE_EPERM;
	}

	float *input_src = (float *)src;
	int16_t *output_dest = (int16_t *)dest;

	for (index = 0; index < point_num; index++) {
		output_dest[index] = (int16_t)(input_src[index] * 32767.0);
	}
	return 0;
}

static int float32_to_int32_process(const void *src, void *dest, int point_num)
{
	int index;
	if (!src || !dest) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "src or dest error");
		return SIMPLE_EPERM;
	}

	float *input_src = (float *)src;
	int32_t *output_dest = (int32_t *)dest;

	for (index = 0; index < point_num; index++) {
		output_dest[index] = (int32_t)(input_src[index] * 2147483647.0);
	}
	return 0;
}

static int int16_to_int32_process(const void *src, void *dest, int point_num)
{
	int index;
	if (!src || !dest) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "src or dest error");
		return SIMPLE_EPERM;
	}

	int16_t *input_src = (int16_t *)src;
	int32_t *output_dest = (int32_t *)dest;

	for (index = 0; index < point_num; index++) {
		output_dest[index] = (int32_t)input_src[index] << 16;
	}
	return 0;
}

static int int32_to_int16_process(const void *src, void *dest, int point_num)
{
	int index;
	if (!src || !dest) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "src or dest error");
		return SIMPLE_EPERM;
	}

	int32_t *input_src = (int32_t *)src;
	int16_t *output_dest = (int16_t *)dest;

	for (index = 0; index < point_num; index++) {
		output_dest[index] = (int16_t)(input_src[index] >> 16);
	}
	return 0;
}

int pcm_format_conversion_init(pcm_format_conversion_t **handle, pcm_format_t input, pcm_format_t output)
{
	assert(sizeof(float) == 4);

	pcm_format_conversion_t *tmp_handle = (pcm_format_conversion_t *)calloc(1, sizeof(pcm_format_conversion_t));
	if (!tmp_handle) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "open conversion_t error");
		return SIMPLE_ENOMEM;
	}
	tmp_handle->input_format = input;
	tmp_handle->output_format = output;

	if (PCM_FORMAT_INT_16BIT == input && PCM_FORMAT_FLOAT_32BIT == output)
		tmp_handle->process = int16_to_float32_process;
	else if (PCM_FORMAT_INT_32BIT == input && PCM_FORMAT_FLOAT_32BIT == output)
		tmp_handle->process = int32_to_float32_process;
	else if (PCM_FORMAT_FLOAT_32BIT == input && PCM_FORMAT_INT_16BIT == output)
		tmp_handle->process = float32_to_int16_process;
	else if (PCM_FORMAT_FLOAT_32BIT == input && PCM_FORMAT_INT_32BIT == output)
		tmp_handle->process = float32_to_int32_process;
	else if (PCM_FORMAT_INT_16BIT == input && PCM_FORMAT_INT_32BIT == output)
		tmp_handle->process = int16_to_int32_process;
	else if (PCM_FORMAT_INT_32BIT == input && PCM_FORMAT_INT_16BIT == output)
		tmp_handle->process = int32_to_int16_process;
	else {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "conversion of %d to %d is not supported", input, output);
		free(tmp_handle);
		return SIMPLE_EPERM;
	}
	*handle = tmp_handle;
	return SIMPLE_OK;
}

int pcm_format_conversion_process(pcm_format_conversion_t *handle, const void *src, void *dest, int point_num)
{
	if (!handle || !(handle->process)) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "conversion handle error");
		return SIMPLE_EPERM;
	}

	return handle->process(src, dest, point_num);
}

int pcm_format_conversion_uninit(pcm_format_conversion_t *handle)
{
	if (!handle) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "conversion handle error");
		return SIMPLE_EPERM;
	}
	free(handle);
	return SIMPLE_OK;
}

int pcm_format_get_bit_depth(pcm_format_t format, int *bit_depth)
{
	if (!bit_depth) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "bit_depth error");
		return SIMPLE_EPERM;
	}

	switch (format) {
	case PCM_FORMAT_FLOAT_32BIT:
		*bit_depth = 32;
		break;

	case PCM_FORMAT_INT_16BIT:
		*bit_depth = 16;
		break;

	case PCM_FORMAT_INT_32BIT:
		*bit_depth = 32;
		break;

	default:
		simple_log_err(SIMPLE_LOG_TYPE_MAIN, "intput type(%d) error", format);
		return SIMPLE_EPERM;
	}
	return SIMPLE_OK;
}

int file_pcm_format_conversion(const char *src_name, const char *dest_name,
	pcm_format_t intput, pcm_format_t output)
{
	int ret = SIMPLE_OK;
	int bit_depth;
	int out_depth;
	FILE *input_fid = NULL;
	FILE *output_fid = NULL;
	pcm_format_conversion_t *pcm_conversion_handle = NULL;
	void *input_src = NULL;
	void *output_src = NULL;

	int period_size;  // frames at 1 channel
	int read_size;

	ret = pcm_format_get_bit_depth(intput, &bit_depth);
	if (ret) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "pcm_format_get_bit_depth error %d", ret);
		return ret;
	}

	ret = pcm_format_get_bit_depth(output, &out_depth);
	if (ret) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "pcm_format_get_bit_depth error %d", ret);
		return ret;
	}

	period_size = PERIOD_FRAME_SIZE;  // frames at 1 channel
	read_size = period_size * bit_depth / 8;

	input_src = calloc(1, read_size);
	if (!input_src) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "malloc input src error");
		return SIMPLE_ENOMEM;
	}

	output_src = calloc(1, period_size * out_depth / 8);
	if (!output_src) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "malloc output src error");
		ret = SIMPLE_ENOMEM;
		goto exit0;
	}

	ret = pcm_format_conversion_init(&pcm_conversion_handle, intput, output);
	if (ret) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "pcm_format_conversion_init error %d", ret);
		goto exit1;
	}

	input_fid = fopen(src_name, "rb");
	if (!input_fid) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "open %s error %d", src_name, ret);
		goto exit2;
	}

	output_fid = fopen(dest_name, "wb");
	if (!output_fid) {
		simple_log_err(SIMPLE_LOG_TYPE_CONVERSION, "open %s error %d", dest_name, ret);
		goto exit3;
	}

	while (!feof(input_fid)) {
		ret = fread(input_src, 1, read_size, input_fid);
		int point_num = ret * 8 / bit_depth;
		pcm_conversion_handle->process(input_src, output_src, point_num);

		fwrite(output_src, point_num, out_depth / 8, output_fid);
	}
	ret = SIMPLE_OK;

	fclose(output_fid);

exit3:
	fclose(input_fid);

exit2:
	pcm_format_conversion_uninit(pcm_conversion_handle);

exit1:
	free(output_src);

exit0:
	free(input_src);
	return ret;
}

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "simple_errno.h"
#include "simple_log.h"

#include "simple_pcm_format_make.h"

#define PI  3.141592653

int simple_pcm_format_make(int channel_num, pcm_format_t pcm_format, int sample_rate,
                           int frequency, int time_len, int data_size, void *data_src)
{
    int ret = SIMPLE_OK;

    if (!data_src) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "data_src is null");
        return SIMPLE_EPERM;
    }

    int bit_depth = 0;
    ret = pcm_format_get_bit_depth(pcm_format, &bit_depth);
    if (ret != SIMPLE_OK) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "format(%d) error", pcm_format);
        return SIMPLE_EPERM;
    }

    int i, j;
    int frame_byte = channel_num * bit_depth / 8;
    int frame_num = time_len * sample_rate;
    int data_len = frame_num * frame_byte;

    if (data_len > data_size)
        return SIMPLE_EPERM;

    switch (pcm_format) {
    case PCM_FORMAT_INT_16BIT:
    {
        double tmp = 2.0 * PI * frequency / sample_rate;
        int16_t *data_src_16 = (int16_t *)data_src;
        for (i = 0; i < frame_num; i++) {
            for (j = 0; j < channel_num; j++) {
                data_src_16[channel_num * i + j] = sin(tmp * i) * 32767;
            }
        }
    }
        break;

    case PCM_FORMAT_INT_32BIT:
    {
        double tmp = 2.0 * PI * frequency / sample_rate;
        int32_t *data_src_32 = (int32_t *)data_src;
        for (i = 0; i < frame_num; i++) {
            for (j = 0; j < channel_num; j++) {
                data_src_32[channel_num * i + j] = sin(tmp * i) * 2147483647;
            }
        }
    }
        break;

    case PCM_FORMAT_FLOAT_32BIT:
    {
        double tmp = 2.0 * PI * frequency / sample_rate;
        float *data_src_float_32 = (float *)data_src;
        for (i = 0; i < frame_num; i++) {
            for (j = 0; j < channel_num; j++) {
                data_src_float_32[channel_num * i + j] = sin(tmp * i);
            }
        }
    }
        break;

    default:
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "bitdepth error\n");
        break;
    }
    return SIMPLE_OK;
}

int file_pcm_format_make(const char *file_name, int channel_num, pcm_format_t format, int sample_rate,
                         int frequency, int time_len)
{
    if (!file_name) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "file_name error");
        return SIMPLE_EPERM;
    }

    int ret = SIMPLE_OK;
    int bit_depth = 0;
    ret = pcm_format_get_bit_depth(format, &bit_depth);
    if (ret != SIMPLE_OK) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "pcm_format_get_bit_depth error: %d", ret);
        return ret;
    }

    int frame_byte = channel_num * bit_depth / 8;
    int frame_len = time_len * sample_rate;
    int data_size = frame_len * frame_byte;

    void *data_src = calloc(1, data_size);
    if (!data_src) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "data_src calloc error");
        return SIMPLE_ENOMEM;
    }

    ret = simple_pcm_format_make(channel_num, format, sample_rate, frequency, time_len, data_size, data_src);
    if (ret != SIMPLE_OK) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "simple_pcm_format_make error: %d", ret);
        goto exit0;
    }

    FILE *fid = fopen(file_name, "wb");
    if (!fid) {
        simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "fopen error");
        goto exit0;
    }

    fwrite(data_src, 1, data_size, fid);
    fclose(fid);
    free(data_src);

    return SIMPLE_OK;

exit0:
    free(data_src);
    return ret;
}

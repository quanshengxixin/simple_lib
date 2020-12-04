#ifndef __SIMPLE_PCM_FORMAT_MAKE_H__
#define __SIMPLE_PCM_FORMAT_MAKE_H__

#include "simple_pcm_format.h"

int simple_pcm_format_make(int channel_num, pcm_format_t pcm_format, int sample_rate,
                           int frequency, int time_len, int data_size, void *data_src);

int file_pcm_format_make(const char *file_name, int channel_num, pcm_format_t format, int sample_rate,
                         int frequency, int time_len);

#endif // __SIMPLE_PCM_FORMAT_MAKE_H__
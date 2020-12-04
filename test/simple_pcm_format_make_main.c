#include <stdio.h>
#include <stdlib.h>
#include "simple_errno.h"
#include "simple_log.h"

#include "simple_pcm_format_make.h"

int main(int argc, char **argv)
{
    char file_name[256] = { 0 };

    set_loglevel(SIMPLE_LOG_TYPE_PCM_MAKE, SIMPLE_LOG_DBG);

    int sample_rate = 44100;
    pcm_format_t format;
    int channel_num = 8;
    int time_len = 10;
    int frequency = 1000;
    for (format = PCM_FORMAT_FIRST; format < PCM_FORMAT_NUM; format ++) {
        char *name = pcm_format_get_name(format);
        if (!name) {
            simple_log_err(SIMPLE_LOG_TYPE_PCM_MAKE, "pcm_format_get_name error");
            return SIMPLE_EINVAL;
        }
        sprintf(file_name, "%d_%dch_%s.pcm", sample_rate, channel_num, pcm_format_get_name(format));
        file_pcm_format_make(file_name, channel_num, format, sample_rate, frequency, time_len);
    }
    return SIMPLE_OK;
}
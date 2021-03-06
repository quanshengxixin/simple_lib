#ifndef __SIMPLE_PCM_FORMAT_H__
#define __SIMPLE_PCM_FORMAT_H__

typedef enum {
	PCM_FORMAT_FIRST = 0,
	PCM_FORMAT_INT_16BIT = PCM_FORMAT_FIRST,
	PCM_FORMAT_INT_32BIT,
	PCM_FORMAT_FLOAT_32BIT,
	PCM_FORMAT_NUM,
} pcm_format_t;

int pcm_format_get_bit_depth(pcm_format_t format, int *bit_depth);

char * pcm_format_get_name(pcm_format_t format);

#endif // __SIMPLE_PCM_FORMAT_H__
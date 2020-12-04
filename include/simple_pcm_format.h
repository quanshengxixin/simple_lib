#ifndef __SIMPLE_PCM_FORMAT_H__
#define __SIMPLE_PCM_FORMAT_H__

typedef enum {
	PCM_FORMAT_INT_16BIT = 0,
	PCM_FORMAT_INT_32BIT,
	PCM_FORMAT_FLOAT_32BIT,
	PCM_FORMAT_NUM,
} pcm_format_t;

#endif // __SIMPLE_PCM_FORMAT_H__
#ifndef __PCM_FORMAT_CONVERSION_H__
#define __PCM_FORMAT_CONVERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PCM_FORMAT_INT_16BIT = 0,
	PCM_FORMAT_INT_32BIT,
	PCM_FORMAT_FLOAT_32BIT,
	PCM_FORMAT_NUM,
} pcm_format_t;

typedef struct {
	pcm_format_t input_format;
	pcm_format_t output_format;
	int(*process)(const void *src, void *dest, int point_num);
} pcm_format_conversion_t;

int pcm_format_conversion_init(pcm_format_conversion_t **handle, pcm_format_t input, pcm_format_t output);
int pcm_format_conversion_process(pcm_format_conversion_t *handle, const void *src, void *dest, int point_num);
int pcm_format_conversion_uninit(pcm_format_conversion_t *handle);

int pcm_format_get_bit_depth(pcm_format_t format, int *bit_depth);

int file_pcm_format_conversion(const char *src_name, const char *dest_name, pcm_format_t intput, pcm_format_t output);

#ifdef __cplusplus
}
#endif

#endif //__PCM_FORMAT_CONVERSION_H__
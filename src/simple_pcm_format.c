#include <stddef.h>
#include "simple_log.h"
#include "simple_errno.h"

#include "simple_pcm_format.h"

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
		return SIMPLE_EPERM;
	}
	return SIMPLE_OK;
}

char * pcm_format_get_name(pcm_format_t format)
{
	switch (format) {
	case PCM_FORMAT_FLOAT_32BIT:
		return "float32";

	case PCM_FORMAT_INT_16BIT:
		return "int16";

	case PCM_FORMAT_INT_32BIT:
		return "int32";

	default:
		return NULL;
	}
}

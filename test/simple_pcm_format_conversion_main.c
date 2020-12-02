#include "simple_log.h"
#include "simple_errno.h"

#include "simple_pcm_format_conversion.h"

int main(int argc, char **argv)
{
	int ret = SIMPLE_OK;
	set_loglevel(SIMPLE_LOG_TYPE_CONVERSION, SIMPLE_LOG_DBG);

#if 1
	ret = file_pcm_format_conversion("44100_2ch_16_origin.raw", "44100_2ch_float_new.raw",
		PCM_FORMAT_INT_16BIT, PCM_FORMAT_FLOAT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_float_new.raw", "44100_2ch_16_new.raw",
		PCM_FORMAT_FLOAT_32BIT, PCM_FORMAT_INT_16BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_16_origin.raw", "44100_2ch_32_new.raw",
		PCM_FORMAT_INT_16BIT, PCM_FORMAT_INT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_32_new.raw", "44100_2ch_16_new.raw",
		PCM_FORMAT_INT_32BIT, PCM_FORMAT_INT_16BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_32_origin.raw", "44100_2ch_float_new.raw",
		PCM_FORMAT_INT_32BIT, PCM_FORMAT_FLOAT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_float_new.raw", "44100_2ch_32_new.raw",
		PCM_FORMAT_FLOAT_32BIT, PCM_FORMAT_INT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_32_origin.raw", "44100_2ch_16_new.raw",
		PCM_FORMAT_INT_32BIT, PCM_FORMAT_INT_16BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_16_new.raw", "44100_2ch_32_new.raw",
		PCM_FORMAT_INT_16BIT, PCM_FORMAT_INT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_float_origin.raw", "44100_2ch_16_new.raw",
		PCM_FORMAT_FLOAT_32BIT, PCM_FORMAT_INT_16BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_16_new.raw", "44100_2ch_float_new.raw",
		PCM_FORMAT_INT_16BIT, PCM_FORMAT_FLOAT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_float_origin.raw", "44100_2ch_32_new.raw",
		PCM_FORMAT_FLOAT_32BIT, PCM_FORMAT_INT_32BIT);
	if (ret)
		return ret;
#endif

#if 1
	ret = file_pcm_format_conversion("44100_2ch_32_new.raw", "44100_2ch_float_new.raw",
		PCM_FORMAT_INT_32BIT, PCM_FORMAT_FLOAT_32BIT);
	if (ret)
		return ret;
#endif
	return ret;
}

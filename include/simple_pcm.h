#ifndef __SIMPLE_PCM_H__
#define __SIMPLE_PCM_H__

typedef signed long snd_pcm_sframes_t;
typedef unsigned long snd_pcm_uframes_t;

typedef enum _snd_pcm_stream {
	/** Playback stream */
	SND_PCM_STREAM_PLAYBACK = 0,
	/** Capture stream */
	SND_PCM_STREAM_CAPTURE,
	SND_PCM_STREAM_LAST = SND_PCM_STREAM_CAPTURE
} snd_pcm_stream_t;

typedef struct __snd_pcm_hw_params {

} snd_pcm_hw_params_t;

typedef struct __snd_pcm_sw_params {

} snd_pcm_sw_params_t;

typedef struct __snd_pcm {
    char *name;
    snd_pcm_stream_t stream;
} snd_pcm_t;

int snd_pcm_open(snd_pcm_t **pcmp, const char *name, 
		 snd_pcm_stream_t stream, int mode);

int snd_pcm_close(snd_pcm_t *pcm);

int snd_pcm_hw_params(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);

int snd_pcm_sw_params(snd_pcm_t *pcm, snd_pcm_sw_params_t *params);

int snd_pcm_prepare(snd_pcm_t *pcm);

int snd_pcm_drop(snd_pcm_t *pcm);

int snd_pcm_drain(snd_pcm_t *pcm);

snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm, const void *buffer, snd_pcm_uframes_t size);

snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size);
#endif
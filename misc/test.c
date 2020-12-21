/*******************************************************************************
Copyright (c) wubihe Tech. Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
Date Created:	2014-10-25
Author:			wubihe QQ:1269122125 Email:1269122125@qq.com
Description:	本例子解析WAV头，并打印音频相关信息
--------------------------------------------------------------------------------
Modification History
DATE          AUTHOR          DESCRIPTION
--------------------------------------------------------------------------------
********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <io.h>

#ifndef HAVE_INT32_T
typedef signed int int32_t;
#endif
#ifndef HAVE_INT16_T
typedef signed short int16_t;
#endif
#ifndef HAVE_U_INT32_T
typedef unsigned int u_int32_t;
#endif
#ifndef HAVE_U_INT16_T
typedef unsigned short u_int16_t;
#endif

#ifdef WORDS_BIGENDIAN
# define UINT32(x) SWAP32(x)
# define UINT16(x) SWAP16(x)
#else
# define UINT32(x) (x)
# define UINT16(x) (x)
#endif

typedef struct
{
	FILE *f;
	int channels;
	int samplebytes;
	int samplerate;
	int samples;
	int bigendian;
	int isfloat;
} pcmfile_t;


typedef struct
{
	u_int32_t label;           /* 'RIFF' */
	u_int32_t length;        /* Length of rest of file */
	u_int32_t chunk_type;      /* 'WAVE' */
}
riff_t;

typedef struct
{
	u_int32_t label;
	u_int32_t len;
}
riffsub_t;

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

#define WAVE_FORMAT_PCM		1
#define WAVE_FORMAT_FLOAT	3
#define WAVE_FORMAT_EXTENSIBLE	0xfffe
#define INPUT_FILE ("huangdun.wav")
#define OUTPUT_FILE ("huangdun_r48000_FMT_S16_c2.pcm")
struct WAVEFORMATEX
{
	u_int16_t wFormatTag;
	u_int16_t nChannels;
	u_int32_t nSamplesPerSec;
	u_int32_t nAvgBytesPerSec;
	u_int16_t nBlockAlign;
	u_int16_t wBitsPerSample;
	u_int16_t cbSize;
}
#ifdef __GNUC
__attribute__((packed))
#endif
;

struct WAVEFORMATEXTENSIBLE
{
	struct WAVEFORMATEX Format;
	union {
		u_int16_t wValidBitsPerSample;	// bits of precision
		u_int16_t wSamplesPerBlock;		// valid if wBitsPerSample==0
		u_int16_t wReserved;		// If neither applies, set to zero.
	} Samples;
	u_int32_t dwChannelMask;		// which channels are present in stream
	unsigned char SubFormat[16];		// guid
}
#ifdef __GNUC
__attribute__((packed))
#endif
;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

static unsigned char waveformat_pcm_guid[16] =
{
	WAVE_FORMAT_PCM,0,0,0,
	0x00, 0x00,
	0x10, 0x00,
	0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
};

static void unsuperr(const char *name)
{
	fprintf(stderr, "%s: file format not supported\n", name);
}


pcmfile_t *wav_open_read(const char *name, int rawinput)
{
	int i;
	int skip;
	FILE *wave_f;
	riff_t riff;
	riffsub_t riffsub;
	struct WAVEFORMATEXTENSIBLE wave;
	char *riffl = "RIFF";
	char *wavel = "WAVE";
	char *bextl = "BEXT";
	char *fmtl = "fmt ";
	char *datal = "data";
	int fmtsize;
	pcmfile_t *sndf;
	int dostdin = 0;

	if (!strcmp(name, "-"))
	{
#ifdef _WIN32
		_setmode(_fileno(stdin), O_BINARY);
#endif
		wave_f = stdin;
		dostdin = 1;
	}
	else if (!(wave_f = fopen(name, "rb")))
	{
		perror(name);
		return NULL;
	}

	if (!rawinput) // header input
	{
		if (fread(&riff, 1, sizeof(riff), wave_f) != sizeof(riff))
			return NULL;
		if (memcmp(&(riff.label), riffl, 4))
			return NULL;
		if (memcmp(&(riff.chunk_type), wavel, 4))
			return NULL;

		// handle broadcast extensions. added by pro-tools,otherwise it must be fmt chunk.
		if (fread(&riffsub, 1, sizeof(riffsub), wave_f) != sizeof(riffsub))
			return NULL;
		riffsub.len = UINT32(riffsub.len);

		if (!memcmp(&(riffsub.label), bextl, 4))
		{
			fseek(wave_f, riffsub.len, SEEK_CUR);

			if (fread(&riffsub, 1, sizeof(riffsub), wave_f) != sizeof(riffsub))
				return NULL;
			riffsub.len = UINT32(riffsub.len);
		}

		if (memcmp(&(riffsub.label), fmtl, 4))
			return NULL;
		memset(&wave, 0, sizeof(wave));

		fmtsize = (riffsub.len < sizeof(wave)) ? riffsub.len : sizeof(wave);
		if (fread(&wave, 1, fmtsize, wave_f) != fmtsize)
			return NULL;

		for (skip = riffsub.len - fmtsize; skip > 0; skip--)
			fgetc(wave_f);

		for (i = 0;; i++)
		{
			if (fread(&riffsub, 1, sizeof(riffsub), wave_f) != sizeof(riffsub))
				return NULL;
			riffsub.len = UINT32(riffsub.len);
			if (!memcmp(&(riffsub.label), datal, 4))
				break;
			if (i > 10)
				return NULL;

			for (skip = riffsub.len; skip > 0; skip--)
				fgetc(wave_f);
		}
		if (UINT16(wave.Format.wFormatTag) != WAVE_FORMAT_PCM && UINT16(wave.Format.wFormatTag) != WAVE_FORMAT_FLOAT)
		{
			if (UINT16(wave.Format.wFormatTag) == WAVE_FORMAT_EXTENSIBLE)
			{
				if (UINT16(wave.Format.cbSize) < 22) // struct too small
					return NULL;
				if (memcmp(wave.SubFormat, waveformat_pcm_guid, 16))
				{
					waveformat_pcm_guid[0] = WAVE_FORMAT_FLOAT;
					if (memcmp(wave.SubFormat, waveformat_pcm_guid, 16))
					{
						unsuperr(name);
						return NULL;
					}
				}
			}
			else
			{
				unsuperr(name);
				return NULL;
			}
		}
	}

	sndf = (pcmfile_t *)malloc(sizeof(*sndf));
	memset(sndf, 0, sizeof(*sndf));
	sndf->f = wave_f;

	if (UINT16(wave.Format.wFormatTag) == WAVE_FORMAT_FLOAT) {
		sndf->isfloat = 1;
	} else {
		sndf->isfloat = (wave.SubFormat[0] == WAVE_FORMAT_FLOAT);
	}
	if (rawinput)
	{
		sndf->bigendian = 1;
		if (dostdin)
			sndf->samples = 0;
		else
		{
			fseek(sndf->f, 0 , SEEK_END);
			sndf->samples = ftell(sndf->f);
			rewind(sndf->f);
		}
	}
	else
	{
		sndf->bigendian = 0;
		sndf->channels = UINT16(wave.Format.nChannels);
		sndf->samplebytes = UINT16(wave.Format.wBitsPerSample) / 8;
		sndf->samplerate = UINT32(wave.Format.nSamplesPerSec);
		sndf->samples = riffsub.len / (sndf->samplebytes * sndf->channels);
	}
	return sndf;
}

int wav_close(pcmfile_t *sndf)
{
	int i = fclose(sndf->f);
	free(sndf);
	return i;
}


int is()
{
	FILE *fpout;

	fpout=fopen(OUTPUT_FILE,"wb+");
	if(fpout == NULL)
	{
		printf("Create pcm file error\n");
		return -1;
	}

	pcmfile_t * pPcmFile = wav_open_read(INPUT_FILE, 0);

	printf("channels:%1d\t samplebytes:%1d\t samplerate:%06d\t samples:%07d\t bigendian:%1d\n",pPcmFile->channels,pPcmFile->samplebytes,pPcmFile->samplerate,pPcmFile->samples,pPcmFile->bigendian);

	unsigned short usSample;

	while(!feof(pPcmFile->f))
	{
		fread(&usSample,sizeof(unsigned short),1,pPcmFile->f);
		fwrite(&usSample,sizeof(unsigned short),1,fpout);

	}


	wav_close(pPcmFile);
	fclose(fpout);
	printf("Parser WAV Success!!");
	getchar();

	return 0;
}


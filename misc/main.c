#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

typedef struct {
    uint32_t id;
    uint32_t size;
} chunk_head;

typedef struct {
    uint32_t id;
    uint32_t size;
    uint32_t format;
} riff_chunk;

typedef struct {
    uint16_t audio_format;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t frame_size;
    uint16_t bitdepth;
} fmt_chunk;

void print_id(uint32_t id)
{
    char c_id[5] = {0};
    memcpy(c_id, &id, 4);
    printf("%s\n", c_id);
}

void help(char *common)
{
    printf("Usage: %s\n", common);
}

int main(int argc, char **argv)
{
    riff_chunk riff_info;
    fmt_chunk fmt_info;
    chunk_head chunk_head_info;
    char *file_name = "test_in.wav";
    int option_index;
    char c;
    static const char short_options[] = "hn:";
    static const struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"name", 1, 0, 'n'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (c) {
        case 'h':
            help(argv[0]);
            return 0;

        case 'n':
            file_name = optarg;
            printf("%s\n", file_name);
            break;
        default:
            fprintf(stderr, "Try '%s --help' for more information.\n", "wavRead");
            return 1;
        }
    }

    FILE * wav_fid = fopen(file_name, "rb");
    if (wav_fid < 0) {
        fprintf(stderr, "%s open fail!\n", file_name);
        return 1;
    }

    if (fread(&riff_info, 1, sizeof(riff_info), wav_fid) < sizeof(riff_info)) {
        printf("the wav file is small\n");
        fclose(wav_fid);
        return -1;
    }

    while (fread(&chunk_head_info, 1, sizeof(chunk_head_info), wav_fid) == sizeof(chunk_head_info)) {
        print_id(chunk_head_info.id);
        printf("chunk_head_info.size = %d\n", chunk_head_info.size);
        if (!memcmp(&(chunk_head_info.id), "fmt ", 4)) {
            long int current = ftell(wav_fid);
            printf("%d\n", sizeof(fmt_info));
            fread(&fmt_info, 1, sizeof(fmt_info), wav_fid);
            printf("fmt_info.audio_format = %d\n", fmt_info.audio_format);
            printf("fmt_info.channels = %d\n", fmt_info.channels);
            printf("fmt_info.sample_rate = %d\n", fmt_info.sample_rate);
            printf("fmt_info.byte_rate = %d\n", fmt_info.byte_rate);
            printf("fmt_info.frame_size = %d\n", fmt_info.frame_size);
            printf("fmt_info.bitdepth = %d\n", fmt_info.bitdepth);
            if (fmt_info.audio_format != 1) {
                printf("Support PCM format only\n");
                return 0;
            }
            fseek(wav_fid, current + chunk_head_info.size, 0);
        } else if (!memcmp(&(chunk_head_info.id), "data", 4)) {
            long int current = ftell(wav_fid);
            char pcm_name[128] = {0};
            char *tmp_buffer = (char *)malloc(fmt_info.frame_size);
            sprintf(pcm_name, "%d_%dbit_%dch.pcm", fmt_info.sample_rate, fmt_info.bitdepth, fmt_info.channels);
            printf("%s\n", pcm_name);
            FILE * pcm_fid = fopen(pcm_name, "wb");
            int index;
            for (index = 0; index < chunk_head_info.size / fmt_info.frame_size; index++) {
                fread(tmp_buffer, 1, fmt_info.frame_size, wav_fid);
                fwrite(tmp_buffer, 1, fmt_info.frame_size, pcm_fid);
            }
            fclose(pcm_fid);

            fseek(wav_fid, current + chunk_head_info.size, 0);
        } else {
            fseek(wav_fid, chunk_head_info.size, 1);
        }
    }
    fclose(wav_fid);
    return 0;
}

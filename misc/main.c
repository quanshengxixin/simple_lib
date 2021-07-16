#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

typedef struct {
    uint32_t id;
    int32_t size;
} subchunk_head;

typedef struct {
    uint32_t id;
    int32_t size;
    uint32_t format;
} riff_chunk;

typedef struct {
    int16_t audio_format;
    int16_t channels;
    int32_t sample_rate;
    int32_t byte_rate;
    int16_t frame_size;
    int16_t bitdepth;
} fmt_chunk;

void help(char *common)
{
    printf("Usage: %s\n", common);
}

static int check_riff_chunk_info(riff_chunk *riff_info)
{
    if (memcmp(&(riff_info->id), "RIFF", 4)) {
        printf("%s() %d: chunk id error\n", __func__, __LINE__);
        printf("chunk id is %.*s\n", 4, &riff_info->id);
        return -1;
    }

    if (memcmp(&(riff_info->format), "WAVE", 4)) {
        printf("%s() %d: chunk format error\n", __func__, __LINE__);
        printf("chunk format is %.*s\n", 4, &riff_info->format);
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    riff_chunk riff_info;
    fmt_chunk fmt_info;
    subchunk_head subchunk_head_info;

    long int fmt_current = 0;
    int32_t fmt_size = 0;
    long int data_current = 0;
    int32_t data_size = 0;
    char *file_name = "test2.wav";
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
            break;
        default:
            fprintf(stderr, "Try '%s --help' for more information.\n", "wavRead");
            return 1;
        }
    }

    printf("file_name = %s\n", file_name);
    FILE * wav_fid = fopen(file_name, "rb");
    if (wav_fid == NULL) {
        fprintf(stderr, "%s open fail!\n", file_name);
        return 1;
    }

    int ret = fread(&riff_info, 1, sizeof(riff_info), wav_fid);
    if (ret != sizeof(riff_info)) {
        printf("file size error\n");
        goto exit1;
    }
    printf("id = %.4s, size = %d, format = %.4s\n", &riff_info.id, riff_info.size, &riff_info.format);

    if (check_riff_chunk_info(&riff_info)) {
        goto exit1;
    }

    // start reading subchunk place
    while (fread(&subchunk_head_info, 1, sizeof(subchunk_head_info), wav_fid) == sizeof(subchunk_head_info)) {
        if (!memcmp(&(subchunk_head_info.id), "fmt ", 4)) {
            fmt_current = ftell(wav_fid);
            fmt_size = subchunk_head_info.size;
            printf("id = %.4s, size = %d\n", &subchunk_head_info.id, subchunk_head_info.size);
        }
        if (!memcmp(&(subchunk_head_info.id), "data", 4)) {
            data_current = ftell(wav_fid);
            data_size = subchunk_head_info.size;
            printf("id = %.4s, size = %d\n", &subchunk_head_info.id, subchunk_head_info.size);
        }
        if (fseek(wav_fid, subchunk_head_info.size, 1)) {
            printf("fseek error\n");
            goto exit1;
        }
    }

    printf("fmt current = %ld, fmt size = %d\n", fmt_current, fmt_size);
    printf("data current = %ld, data size = %d\n", data_current, data_size);

    if (fmt_current == 0 || fmt_size == 0 || data_current == 0 || data_size == 0) {
        printf("data or fmt error\n");
        goto exit1;
    }

    // read fmt info
    fseek(wav_fid, fmt_current, 0);
    fread(&fmt_info, 1, sizeof(fmt_info), wav_fid);

    printf("fmt_info.audio_format = %d\n", fmt_info.audio_format);
    printf("fmt_info.channels = %d\n", fmt_info.channels);
    printf("fmt_info.sample_rate = %d\n", fmt_info.sample_rate);
    printf("fmt_info.byte_rate = %d\n", fmt_info.byte_rate);
    printf("fmt_info.frame_size = %d\n", fmt_info.frame_size);
    printf("fmt_info.bitdepth = %d\n", fmt_info.bitdepth);

    if (fmt_info.audio_format != 1) {
        printf("audio format %d not support\n", fmt_info.audio_format);
        goto exit1;
    }

    // read data info
    fseek(wav_fid, data_current, 0);

    char out_filename[256] = { 0 };
    FILE *pcm_fid;
    sprintf(out_filename, "%s_%dHz_%dch_%dbit.pcm", file_name, fmt_info.sample_rate,
        fmt_info.channels, fmt_info.bitdepth);

    pcm_fid = fopen(out_filename, "wb");
    if (pcm_fid == NULL) {
        printf("open %s error\n", out_filename);
        goto exit1;
    }

    int32_t pcm_size = 1024 * fmt_info.frame_size;
    void *pcm_src = malloc(pcm_size);
    if (!pcm_src) {
        printf("malloc error \n");
        goto exit2;
    }

    while (data_size > 0) {
        if (data_size > pcm_size) {
            fread(pcm_src, 1, pcm_size, wav_fid);
            fwrite(pcm_src, 1, pcm_size, pcm_fid);
        } else {
            fread(pcm_src, 1, data_size, wav_fid);
            fwrite(pcm_src, 1, data_size, pcm_fid);
        }
        data_size -= pcm_size;
    }

    free(pcm_src);



exit2:
    fclose(pcm_fid);

    /*

    fread(&chunk_head_info, 1, sizeof(chunk_head_info), wav_fid);
    print_id(chunk_head_info.id);
    printf("size = %d\n", chunk_head_info.size);

    long bbb = ftell(wav_fid);
    printf("bbb = %ld\n", bbb);

    fseek(wav_fid, 0, 2);
    long aaa = ftell(wav_fid);
    printf("aaa = %ld\n", aaa);
    */

    /*
    fread(&riff_info, 1, sizeof(riff_info), wav_fid);
    print_id(riff_info.id);
    print_id(riff_info.format);


    fread(&fmt_info, 1, sizeof(fmt_info), wav_fid);
    print_id(fmt_info.id);
    printf("fmt_info.audio_format = %d\n", fmt_info.audio_format);
    printf("fmt_info.channels = %d\n", fmt_info.channels);
    printf("fmt_info.sample_rate = %d\n", fmt_info.sample_rate);
    printf("fmt_info.byte_rate = %d\n", fmt_info.byte_rate);
    printf("fmt_info.frame_size = %d\n", fmt_info.frame_size);
    printf("fmt_info.bitdepth = %d\n", fmt_info.bitdepth);
    */
    /*
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
    */

exit1:
    fclose(wav_fid);
    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "simple_txt_pcm.h"

#define ENABLE_DBG_LEVEL    0

#if ENABLE_DBG_LEVEL
#define printf_dbg  printf
#else
#define printf_dbg(x...)
#endif

#define printf_err  printf

int txt2pcm(char *txt_name, char *pcm_name, int bitdepth)
{
    int ret = 0;
    FILE *txt_fid = NULL;
    FILE *pcm_fid = NULL;
    uint32_t total_line = 0;

    if (!txt_name) {
        printf_err("txt_name is NULL\n");
        return -1;
    }

    if (!pcm_name) {
        printf_err("pcm_name is NULL\n");
        return -1;
    }

    txt_fid = fopen(txt_name, "r");
    if (!txt_fid) {
        ret = -1;
        printf_err("fopen %s fail!\n", txt_name);
        goto exit1;
    }
    printf_dbg("%s has been opened\n", txt_name);

    pcm_fid = fopen(pcm_name, "wb");
    if (!pcm_fid) {
        ret = -1;
        printf_err("fopen %s fail\n", pcm_name);
        goto exit2;
    }
    printf_dbg("%s has been opened\n", pcm_name);

    while (true) {
        uint32_t tmp32;
        uint16_t tmp16;
        int scanf_num = 0;
        if (bitdepth == 16) {
            scanf_num = fscanf(txt_fid, "%*f,%*d,%x", &tmp16);
        } else if (bitdepth == 32) {
            scanf_num = fscanf(txt_fid, "%*f,%*d,%x", &tmp32);
        }
        if (scanf_num < 1) {
            if (scanf_num == EOF) {
                printf_dbg("%s EOF\n", txt_name);
                break;
            } else {
                printf_err("fscanf fail: %d\n", scanf_num);
                break;
            }
        }
        printf_dbg("0x%08x, scanf_num = %d\n", tmp, scanf_num);
        total_line++;
        if (bitdepth == 16) {
            fwrite(&tmp16, sizeof(tmp16), 1, pcm_fid);
        } else if(bitdepth == 32) {
            fwrite(&tmp32, sizeof(tmp32), 1, pcm_fid);
        }
    }

    printf("total line = %u\n", total_line);
    fclose(pcm_fid);

exit2:
    fclose(txt_fid);

exit1:
    return ret;
}

int pcm2txt(char *pcm_name, char *txt_name, int bitdepth)
{
    int ret = 0;
    FILE *pcm_fid = NULL;
    FILE *txt_fid = NULL;

    if (!pcm_name) {
        printf_err("pcm_name is NULL\n");
        return -1;
    }

    if (!txt_name) {
        printf_err("txt_name is NULL\n");
        return -1;
    }

    pcm_fid = fopen(pcm_name, "rb");
    if (!pcm_fid) {
        ret = -1;
        printf_err("%s open fail\n", pcm_name);
        goto exit1;
    }
    printf_dbg("%s has been opened\n", pcm_name);

    txt_fid = fopen(txt_name, "w");
    if (!txt_fid) {
        ret = -1;
        printf_err("%s open fail\n", txt_name);
        goto exit2;
    }

    while (true) {
        uint32_t tmp;
        size_t elem_num = fread(&tmp, sizeof(tmp), 1, pcm_fid);
        if (elem_num != 1) {
            printf_dbg("fread fail, elem_num = %u\n", elem_num);
            break;
        }
        fprintf(txt_fid, "0x%08x\n", tmp);
    }

    fclose(txt_fid);

exit2:
    fclose(pcm_fid);

exit1:
    return ret;
}
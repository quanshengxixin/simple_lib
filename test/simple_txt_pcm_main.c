#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "simple_txt_pcm.h"

#define printf_err printf
#define printf_dbg printf

static const char *charge_format_type_name[CHANGE_TYPE_FORMAT_NUM] = {
    [CHANGE_TYPE_PCM_2_TXT] = "CHANGE_TYPE_PCM_2_TXT",
    [CHANGE_TYPE_TXT_2_PCM] = "CHANGE_TYPE_TXT_2_PCM",
};

static void txt_pcm_help()
{
    printf("-h:\n");
    printf("  --help:\t\t show help info\n\n");

    printf("-i:\n");
    printf("  --input-name:\t\t input file path\n\n");

    printf("-o:\n");
    printf("  --output-name:\t output file path\n\n");

    printf("-t:\n");
    printf("  --type:\t\t show change type\n");
    printf("\t0: pcm to txt\n");
    printf("\t1: txt to pcm\n\n");

    printf("-b:\n");
    printf("  --bitdepth:\t\t bitdepth 16bit or 32bit\n\n");
}

int main(int argc, char **argv)
{
    int ret = 0;
    int c;
    int option_index;
    char *input_name = "input.pcm";
    char *output_name = "output.txt";
    int bitdepth = 32;

    change_format_type change_type = CHANGE_TYPE_PCM_2_TXT;

    static const char short_options[] = "hi:o:t:b:";
    static const struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"input-name", 1, 0, 'i'},
        {"output-name", 1, 0, 'o'},
        {"type", 1, 0, 't'},
        {"bitdepth", 1, 0, 'b'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (c) {
        case 'h':
            txt_pcm_help();
            return ret;
            break;

        case 'i':
            input_name = optarg;
            break;

        case 'o':
            output_name = optarg;
            break;

        case 't':
            change_type = atoi(optarg);
            break;

        case 'b':
            bitdepth = atoi(optarg);
            break;

        default:
            break;
        }
    }

    /* params check */
    if (change_type < 0 || change_type >= CHANGE_TYPE_FORMAT_NUM) {
        printf_err("change_type = %d\n", change_type);
        return -1;
    }
    if (bitdepth != 16 && bitdepth != 32) {
        printf_err("bitdepth = %d fail\n", bitdepth);
        return -1;
    }
    printf_dbg("input name = %s\n", input_name);
    printf_dbg("output name = %s\n", output_name);
    printf_dbg("bitdepth = %d\n", bitdepth);
    printf_dbg("change type = %s\n", charge_format_type_name[change_type]);

    switch (change_type) {
    case CHANGE_TYPE_PCM_2_TXT:
        ret = pcm2txt(input_name, output_name, bitdepth);
        break;

    case CHANGE_TYPE_TXT_2_PCM:
        ret = txt2pcm(input_name, output_name, bitdepth);
        break;

    default:
        printf_dbg("change_type = %d\n", change_type);
        break;
    }

    return ret;
}
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "simple_txt_pcm.h"

#define printf_err printf
#define printf_dbg printf

static const char *charge_format_type_name[CHANGE_TYPE_FORMAT_NUM] = {
    [CHANGE_TYPE_PCM_2_TXT] = {"CHANGE_TYPE_PCM_2_TXT"},
    [CHANGE_TYPE_TXT_2_PCM] = {"CHANGE_TYPE_TXT_2_PCM"},
};

int main(int argc, char **argv)
{
    int ret = 0;
    int c;
    int option_index;
    char *input_name = "input.pcm";
    char *output_name = "output.txt";

    change_format_type change_type = CHANGE_TYPE_PCM_2_TXT;

    static const char short_options[] = "hi:o:t:";
    static const struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"input-name", 1, 0, 'i'},
        {"output-name", 1, 0, 'o'},
        {"type", 1, 0, 't'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (c) {
        case 'h':
            printf("help\n");
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

        default:
            break;
        }
    }

    /* params check */
    if (change_type < 0 || change_type >= CHANGE_TYPE_FORMAT_NUM) {
        printf_err("change_type = %d\n", change_type);
        return -1;
    }
    printf_dbg("input name = %s\n", input_name);
    printf_dbg("output name = %s\n", output_name);
    printf_dbg("change type = %s\n", charge_format_type_name[change_type]);

    switch (change_type) {
    case CHANGE_TYPE_PCM_2_TXT:
        ret = pcm2txt(input_name, output_name);
        break;

    case CHANGE_TYPE_TXT_2_PCM:
        ret = txt2pcm(input_name, output_name);
        break;

    default:
        printf_dbg("change_type = %d\n", change_type);
        break;
    }

    return ret;
}
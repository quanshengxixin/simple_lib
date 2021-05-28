#ifndef __SIMPLE_TXT_PCM_H__
#define __SIMPLE_TXT_PCM_H__

typedef enum {
    CHANGE_TYPE_PCM_2_TXT = 0,
    CHANGE_TYPE_TXT_2_PCM = 1,
    CHANGE_TYPE_FORMAT_NUM = 2,
} change_format_type;

int pcm2txt(char *pcm_name, char *txt_name);
int txt2pcm(char *txt_name, char *pcm_name);

#endif
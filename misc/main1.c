#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define COMA (0x2c)
#define SPACE (0x20)
#define HEX (0x78)
#define FLAG_0D (0x0D)
#define FLAG_0A (0x0A)

static unsigned char f(const unsigned char *t)
{
    switch (*t){
        case '0': return 0x0;
        case '1': return 0x1;
        case '2': return 0x2;
        case '3': return 0x3;
        case '4': return 0x4;
        case '5': return 0x5;
        case '6': return 0x6;
        case '7': return 0x7;
        case '8': return 0x8;
        case '9': return 0x9;
        case 'a':
        case 'A': return 0xA;
        case 'b':
        case 'B': return 0xB;
        case 'c':
        case 'C': return 0xC;
        case 'd':
        case 'D': return 0xD;
        case 'e':
        case 'E': return 0xE;
        case 'f':
        case 'F': return 0xF;  
        default: return 0x0;
    }
}
void dataRegular(FILE *fin, unsigned char *buf,unsigned char *buf_raw, unsigned int *p_vsize)
{
    unsigned char cbuf[1];
	//unsigned short sbuf[1];
	unsigned char *p = buf_raw;
	unsigned char *q = buf;
    int n;
	int count = 0;
	while(1) {
        n = fread(cbuf, 1, 1, fin);
        if(n != 1)
			break;

		if (cbuf[0] == COMA || cbuf[0] == SPACE || cbuf[0] == FLAG_0D || cbuf[0] == FLAG_0A)
			continue;

		if (cbuf[0] == HEX) { /* backspace 0x's 0 */
			p--;
			count--;
			continue;
		}
		*(p++) = cbuf[0];
		count++;
	}
	p = buf_raw;
    while(count){
        if ((*p >= '0' && *p <='9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F')){
            *(q++) = *p;
            (*p_vsize)++;
        }
		p++;
		count--;
    }
}

int main(int argc, char *argv[])
{
    FILE *txtin, *pcmout;
    const char *txtname, *pcmname;   
    unsigned char *buf1;
    unsigned char *buf1_raw;
    unsigned char *buf2;
    unsigned char *p;
    unsigned int size;
    unsigned int valid_size = 0; //文本中有效字符个数
    unsigned char ch1;
    unsigned char ch2;
    unsigned char ch3;
    int i, j;
    int input1, input2;

#if 1
	if (argc != 5){
		printf("Usage:   txt2pcm.exe [-f] [-e] [-c] [-o]\n");
		printf("   -f:  full name of the file which contain textual data(like a.txt).\n");
		printf("   -e:  0 is little endian, 1 is big endian.\n");
		printf("   -c:  Bytes of a sample, could be 1, 2, 3, 4.\n");
		printf("   -o:  full name of the output file which contain binary data(like aout.pcm)");
        return 0;
    }
    txtname = argv[1];
	input1 = atoi(argv[2]);
	input2 = atoi(argv[3]);
    pcmname = argv[4];
#else
	txtname = "test1.txt";
	input1 = 0;
	input2 = 4;
    pcmname = "test1.pcm";
#endif
    txtin = fopen(txtname, "r");
    pcmout = fopen(pcmname, "wb");
    if (!txtin){
        printf("Error! Open input file error.\n");
        return 0;
    }
    if (!pcmout){
        printf("Error! Open output file error.\n");
        return 0;
    }
    //get file size
    fseek(txtin, 0, SEEK_END);
    size = ftell(txtin);
    printf("Text data file size = %dByte.\n", size);
    fseek(txtin, 0, SEEK_SET);    
    //set buffer
    buf1 = (unsigned char*)malloc(sizeof(unsigned char)*size);
    buf1_raw = (unsigned char*)malloc(sizeof(unsigned char)*size);
    buf2 = (unsigned char*)malloc(sizeof(unsigned char)*size/2);
    memset(buf1, 0, sizeof(size));
    memset(buf2, 0, sizeof(size));
    dataRegular(txtin, buf1,buf1_raw, &valid_size);
    printf("valid size = %uByte.\n", valid_size);
    if ((valid_size%2) != 0){//有效字符个数应为偶数
        printf("Error: number of valid character should be even.\n");
        return 0;
    }
    p = buf2;
    for (i = 0; i < valid_size; i+=2){
        ch1 = f(&buf1[i]);
        ch2 = f(&buf1[i+1]);
        ch3 = (ch1 << 4)|ch2; 
        *(p++) = ch3;
    }
    if (input1 == 0){//little endian
        if(input2 == 2){// 2Byte
            for (j = valid_size/2 - 1; j >= 0; j-=2){
                ch1 = buf2[j];
                buf2[j] = buf2[j-1];
                buf2[j-1] = ch1;
            }
        }
		if (input2 == 3){
			for (j = valid_size/2 - 1; j >=0; j-=3){
				ch1 = buf2[j];
				buf2[j] = buf2[j-2];
				buf2[j-2] = ch1;
			}
		}
        if (input2 == 4){//4Byte
            for (j = valid_size/2 - 1; j >= 0; j-=4){
                ch1 = buf2[j];
                buf2[j] = buf2[j-3];
                buf2[j-3] = ch1;
                ch2 = buf2[j-1];
                buf2[j-1] = buf2[j-2];
                buf2[j-2] = ch2;
            }
        }    
    }
    fwrite(buf2, 1, valid_size/2, pcmout);
    fclose(txtin);
    fclose(pcmout);
    printf("Done! You get a file named '%s'.\n", pcmname);
    return 0;
}
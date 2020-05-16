#ifndef DS_LAB1_COMPRESS_H
#define DS_LAB1_COMPRESS_H

#include "Huffman.h"

struct HEAD
{
    char type[4];   // file type
    int length;     // origin file length
    int weight[256]; // weight number
};

int compress(const char* fileName);
char str2byte(const char *pBinStr);
int encode(const char *filename, HuffmanCode pHC, char *&pBuf, int size);
int initHead(const char* filename, HEAD &head);
long long writeFile(const char* filename, HEAD head, const char *pBuf, int size);

#endif //DS_LAB1_COMPRESS_H

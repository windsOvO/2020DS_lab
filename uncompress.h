#ifndef DS_LAB1_COMPRESS_H
// 条件编译
struct HEAD
{
    char type[4];   // file type
    int length;     // origin file length
    int weight[256]; // weight number
};
#endif


#ifndef DS_LAB1_UNCOMPRESS_H
#define DS_LAB1_UNCOMPRESS_H

#include "Huffman.h"

int uncompress(const char* filename);
void byte2str(int byte, char* pBinStr);
int decodeAndRecover(const char *filename, HuffmanTree &pHT, int size);

#endif //DS_LAB1_UNCOMPRESS_H

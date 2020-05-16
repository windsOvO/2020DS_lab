#include "compress.h"

#include <cstdio>
#include <cstring>
#include "Huffman.h"

int compress(const char* filename)
{
    int weight[256] = {0};  // 每种字节出现的情况，weight[ch]
    FILE *fp = nullptr;
    if (!(fp = fopen(filename, "rb")))
    {
        printf("文件打开失败！\n");
        return 0;
    }
    int ch;
    long long len1 = 0;
    while ((ch = getc(fp)) != EOF)
    {
        weight[ch]++;
        len1++;
    }
    fclose(fp);
    // output appearance condition of 256 kinds of byte
    HuffmanTree pHT; // store Huffman tree
    HuffmanCode pHC; // store Huffman code
    creatHuffmanTree(pHT, weight, 256);  // build Huffman Tree
    // encode
    huffmanCoding(pHC, pHT);
    int size = 0;
    for (int i = 0; i < 256; i++)
    {
        size += weight[i] * int(strlen(pHC[i]) );
    }
    size = (size % 8) ? size / 8 + 1 : size / 8;
    // compress origin code to huffman code
    char* pBuf = nullptr;
    encode(filename, pHC, pBuf, size);
    HEAD head;
    initHead(filename, head);
    long long len2 = writeFile(filename, head, pBuf, size);
    // free memory
    delete[] pHT;
    delete[] pBuf;
    for (int i = 0; i < 256; i++)
        delete[] pHC[i];
    delete[] pHC;
    printf("原文件字节大小：%lld\n", len1);
    printf("压缩后字节大小：%lld\n", len2);
    printf("压缩率为：%f%%\n", len2 * 100.0 / len1 );
    return 0;
}

// 8 characters to 2 characters
// fake-byte
char str2byte(const char *pBinStr)
{
    char b = 0x00;
    for (int i = 0; i < 8; i++)
    {
        b = b << 1; // left shift
        if (pBinStr[i] == '1')
            b = b | 0x01;
    }
    return b;
}

int encode(const char *filename, HuffmanCode pHC, char *&pBuf, int size) // *&puf use reference of pointer
{
    pBuf = new char[size]; // buffer zone
    char tmp[256] = {0}; // work zone
    int pos = 0; // buffer zone pointer
    FILE *fp = nullptr;
    if (!(fp = fopen(filename, "rb")))
    {
        printf("文件打开失败！\n");
        return 0;
    }
    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        unsigned char hc = ch;
        printf("%d", ch);
        strcat(tmp, pHC[ch]); // copy HC to tmp
        // compress code
        while (strlen(tmp) >= 8)
        {
            pBuf[pos++] = str2byte(tmp);
            for (int i = 0; i < 256 - 8; i++) // bodily movement
                tmp[i] = tmp[i + 8];
        }
    }
    if (strlen(tmp) > 0) // 剩余
        pBuf[pos] = str2byte(tmp);
    fclose(fp);
    return 0;
}

int initHead(const char* filename, HEAD &head)
{
    // initial file head
    strcpy(head.type, "HUF");
    head.length = 0;
    for (int i = 0; i < 256; i++)
        head.weight[i] = 0;
    // read file
    FILE *fp = nullptr;
    if (!(fp = fopen(filename, "rb")))
    {
        printf("文件打开失败！\n");
        return 0;
    }
    int ch;
    while ((ch = getc(fp)) != EOF)
    {
        head.weight[ch]++;
        head.length++;
    }
    fclose(fp);
    return 0;
}

long long writeFile(const char* filename, HEAD head, const char *pBuf, int size)
{
    // generate filename
    char newFilename[256] = {0};
    strcpy(newFilename, filename);
    strcat(newFilename, ".huf");
    // open file
    FILE *fp = nullptr;
    if (!(fp = fopen(newFilename, "wb")))
    {
        printf("文件打开失败！\n");
        return 0;
    }
    // write file head
    fwrite(&head, sizeof(HEAD), 1, fp);
    // write new code
    fwrite(pBuf, sizeof(char), size, fp);
    fclose(fp);
    printf("===== 文件压缩成功！ =====\n");
    long long len = int(sizeof(HEAD)) + size;
    return len;
}
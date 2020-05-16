#include "uncompress.h"

#include <cstdio>
#include <cstring>
#include "Huffman.h"

int uncompress(const char* filename)
{
    FILE *fp = nullptr;
    if (!(fp = fopen(filename, "rb")))
    {
        printf("文件打开失败！\n");
        return 0;
    }
    HEAD head;
    fread(&head, sizeof(HEAD), 1, fp);
    if (strcmp(head.type, "HUF") != 0)
    {
        printf("非HUF格式，无法解压！\n");
        return 0;
    }
    HuffmanTree pHT; // store Huffman tree
    HuffmanCode pHC; // store Huffman code
    creatHuffmanTree(pHT, head.weight, 256);
//    huffmanCodint(pHC, pHT);
    int size = head.length;
    // restore
    decodeAndRecover(filename, pHT, size);
    return 0;
}

void byte2str(int byte, char* pBinStr)
{
    for (int i = 7; i >= 0; i--)
    {
        if (byte % 2 == 1)
            pBinStr[i] = '1';
        else
            pBinStr[i] = '0';
        byte /= 2;
    }
}

int decodeAndRecover(const char *filename, HuffmanTree &pHT, int size)
{
    // fp1
    char newFilename[256] = {0};
    strcpy(newFilename, filename);
    int len = strlen(newFilename);
    newFilename[len - 4] = '\0';
    FILE *fpOut = nullptr;
    if (!(fpOut = fopen(newFilename, "rb")))
    {
        printf("文件创建失败！\n");
        return 0;
    }
    // fp2
    FILE *fp = nullptr;
    if (!(fp = fopen(filename, "rb")))
    {
        printf("文件打开失败！\n");
        return 0;
    }
    // filter head
    HEAD head;
    fread(&head, sizeof(HEAD), 1, fp);
    // search on Huffman Tree
    int root = 511;
    char tmp[8] = {0}; // work zone - 8 bit buffer store file code
    int posOfTmp = 8; // work zone pointer
    int cnt = 0;
    while (cnt != size)
    {
        if (posOfTmp == 8) // work zone empty
        {
            int ch;
            if ((ch = fgetc(fp)) == EOF)
                printf("error2\n");
            byte2str(ch, tmp);
            cnt++;
            posOfTmp = 0;
        }
        if (pHT[root].lchild == 0 && pHT[root].rchild == 0)
        {
            cnt++;
            fputc(root - 1, fpOut);  // recover file
            root = 511;
        }
        else if (tmp[posOfTmp] == '0')
        {
            posOfTmp++;
            root = pHT[root].lchild;
        }
        else
        {
            posOfTmp++;
            root = pHT[root].rchild;
        }
    }
    fclose(fp);
    fclose(fpOut);
    printf("===== 文件解压成功！ =====\n");
    return 0;
}


// when use fgetc and fwrite, buffer must be [unsigned char]
// pay attention to binary code between char[-127-128] and unsigned char[0-256]

// distinguish char and byte in cpp

// fgetc: obtain unsigned char and add -1 then cast to int



//int restoreFile(const char* filename, unsigned char *pBuf, int size)
//{
//    // generate filename
//    char newFilename[256] = {0};
//    strcpy(newFilename, filename);
//    int len = strlen(newFilename);
//    newFilename[len - 4] = '\0';
//    // open file
//    FILE *fp = nullptr;
//    if (!(fp = fopen(newFilename, "wb")))
//    {
//        printf("文件创建失败！\n");
//        return 0;
//    }
//    // write new code
//    fwrite(pBuf, sizeof(unsigned char), size, fp);
//    fclose(fp);
//    printf("===== 文件解压成功！ =====\n");
//    return 0;
//}
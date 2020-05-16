#ifndef DS_LAB1_HUFFMAN_H
#define DS_LAB1_HUFFMAN_H

#include <string>

// Huffman Tree Node
struct HTNode
{
    int weight;
    int parent;
    int lchild;
    int rchild;
};

// Huffman Tree
typedef HTNode *HuffmanTree;

// Huffman Code
typedef char** HuffmanCode;

// use weight[i] to build HuffmanTree
int creatHuffmanTree(HuffmanTree &pHT, int *w, int n);
// improved traverse [record path if every node]
// use HuffmanTree to allocate binary code to every type of byte
int huffmanCoding(HuffmanCode &pHC, HuffmanTree &pHT);

// search the node of minimum weight - non use
int selectMinWeight(HuffmanTree pHT, int size);

// test function 1
void testHuffmanTree(HuffmanTree pHT);
// pre-oder output weight - test function
void preOrderTraverse(int root, HuffmanTree pHT);
// test function 2
void testHufCode(int root, HuffmanTree pHT, HuffmanCode pHC);

#endif //DS_LAB1_HUFFMAN_H

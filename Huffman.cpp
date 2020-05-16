#include "Huffman.h"

#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

// pHT [1 - 511]: 511 node, pHT[0] is flag
int creatHuffmanTree(HuffmanTree &pHT, int *w, int n)
{
    pHT = new HTNode[2 * n];
    int cnt = 1;
    // small root heap | store division condition every stages
    // pair<weight, node number>, compare left first
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > q;
    // initial queue
    for (int i = 0; i < n; i++) // the first 256 correspond to the type of their bytes
    {
        pHT[cnt].weight = w[i];
        pHT[cnt].lchild = pHT[cnt].rchild = 0; // initial
        pair<int, int> p(w[i], cnt);
        q.push(p);
        cnt++;
    }
    // Huffman
    while (q.size() != 1) // pop 2 node together and pHT[511].parent == 0
    {
        pHT[cnt].weight = q.top().first;
        pHT[cnt].lchild = q.top().second;
        q.pop();
        pHT[cnt].weight += q.top().first; // sum
        pHT[cnt].rchild = q.top().second;
        pHT[pHT[cnt].rchild ].parent = cnt;
        pHT[pHT[cnt].lchild ].parent = cnt;
        q.pop();
        pair<int, int> p(pHT[cnt].weight, cnt);
        q.push(p);
        cnt++;
    }
    return 0;
}


// pHC[0-255] correspond to 2^8 type of byte
int huffmanCoding(HuffmanCode &pHC, HuffmanTree &pHT)
{
    pHC = new char*[256]; // allocate memory 0-255
    char path[256] = {'\0'}; // path recording
    int len = 0; // length of path
    for (int i = 1; i < 512; i++) // use as flag
        pHT[i].weight = 0;
    int p = 511; // root
    // weight == flag: 0-not visited, 1-left child is visited, 2-right child is visited
    while (p != 0)
    {
        if (pHT[p].weight == 0) // is not visited, go left
        {
            pHT[p].weight = 1; // mark
            if (pHT[p].lchild != 0)
            {
                p = pHT[p].lchild;
                path[len++] = '0';
            }
            else if (pHT[p].rchild == 0) // leaf node, deal with problem
            {
                pHC[p - 1] = new char[len + 1];
                path[len] = '\0';
                strcpy(pHC[p - 1], path);
            }
        }
        else if (pHT[p].weight == 1) // go right
        {
            pHT[p].weight = 2; // mark
            if (pHT[p].rchild != 0)
            {
                p = pHT[p].rchild;
                path[len++] = '1';
            }
        }
        else // traceback
        {
            pHT[p].weight = 0;
            p = pHT[p].parent;
            len--;
        }
    }
    return 0;
}


// 256 = 2^8
int selectMinWeight(HuffmanTree pHT, int size)
{
    int minValue = 0x7fffffff;
    int min = 0; // number of minimum element
    // search
    for (int i = 1; i <= size; i++)
    {
        if (pHT[i].parent == 0 && pHT[i].weight < minValue)
        {
            minValue = pHT[i].weight;
            min = i;
        }
    }
    return min;
}


/*
 *  test function
 */


void testHuffmanTree(HuffmanTree pHT)
{
    printf("Huffman树的每个节点信息为：\n");
    printf("Byte\tWeight\tParent\tLchild\tRchild\n");
    for (int i = 1; i < 512; i++)
    {
        printf("pHT[%d]\t%4d\t%4d\t%4d\t%4d\n", i, pHT[i].weight,
               pHT[i].parent, pHT[i].lchild, pHT[i].rchild);
    }
}

void preOrderTraverse(int root, HuffmanTree pHT)
{
    printf("%d ", pHT[root].weight);
    if (pHT[root].lchild)
        preOrderTraverse(pHT[root].lchild, pHT);
    if (pHT[root].rchild)
        preOrderTraverse(pHT[root].rchild, pHT);
}

void testHufCode(int root, HuffmanTree pHT, HuffmanCode pHC)
{
    if (pHT[root].lchild == 0 && pHT[root].rchild == 0) // traverse leaves
        printf("0x%02X %s\n", root - 1, pHC[root - 1]);
    if (pHT[root].lchild)
        testHufCode(pHT[root].lchild, pHT, pHC);
    if (pHT[root].rchild)
        testHufCode(pHT[root].rchild, pHT, pHC);
}
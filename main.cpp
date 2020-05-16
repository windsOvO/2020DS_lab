#include <iostream>
#include "compress.h"
#include "uncompress.h"
using namespace std;

int main()
{
    cout << "======== 欢迎使用文件解压缩程序 ========" << endl;
    cout << "==========  by cl-winds  ===========" << endl;
    cout << "1. Huffman文件压缩" << endl;
    cout << "2. Huffman文件解压" << endl;
    cout << "请输入功能选项:";
    int opt = 1;
    while (cin >> opt && !(opt == 1 || opt == 2))
    {
        cout << "请重新输入功能选项:";
    }
    cout << "请输入文件名:";
    char filename[256];
    cin >> filename;
    if (opt == 1)
        compress(filename);
    else
        uncompress(filename);
    return 0;
}

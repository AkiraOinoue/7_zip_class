// 7_zip_TestPrg.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <Windows.h>
#include <iostream>
#include <string>
#include "CZipMethod.h"
using namespace std;
int main(int agc, char* agv[])
{
    string opt = "";
    string srcfile = "";
    string dstfile = "";
    if (agc < 3)
    {
        return 1;
    }
    else if (agc == 4)
    {
        opt = agv[1];
        srcfile = agv[2];
        dstfile = agv[3];
    }
    else
    {
        srcfile = agv[1];
        dstfile = agv[2];
    }
    auto lpzip = new ZIP::ArchiveMethod;
    BOOL ret;
    // 解凍します
    if (opt == "-d")
    {
        ret = lpzip->Extract(
            srcfile.c_str(),
            dstfile.c_str()
        );
    }
    // 圧縮します
    else
    {
        ret = lpzip->Compress(
            srcfile.c_str(),
            dstfile.c_str()
        );
    }
    if (ret == 1)
    {
        std::cout << "処理は成功です" << std::endl;
    }
    else
    {
        std::cout << "処理は失敗です" << std::endl;
    }
}

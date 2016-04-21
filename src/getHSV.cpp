/*************************************************************************
	> File Name: getHSV.cpp
	> Author: lumujin
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年04月16日 星期六 17时09分57秒
 ************************************************************************/
#include "BoWBuilder.h"
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char** argv){
    if (argc != 2){
	cout << "./bin/gethsv <filenamelist>" << endl;
	return 0;
    }
    auto file_path = ReadAllLinesFromFile(argv[1]);

    cout << file_path.size() << endl;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < file_path.size(); i++){
        string str1 = "./bin/hsv " + file_path[i] + " > " + file_path[i] + "_.txt";
 	cout << str1.c_str() << endl;
        system(str1.c_str());
        string str2 = "rm " + file_path[i];
 	cout << str2.c_str() << endl;
        system(str2.c_str());
    }
    string str = "rm " + string(argv[1]);
    system(str.c_str());

    return 0;
}

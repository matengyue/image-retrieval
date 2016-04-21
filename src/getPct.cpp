/*************************************************************************
	> File Name: getJpg.cpp
	> Author: lumujin
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年04月19日 星期二 10时46分01秒
 ************************************************************************/
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char ** argv){
    ifstream infile;
    infile.open(argv[1], ios::in);

    if (!infile){
        cout << "Can not open the file." << endl;
        return 0;
    }

    string str, com;
    while (!infile.eof()){
        infile >> str;
 	com = "cat " + str + " >> line";
	system(com.c_str());
    }

    return 0;
}

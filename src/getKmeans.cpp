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
#include <sstream>
using namespace std;

int main(int argc, char** argv){
    if (argc != 2){
	cout << "./bin/getkmeans <filenamelist>" << endl;
	return 0;
    }
    auto file_path = ReadAllLinesFromFile(argv[1]);

    cout << file_path.size() << endl;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < file_path.size(); i++){
	int count = 0;
	string str = "sed -n '$=' " + string(file_path[i]);
	FILE *pp = popen(str.c_str(), "r");
	char tmp[10];
	fgets(tmp, sizeof(tmp), pp);
	count = atof(tmp);
	count = count / 1200 + 1;
	stringstream s, s_c;
	s << i+1;
	s_c << count;

	str = "mkdir ./data/kmean/" + s.str();
	system (str.c_str());
        string str1 = "./bin/kmeans " + file_path[i] + " " + s_c.str() + " ./data/kmean/" + s.str() + "/";
 	cout << str1.c_str() << endl;
        system(str1.c_str());
    }

    return 0;
}

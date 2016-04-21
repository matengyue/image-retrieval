/*************************************************************************
	> File Name: main_.cpp
    > Abstract: index词袋模型
    > Author: matengyue
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年04月5日 星期二 13时11分41秒

    当前版本号：1.0
    取代版本号：1.0
 ************************************************************************/
#include "BoWBuilder.h"

using namespace std;

void readme() {
    cout << "Usage: ./index <infile>" << endl;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        readme();
        return -1;
    }

    auto imgfns = ReadAllLinesFromFile(argv[1]);
    BoWBuilder bowb;
    vector<Mat> features;
    BoWCollection bows;
    auto dict = bowb.BuildCodebookAndQuantize(imgfns, features, bows);

    //存储词袋数据
    bowb.WriteCodebook(dict);

    //写入到文件
    ofstream ofs("bows.dat", ios::binary);
    if (!ofs) { 
        throw runtime_error("Cannot open file."); 
    }
    bows.Serialize(ofs);
    ofs.close();

    return 0;
}

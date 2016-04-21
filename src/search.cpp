/*************************************************************************
	> File Name: search1.cpp
    > Abstract: 检索匹配
    > Author: matengyue
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年03月29日 星期二 13时24分29秒

    当前版本号：1.0
    取代版本号：1.0
 ************************************************************************/

#include "grey.h"
#include <algorithm>
#include "BoWBuilder.h"

using namespace std;

void readme() {
    cout << "Usage: ./search <imgNameFile> <simImgFilePath>" << endl;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        readme();
        return -1;
    }

    //Time
    bool isTxt = (argc == 3);

    //BoWBuilder bowbuilder;
    // 文件读取
    //auto dict = bowbuilder.ReadCodebook();
    // 特征提取&量化
    //auto bow = bowbuilder.Quantize(dict, argv[1]);
    // 词袋读取
    ifstream ifs("bows.dat", ios::binary);
    if (!ifs) { 
        throw runtime_error("Cannot open file."); 
    }
    BoWCollection bows = BoWCollection::Deserialize(ifs);
    ifs.close();
    
   // int j = 0;
    // 匹配

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int j = 0; j < bows.bows.size(); j++) {
        string filename = argv[2];
        vector<pair<int, float>> dists;
        //---------------------------------------
        //char name[10];
        //sprintf(name, "%d", j);
        //filename = filename + string(name) + string(".txt");
        filename = filename + string("temp.txt");
        cout << filename.c_str() << endl;
        ofstream ofs(filename.c_str(), ios::app);
        if (!ofs)
            throw runtime_error("Cannot open file.");
        //---------------------------------------
        for (int i = 0; i < bows.bows.size(); i++) {
            //直方正交距离
            //float dist = Sum(Zip<float, float>(bows.bows[i].bow, bows.bows[i].bow, [](float f1, float f2) {return min (f1, f2);}));
            
            // 余弦相似性距离
            float dist = std::inner_product(bows.bows[i].bow.begin(), bows.bows[i].bow.end(), bows.bows[j].bow.begin(), 0.0);
            //cout << dist << " " << i << endl;
            
            dists.push_back(pair<int, float>(i, dist));
        }
        
        // 排序输出
        sort(dists, [](const pair<int, float> &p1, const pair<int, float> &p2) { return p1.second > p2.second; });
        // for (auto p : dists) { 
        //     cout << p.second << endl; 
        // }
        
        if (isTxt) {
            auto files = ReadAllLinesFromFile(argv[1]);
       
            const char* arg = string(files[dists[0].first]).c_str();
            IplImage *pSrcImage = cvLoadImage(arg, CV_LOAD_IMAGE_UNCHANGED);
            IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);

            cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
            
            CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);
            cout << "start" << endl;
            for (int i = 0; i < 200; i++){
	            //if (dists[i].second <= 0) 
                if (dists[i].second >= 0.0002) {
                    const char* arg = string(files[dists[i].first]).c_str();
                    if (grey(arg, pcvHistogram) >= 0.9) {
                        ofs << files[dists[i].first] << "\t" << j+1 << endl;
                        cout << files[dists[i].first] << endl;
                        cout << dists[i].second << endl;
                    }
                }
                else{
                    break;
                }
            }
            cout << "----------------------------------" << endl;
            cvReleaseHist(&pcvHistogram);
        }
    }
    
    return 0;
}

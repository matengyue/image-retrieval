/*************************************************************************
	> File Name: ys.cpp
	> Author: lumujin
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年03月10日 星期四 15时11分53秒
 ************************************************************************/

#include <algorithm>
#include "BoWBuilder.h"
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;

struct Tuple{
    float attr[10] = {0};
    string name_;
};

void readme(){
    cout << "./hsv <filename> > <outfile>" << endl;
}

Tuple getHSV(string jpg){
  //  cout << jpg.c_str() << endl;
    IplImage * src= cvLoadImage(jpg.c_str());
        
    IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
    IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
    IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
    IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);
    IplImage* planes[] = {h_plane, s_plane};
        
    /** H 分量划分为3个等级，S分量划分为3个等级 */
    int h_bins = 3, s_bins = 3;
    int hist_size[] = {h_bins, s_bins};
        
    /** H 分量的变化范围 */
    float h_ranges[] = {0, 180}; 
        
    /** S 分量的变化范围*/
    float s_ranges[] = {0, 255};
    float* ranges[] = {h_ranges, s_ranges};
        
    /** 输入图像转换到HSV颜色空间 */
    cvCvtColor(src, hsv, CV_BGR2HSV);
    cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);
        
    /** 创建直方图，二维, 每个维度上均分 */
    CvHistogram * hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1);
    /** 根据H,S两个平面数据统计直方图 */
    cvCalcHist(planes, hist, 0, 0);
        
    //------------------------------
    Tuple tuple;
    tuple.name_ = jpg;
    for(int h = 0; h < h_bins; h++) {
        for(int s = 0; s < s_bins; s++) {
            int i = h*s_bins + s;
            /** 获得直方图中的统计次数，计算显示在图像中的高度 */
            float bin_val = cvQueryHistValue_2D(hist, h, s);
            //cout << bin_val/100 << " ";
            tuple.attr[i] = bin_val/100;
        }
    }
  //  cout << endl;
        
    cvReleaseImage(&src);
    cvReleaseImage(&hsv);
    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);
    cvReleaseHist(&hist);

    return tuple;
}

int main(int argc, char** argv)
{
    if (argc < 2){
        readme();
        return -1;
    }

    auto file_path = ReadAllLinesFromFile(argv[1]);
    //cout << file_path.size() << endl;

    vector<Tuple> tuples;

    for (int x = 0; x < file_path.size(); x++){
    	Tuple tuple;
	string name = file_path[x];
	try {
	    tuple = getHSV(name);	
	} catch (cv::Exception& e){
//	    cout << "----------Not jpg.----------" << endl;
	    continue;
	}
        tuples.push_back(tuple); 
        //--------------------------
	cout << tuple.attr[0] << " ";
        cout << tuple.attr[1] << " ";
        cout << tuple.attr[2] << " ";
        cout << tuple.attr[3] << " ";
        cout << tuple.attr[4] << " ";
        cout << tuple.attr[5] << " ";
        cout << tuple.attr[6] << " ";
        cout << tuple.attr[7] << " ";
        cout << tuple.attr[8] << " ";
	cout << tuple.name_ << endl;
//	cout << file_path[x].c_str() << endl; 
    }

    return 0;
}


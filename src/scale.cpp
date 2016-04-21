/*************************************************************************
    > File Name: scale.cpp
    > Abstract: 方缩图像
    > Author: matengyue
    > Mail: lumujin@icloud.com
    > Created Time: 2016年03月28日 星期一 10时03分00秒

    当前版本号：1.0
    取代版本号：1.0
 ************************************************************************/

#include <algorithm>
#include <opencv2/opencv.hpp>
#include "BoWBuilder.h"

using namespace std;
using namespace cv;

void readme() {
    cout << "Usage: ./scale <infile> <size> <path>" << endl;
}

int main(int argc, char ** argv)
{
    if (argc != 4) {
        readme();
        return -1;
    }

    //目标图像尺寸
    int size = atof(argv[2]);
    CvSize czSize;
    czSize.width = size;
    czSize.height = size;

    auto imgfns = ReadAllLinesFromFile(argv[1]);
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (int i = 0; i < imgfns.size(); i++){
        cout << imgfns[i] << endl;
	    const char *pstrImageName = imgfns[i].c_str();
	    string str = imgfns[i] + (string)"_s" + argv[2] + (string)".jpg";
	    const char *pstrSaveImageName = str.c_str();

	    //从文件中读取图像 
	    IplImage *pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_UNCHANGED);
	    IplImage *pDstImage = NULL;	
	
	    //创建图像并缩放
	    pDstImage = cvCreateImage(czSize, pSrcImage->depth, pSrcImage->nChannels);
	    cvResize(pSrcImage, pDstImage, CV_INTER_AREA);
	
	    //保存图片
	    cvSaveImage(pstrSaveImageName, pDstImage);
	
	    cvReleaseImage(&pSrcImage);
	    cvReleaseImage(&pDstImage);
        string sstr = (string)"mv " + imgfns[i] + "_s* " + argv[3]; 
        system(sstr.c_str());
    }

    return 0;
}

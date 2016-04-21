/*************************************************************************
	> File Name: grey.h
    > Abstract: 灰度匹配
	> Author: matengyue
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年04月6日 星期三 15时11分53秒

    当前版本号：1.0
    取代版本号：1.0
 ************************************************************************/

#ifndef GREY_H
#define GREY_H

#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.hpp>
#include <highgui.h>
#include <iostream>
using namespace std;
using namespace cv;

double grey(const char* argv, CvHistogram *pcvHistogram_x);

void FillWhite(IplImage *pImage)
{
	cvRectangle(pImage, cvPoint(0, 0), cvPoint(pImage->width, pImage->height), CV_RGB(255, 255, 255), CV_FILLED);
}
// 创建灰度图像的直方图
CvHistogram* CreateGrayImageHist(IplImage **ppImage)
{
	int nHistSize = 256;
	float fRange[] = {0, 255};  //灰度级的范围  
	float *pfRanges[] = {fRange};  
	CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
	cvCalcHist(ppImage, pcvHistogram);
	return pcvHistogram;
}

// 根据直方图创建直方图图像
double CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram, CvHistogram *pcvHistogram_x)
{
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);
	FillWhite(pHistImage);

	//统计直方图中的最大直方块
	float fMaxHistValue = 0;
	cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);

	//分别将每个直方块的值绘制到图中
	int i;
    double a = 0, b = 0, c = 0;
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(i = 0; i < nImageWidth; i++)
	{
		float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小
		//int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //要绘制的高度
        float fHistValue_x = cvQueryHistValue_1D(pcvHistogram_x, i);
        
        a += fHistValue * fHistValue;
        b += fHistValue_x * fHistValue_x;
        c += fHistValue * fHistValue_x;
	}

    double result = c / (sqrt(a) * sqrt(b));
    
    cvReleaseImage(&pHistImage);

    return result;
}

double grey(const char* argv, CvHistogram *pcvHistogram_x)
{	
	// 从文件中加载原图
	IplImage *pSrcImage = cvLoadImage(argv, CV_LOAD_IMAGE_UNCHANGED);
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);

	// 灰度图
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	
    // 灰度直方图
	CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);
	
	// 创建直方图图像
	int nHistImageWidth = 128;
	int nHistImageHeight = 150;  //直方图图像高度
	int nScale = 2;            
	double result = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram, pcvHistogram_x);

    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pGrayImage);
    cvReleaseHist(&pcvHistogram);

    return result;
}

#endif

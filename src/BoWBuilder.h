/*************************************************************************
	> File Name: BowBuilder.h
    > Abstract: 词袋构建
	> Author: matengyue
	> Mail: lumujin@icloud.com 
	> Created Time: 2016年03月28日 星期一 15时35分41秒

    当前版本号：1.0
    取代版本号：1.0
 ************************************************************************/

#ifndef __BOW_BUILDER__
#define __BOW_BUILDER__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "cppsugar/cppsugar"

//多线程计算
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace cv;
using namespace cv::flann;
using namespace func;
using namespace util;

//词袋词汇
struct BoW
{
    int id;
    vector<float> bow;
    BoW(): id(-1) {};
    BoW(int _id, const std::vector<float> &_bow): id(_id), bow(_bow) {};
    
    //序列化
    void Serialize(ofstream &ofs) const {
        ofs.write((char *)&id, sizeof(int));
        int size = bow.size();
        ofs.write((char *)&size, sizeof(int));
        ofs.write((char *)&bow[0], sizeof(float) * size);
    }

    //反序列化
    static BoW Deserialize(ifstream &ifs) {
        BoW bow;
        ifs.read((char *)&bow.id, sizeof(int));
        int size = 0;
        ifs.read((char *)&size, sizeof(int));
        bow.bow.resize(size);
        ifs.read((char *)&bow.bow[0], sizeof(float) * size);
        return bow;
    }
};

//词袋集合
struct BoWCollection
{
    vector<BoW> bows;
    BoWCollection() {}
    BoWCollection(int _size): bows(_size) {}

    //序列化
    void Serialize(ofstream &ofs) const {
        int size = bows.size();
        ofs.write((char *)&size, sizeof(int));
        for (const auto &bow : bows) {
            bow.Serialize(ofs);
        }
    }

    //反序列化
    static BoWCollection Deserialize(ifstream &ifs) {
        BoWCollection bows;
        int size = 0;
        ifs.read((char *)&size, sizeof(int));
        for (int i = 0; i < size; i++) {
            auto bow = BoW::Deserialize(ifs);
            bows.bows.push_back(bow);
        }
        return bows;
    }
};


//建立词袋模型
class BoWBuilder {
public:
    //词袋特征词汇个数
    //const int DICT_SIZE = 150000;
    const int DICT_SIZE = 10000;
    //Sift特征向量维度
    const int FEATURE_DIMENSION = 128;
    //Kmeans聚类迭代次数
    const int KMEANS_MAX_ITERATION = 20;

    CompositeIndexParams QuantizationIndex;

    BoWBuilder(void) {};
    ~BoWBuilder(void) {};

    //SIFT图像特征提取
    Mat ExtractSIFTFeature(const string &imgfn) const {
        auto img = imread(imgfn, true);    //imgfn: 文件名/文件路径
        //cout <<"img:-----------------------------------"<<endl;
        //cout << img << endl;
        //cout <<"img:-----------------------------------"<<endl;
        vector<KeyPoint> keypoints;
        //特征点检测
        SiftFeatureDetector detector;
        detector.detect(img, keypoints);
        //SIFT特征提取inti
        SiftDescriptorExtractor extractor;
        Mat descriptors;
        if (!keypoints.size()) {
            return Mat();
        }
        //特征提取计算
        extractor.compute(img, keypoints, descriptors);

        //sift转换为rootsift
            //行->特征数量
        for (int y = 0; y < descriptors.rows; y++) {
            //列->128维rootsift特征向量
            for (int x = 0; x < descriptors.cols; x++){
                descriptors.at<float>(y, x) = sqrt(descriptors.at<float>(y, x));
            }
        }
        
        //阀值：10^12
        float threshold = pow(10, 12);
        //规范化描述符
        for (int y = 0; y < descriptors.rows; y++) {
            //求平方和
            float sum = 0;
            for (int x = 0; x < descriptors.cols; x++) {
                sum += descriptors.at<float>(y, x) * descriptors.at<float>(y, x);
            }
            sum = sqrt(sum);
            sum = max(sum, threshold);
            if (sum)
                descriptors.row(y) /= sum;
        }
        //-------------------------------
        cerr << descriptors.rows << " features extracte success." << endl;
        return descriptors;
    }

    //从给定的图像中提取稀疏特征（sift）
    //返回值是特征向量
    //每个向量是图像的特征描述符
    vector<Mat> ExtractSIFTFeatures(const vector<string> &imgfns) const {
        vector<Mat> features(imgfns.size());
	    int cnt = 0;

#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (int i = 0; i < imgfns.size(); i++) {
            features[i] = ExtractSIFTFeature(imgfns[i]);
	        cnt += features[i].rows;
	        cout << cnt <<endl;
        }

        return features;
    }

    //从图像集合中筛选出稀疏特征形成词袋
    //词袋中包含K个词汇
    //返回值为特征向量
    //复杂近似量化也采用Kmeans的中间结果
    Mat BuildCodebookAndQuantize(const vector<string> &imgfns, vector<Mat> &features, BoWCollection &bows) const
    {
        auto c = clock();

        int k = BoWBuilder::DICT_SIZE;
        features = ExtractSIFTFeatures(imgfns);
        cerr << "Feature extraction complete." << endl;
        int totalNum = 0;
        for (int i = 0; i < features.size(); i++) {
            totalNum += features[i].rows;
        }

        //------------------------------------------------------
        k = totalNum / 50;
        //------------------------------------------------------
        // int totalNum = Sum<Mat, int>(features, [](const Mat &v) { return v.rows; });
        cerr << totalNum << " features extracted. " << k << " visual words to cluster, " << (float)(totalNum * 100 / k) / 100
        << " features per word in average." << endl;
        
        // 随机生成聚类中心
        cerr << "Initializing centers...";
        vector<int> centerIds(totalNum);
        for (int i = 0; i < totalNum; i++) centerIds[i] = i;
        random_shuffle(centerIds.begin(), centerIds.end());
        set<int> centerIdSet;
        for (int i = 0; i < k; i++) centerIdSet.insert(centerIds[i]);
        centerIds.clear();
        cerr << "done.\nConstruct data structure..." << endl;

        //构建数据结构
        //将选定的特征点复制到聚类中心
        Mat centersMat(k, BoWBuilder::FEATURE_DIMENSION, DataType<float>::type);
        int currentIdx = 0, centersIdx = 0;
        for (auto it = features.begin(); it != features.end(); it++) {
            for (int i = 0; i < it->rows; i++) {
                if (centerIdSet.find(currentIdx) != centerIdSet.end()) {
                    memcpy((char *)(centersMat.datastart + centersIdx * centersMat.step), it->datastart + i * it->step, sizeof(float) * FEATURE_DIMENSION);
                    centersIdx++;
                }
                currentIdx++;
            }
        }

        // 迭代计算
        cerr << "Actual iteration..." << endl;
        cerr << "Updating the cluster centers..." << endl;
        
        vector<int> labels(totalNum);
        Mat centersSum(k, FEATURE_DIMENSION, DataType<float>::type);
        
        //多线程计算
        vector<pair<int, int>> idx;
        idx.reserve(totalNum);
        
        for(int i = 0; i < features.size(); i++)
        {
            for (int j = 0; j < features[i].rows; j++) {
                idx.push_back(pair<int, int>(i, j));
            }
        }

        for (int iter = 0; iter < KMEANS_MAX_ITERATION; iter++)
        {
            // 建立索引
            Index index(centersMat, KDTreeIndexParams());
            // 分配聚类中心
            vector<int> labelCount(k);
            memset(centersSum.datastart, 0, centersSum.dataend - centersSum.datastart);
#ifdef _OPENMP
#pragma omp parallel for
#endif
            for (int id = 0; id < idx.size(); id++) {
                int i = idx[id].first;
                int j = idx[id].second;
                Mat nn(1, 1, DataType<int>::type);
                Mat dist(1, 1, DataType<float>::type);
                //k邻近算法计算
                index.knnSearch(features[i].row(j), nn, dist, 1, SearchParams());
                int label = nn.at<int>(0, 0);
                
                centersSum.row(label) += features[i].row(j);
                labelCount[label]++;
            }
            for (int i = 0; i < k; i++)
                if (labelCount[i])
                    centersSum.row(i) /= labelCount[i];
            //迭代计算更新聚类中心
            centersMat = centersSum.clone();
            cerr << "Iterative calculate ";
            cerr << iter+1;
            cerr <<" times" << endl;
        }
        
        //迭代计算完毕
        cerr << "Iterative calculation is ";
        //时间计算
#ifdef __linux__
        cerr << "done. " << (clock() - c) / 1000000 << "sec." << endl;
#else
        cerr << "done. " << (clock() - c) / 1000 << "sec." << endl;
#endif

        //量化处理
        cerr << "Quantization... " << endl;
        Index index(centersMat, KDTreeIndexParams());
        bows = BoWCollection();
        bows.bows.resize(features.size());
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for(int i = 0; i < features.size(); i++)
        {
            if (!features[i].rows) {
                bows.bows[i] = BoW(i, vector<float>(k));
                continue;
            }
            
            Mat nn(features[i].rows, 1, DataType<int>::type);
            Mat dist(features[i].rows, 1, DataType<float>::type);
            index.knnSearch(features[i], nn, dist, 1, SearchParams());
            vector<float> bow(k);
            
            for (int j = 0; j < features[i].rows; j++) {
                bow[(int)nn.at<int>(j, 0)]++;
            }
            // L1 归一化处理
            float bowSum = Sum(bow);
            for (auto &b : bow) { b /= bowSum; }
            bows.bows[i] = BoW(i, bow);

            // L2 归一化处理
            //float accum = sqrt(std::inner_product(bows.bows[i].bow.begin(), bows.bows[i].bow.end(), bows.bows[i].bow.begin(), 0.0));
            //for (auto &b : bows.bows[i].bow) { 
                //b /= accum; 
            //}
        }

        return centersMat;
    }

    //读取特征词袋信息
    Mat ReadCodebook() const {
        vector<float> buff;
        ifstream ifs("dict.dat", ios::binary);
        if (!ifs) { throw runtime_error("Cannot open file."); };
        int w = 0, h = 0;
        ifs.read((char *)&h, sizeof(int));
        ifs.read((char *)&w, sizeof(int));
        buff.resize(w * h);
        ifs.read((char *)&buff[0], sizeof(float) * buff.size());
        ifs.close();
        
        //行优先
        return Mat(buff, true).reshape(1, h);
    }

    //写入特征词袋信息
    void WriteCodebook(const Mat &dict) const {
        ofstream ofs("dict.dat", ios::binary);
        if (!ofs) { throw runtime_error("Cannot open file."); }
        ofs.write((char *)&dict.rows, sizeof(int));
        ofs.write((char *)&dict.cols, sizeof(int));
        ofs.write((char *)dict.data, dict.dataend - dict.data);
        ofs.close();
    }

    // 从给定图像兴趣区域提取特征点, 返回量化结果。
    vector<float> Quantize(const Mat &dict, string imgfn) const {
        auto feature = ExtractSIFTFeature(imgfn);
        Index index(dict, KDTreeIndexParams());
        Mat nn(feature.rows, 1, DataType<int>::type);
        Mat dist(feature.rows, 1, DataType<float>::type);
        index.knnSearch(feature, nn, dist, 1, SearchParams());
        vector<float> bow(dict.rows);

        for (int j = 0; j < feature.rows; j++) {
            bow[nn.at<int>(j, 0)]++;
        }
        //L1 归一化处理
        float bowSum = Sum(bow);
        for (auto &b : bow) { b /= bowSum; }

        //float accum = sqrt(std::inner_product(bow.begin(), bow.end(), bow.begin(), 0.0));
        //for (auto &b : bow) { 
            //b /= accum; 
        //}

        return bow;
    }
};

#endif

/*************************************************************************
    > File Name: Kmeans.cpp
    > Abstract: Kmeans聚类
    > Author: matengyue
    > Mail: lumujin@icloud.com
    > Created Time: 2016年03月31日 星期四 10时11分32秒

    当前版本号：1.0
    取代版本号：1.0
 ************************************************************************/
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;
//存放元组的属性信息

struct Tuple{
	float attr1;
	float attr2;
    float attr3;
    float attr4;
	float attr5;
    float attr6;
    float attr7;
	float attr8;
    float attr9;
    string name_;
};

void readme(){
    cout << "./kmeans <file> <k> <path>" << endl;
}

//计算两个元组间的欧几里距离
float getDistXY(Tuple t1, Tuple t2) 
{
	return sqrt((t1.attr1 - t2.attr1) * (t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2) * (t1.attr2 - t2.attr2) + (t1.attr3 - t2.attr3) * (t1.attr3 - t2.attr3) + (t1.attr4 - t2.attr4) * (t1.attr4 - t2.attr4) + (t1.attr5 - t2.attr5) * (t1.attr5 - t2.attr5) + (t1.attr6 - t2.attr6) * (t1.attr6 - t2.attr6) + (t1.attr7 - t2.attr7) * (t1.attr7 - t2.attr7) + (t1.attr8 - t2.attr8) * (t1.attr8 - t2.attr8) + (t1.attr9- t2.attr9) * (t1.attr9 - t2.attr9));
}

//根据质心，决定当前元组属于哪个簇
int clusterOfTuple(Tuple means[], Tuple tuple, int k){
	float dist = getDistXY(means[0], tuple);
	float tmp;
	int label = 0;//标示属于哪一个簇

	for(int i = 1; i < k; i++){
		tmp = getDistXY(means[i], tuple);
		if(tmp < dist) {
            dist = tmp;
            label = i;
        }
	}

	return label;	
}

//获得给定簇集的平方误差
float getVar(vector<Tuple> clusters[], Tuple means[], int k){
	float var = 0;

	for (int i = 0; i < k; i++) {
		vector<Tuple> t = clusters[i];

		for (int j = 0; j< t.size(); j++) {
			var += getDistXY(t[j], means[i]);
		}
	}
	//cout<<"sum:"<<sum<<endl;
	return var;
}

//获得当前簇的均值（质心）
Tuple getMeans(vector<Tuple> cluster){
	
	int num = cluster.size();
	double meansX_x = 0, meansY_x = 0, meansZ_x = 0;
    double meansX_y = 0, meansY_y = 0, meansZ_y = 0;
    double meansX_z = 0, meansY_z = 0, meansZ_z = 0;
	Tuple t;

    for (int i = 0; i < num; i++) {
	    meansX_x += cluster[i].attr1;
	    meansY_x += cluster[i].attr2;
        meansZ_x += cluster[i].attr3;
	    meansX_y += cluster[i].attr4;
	    meansY_y += cluster[i].attr5;
        meansZ_y += cluster[i].attr6;
	    meansX_z += cluster[i].attr7;
	    meansY_z += cluster[i].attr8;
        meansZ_z += cluster[i].attr9;
	}

	t.attr1 = meansX_x / num;
	t.attr2 = meansY_x / num;
    t.attr3 = meansZ_x / num;
	t.attr4 = meansX_y / num;
	t.attr5 = meansY_y / num;
    t.attr6 = meansZ_y / num;
	t.attr7 = meansX_z / num;
	t.attr8 = meansY_z / num;
    t.attr9 = meansZ_z / num;
	
    return t;
	//cout<<"sum:"<<sum<<endl;
}

void KMeans(vector<Tuple> tuples, int k, char *path){
	vector<Tuple> clusters[k];
	Tuple means[k];
	int i=0;

    //默认一开始将前K个元组的值作为k个簇的质心（均值）
	for(i = 0; i < k; i++){
	    means[i].attr1 = tuples[i].attr1;
	    means[i].attr2 = tuples[i].attr2;
        means[i].attr3 = tuples[i].attr3;
	    means[i].attr4 = tuples[i].attr4;
	    means[i].attr5 = tuples[i].attr5;
        means[i].attr6 = tuples[i].attr6;
	    means[i].attr7 = tuples[i].attr7;
	    means[i].attr8 = tuples[i].attr8;
        means[i].attr9 = tuples[i].attr9;
	}

	int lable=0;
	
    //根据默认的质心给簇赋值
	for(i = 0; i < tuples.size() - 1; ++i){
		lable = clusterOfTuple(means,tuples[i], k);
		clusters[lable/2].push_back(tuples[i]);
	}
	
    float oldVar =  -1;
	float newVar = getVar(clusters, means, k);

    int m = 30;
    for (int x = 0; x < m; x++){//当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止
		for (i = 0; i < k; i++){ //更新每个簇的中心点
			means[i] = getMeans(clusters[i]);
			//cout<<"means["<<i<<"]:"<<means[i].attr1<<"  "<<means[i].attr2<<endl;
		}

		oldVar = newVar;
		newVar = getVar(clusters,means, k); //计算新的准则函数值

		for (i = 0; i < k; i++){ //清空每个簇
			clusters[i].clear();
		}

		//根据新的质心获得新的簇
		for(i = 0; i + 1 < tuples.size(); ++i){
			lable = clusterOfTuple(means,tuples[i], k);
    			clusters[lable].push_back(tuples[i]);
        	}
		cout << "iteration." << endl;
    }

	//输出簇
    int j = 1;
	for(lable = 0; lable < (k+1)/2; lable++, j++){
		vector<Tuple> t = clusters[lable];
        //cout << t.size() << endl;
        if (t.size() == 0){
            j--;
            continue;
        }
        //-------------------
        char str[10];
        sprintf(str, "%d", j);
        string str_ = string(path) + string(str) + "_jpg.txt";
        
        ofstream outfile(str_.c_str());
        //-----------------
        for (i = 0; i < t.size(); i++) {
            //cout << t[i].name_.c_str() << endl;
            outfile << t[i].name_.c_str() << endl;
		}
        system("printf '\n \033[33m[Message]\033[36m New Category write Success under Linux done. By Tengyue Ma.\033[39m \n\n'");
	}
}

int main(int argc, char **argv){
    if (argc != 4){
        readme();
    }

	ifstream infile;
	infile.open(argv[1], ios::in);

    if(!infile){
		cout<< "can not open the file." << endl;
		return 0;
	}
    
	vector<Tuple> tuples;
	Tuple tuple;

    //从文件流中读入数据
	while(!infile.eof()){
        infile >> tuple.attr1;
        infile >> tuple.attr2;
	    infile >> tuple.attr3;
	    infile >> tuple.attr4;
	    infile >> tuple.attr5;
	    infile >> tuple.attr6;
	    infile >> tuple.attr7;
	    infile >> tuple.attr8;
	    infile >> tuple.attr9;
            infile >> tuple.name_;
	    tuples.push_back(tuple);
	}

cout << "Read succeed." << endl;
    KMeans(tuples, atof(argv[2]) * 2, argv[3]);
	return 0;
}

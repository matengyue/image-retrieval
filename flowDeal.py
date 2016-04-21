#!/usr/bin/env python
# coding=utf-8

import sys
import os
import datetime

starttime = datetime.datetime.now()

Str1 = "python ./py-box/getPath2Div.py -p " + sys.argv[1] + " -out ./data/imgNameListFile/"
Str2 = "python ./py-box/getImgNameList.py -p ./data/imgNameListFile/ -t ./data/fileList.txt"
Str3 = "./bin/gethsv ./data/fileList.txt"
Str4 = "python ./py-box/getHsvTotal.py -p ./data/imgNameListFile/ -t ./data/imageHsvTotal_9.txt"
Str5 = "./bin/kmeans_y ./data/imageHsvTotal_9.txt 30 ./data/kmean_y/"
Str6 = "python ./py-box/getImgNameList.py -p ./data/kmean_y/ -t ./data/kmeansFileList.txt"
Str7 = "./bin/getkmeans ./data/kmeansFileList.txt"

print Str1
os.system(Str1)
print Str2
os.system(Str2)
print Str3
os.system(Str3)
print Str4
os.system(Str4)
print Str5
os.system(Str5)
print Str6
os.system(Str6)
print Str7
os.system(Str7)

images_names = os.listdir("./data/kmean/")

i = 1
for ind, str_each_file in enumerate(images_names):
    full_path = ''.join(["./data/kmean/", str_each_file, '/'])
    imagefile_names = os.listdir(full_path)
    for x, image_each_name in enumerate(imagefile_names):
    	image_kmeans_file_path = ''.join([full_path, image_each_name])
    	Str = "./bin/index " + image_kmeans_file_path
   	print Str
    	os.system(Str)
    	Str = "./bin/search " + image_kmeans_file_path + " ./data/simFile/"
    	print Str
    	os.system(Str)
	#    Str = "mv dict.dat Dict/dict" + str(i) + ".dat"
	#    os.system(Str)
	#    Str = "mv bows.dat Bows/bows" + str(i) + ".dat"
	#    os.system(Str)
    	Str = "mv ./data/simFile/temp.txt ./data/simFile/" + str(i) + ".txt"
    	os.system(Str)
    	i += 1

endtime = datetime.datetime.now()
print (endtime - starttime).seconds

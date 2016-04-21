# -*- coding: utf-8 -*-
#!/usr/bin/env python
# coding=utf-8

import os
import argparse as ap

#获取数据集合路径
parser = ap.ArgumentParser()
parser.add_argument("-p", "--imagesSet", help="Path to images Set", required="True")
parser.add_argument("-t", "--imagesFile", help="Path to imagesFile path", required="True")
args = vars(parser.parse_args())

#获取数据列表并存储到文件
imagesDir = args["imagesSet"]
imagesFile = args["imagesFile"]

images_names_dir = os.listdir(imagesDir)
images_names_dir = sorted(images_names_dir)

f = open(imagesFile, "w")
for ind, str_each_file in enumerate(images_names_dir):
    full_path = ''.join([imagesDir, str_each_file])
    full_path = ''.join([full_path, '/'])
    images_names = os.listdir(full_path)
    images_names = sorted(images_names)
    for i, image_each_name in enumerate(images_names):
        image_full_path = ''.join([full_path, image_each_name])
        f.writelines('%s\n' % image_full_path)
    print 'write %s' % (full_path)
f.close

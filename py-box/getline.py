#!/usr/bin/env python
# coding=utf-8

import sys
import os

images_names = os.listdir(sys.argv[1])

for ind, str_each_file in enumerate(images_names):
    image_kmeans_file_path = ''.join([sys.argv[1], str_each_file])
    Str = "wc -lcw " + image_kmeans_file_path
    print Str
    os.system(Str)

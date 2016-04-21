#!/usr/bin/env python
# coding=utf-8

import os
import sys

images_names = os.listdir(sys.argv[1])

for ind, str_each_file in enumerate(images_names):
    print str_each_file

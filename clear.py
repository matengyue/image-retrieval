#!/usr/bin/env python
# coding=utf-8

import sys
import os

Str1 = "rm ./data/*.txt"
Str2 = "rm -rf ./data/kmean/*"
Str3 = "rm ./data/kmean_y/*"
Str4 = "rm ./data/simFile/*"
Str5 = "rm ./*dat"

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

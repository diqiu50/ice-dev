#!/usr/bin/python
#-*-coding:utf-8-*- 
import os

cmd = "ping sa.uudaili.info -c 5"
os.system(cmd);
print ""

i=0;
while i<20:
    cmd = "ping sa" + str(i) + ".uudaili.info -c 5"
    os.system(cmd);
    print 
    i = i+1

#!/usr/bin/python
#-*-coding:utf-8-*- 
import virtkey 
import time

v = virtkey.virtkey()
v.press_keysym(65507) #Ctrl键位
v.press_unicode(ord('a')) #模拟字母V
v.release_unicode(ord('a'))
v.release_keysym(65507)
time.sleep(5)
v.press_keysym(65421) #Enter
v.release_keysym(65421)

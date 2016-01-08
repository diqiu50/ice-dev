#coding=utf-8
import pymysql
import sys
import os

if len(sys.argv) >=2:
    dir = sys.argv[1]
else:
    print("missing args!");
    exit();

conn=conn = pymysql.connect(host='192.168.99.184',user='root',passwd='12345',db='ice_db',port=3306)
cur=conn.cursor()
cur.execute('select * from res_t')
for r in cur.fetchall():
    filename = dir + r[0];
    print(filename);
    dirname = os.path.split(filename)[0];
    if os.path.exists(dirname) == False:
            os.makedirs(dirname)
    open(filename, "wb").write(r[3]);
cur.close()
conn.close()

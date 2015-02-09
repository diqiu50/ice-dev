#!/bin/bash

pwd=~/Backup/code
cd $pwd
#echo $pwd

find ~/AOS/src -name "[^.]*.[ch]*" -mmin -15>$USER.bl
dirprifix=`date +%Y%m%d`

while read line
do
	dirname=${line#*src/};
	mkdir -p src_$dirprifix/${dirname%/*};
	dirname=src_$dirprifix/${dirname%/*};
	cp $line $dirname/${line##*/};
done <$USER.bl 

rm $USER.bl

tardir=src_`date +%Y%m%d -d "-1 day"`
#echo $tardir

if [ -d $tardir ]; then
	tar -zcvf ${tardir}.tar.gz ${tardir} 1>/dev/null
	rm -fr $tardir
fi

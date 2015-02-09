#! /bin/sh

if [ "$1" = "" ]; then
	echo "missing args!"
	exit 1
fi

pid=`ps ax -o comm,pid |grep $1|awk '{print $2}'`

if [ "$pid" = "" ]; then
	echo "can not find the process!"
	exit 1
fi

status=`ps -o spid,%cpu -L -p $pid --no-headers`
pidarry=``

while true
do
	status=`ps -o spid,%cpu -L -p $pid --no-headers`
	echo $status
	exit 0
done

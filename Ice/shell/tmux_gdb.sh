#!/bin/bash 
# Barry.Niu


# a script to attach gdb over jimodb.


user=`whoami`

starttmux() { 
     #if [ -z "$Exes" ]; then 
     #   echo -n "Please provide of list of Exes separated by spaces [ENTER]: " 
     #   read Exes 
     #fi 

     #local Exes=( $Exes ) 
	 mkdir -p /tmp/${user}
     for i in "${Exes[@]}"
	 do 
		 num=`pgrep -u ${user} $i|wc -l`
		 for((n=1;n<=${num};n++))
		 do
			 pid=`pgrep -u ${user} $i|sed -n ${n}p`
			 source_txt="/tmp/${user}/source_"${i}"_"${n}".txt"
			 echo "attach $pid" > $source_txt
			 echo "b alarm.c:50" >> $source_txt
			 echo "shell tmux select-layout tiled > /dev/null" >> $source_txt
			 echo "shell tmux select-pane -t 0" >> $source_txt
			 echo "c" >> $source_txt
			 tmux split-window -h  "gdb -x $source_txt" 
	 	done
     done 

 } 

# Exes=${Exes:=$*} 
Exes=(frontend.exe cube.exe task.exe)

starttmux 

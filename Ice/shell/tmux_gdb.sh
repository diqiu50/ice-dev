#!/bin/bash 
# ssh-multi 
# D.Kovalov 
# Based on http://linuxpixies.blogspot.jp/2011/06/tmux-copy-mode-and-how-to-control.html 


# a script to ssh multiple servers over multiple tmux panes 


user=`whoami`

starttmux() { 
     #if [ -z "$Exes" ]; then 
     #   echo -n "Please provide of list of Exes separated by spaces [ENTER]: " 
     #   read Exes 
     #fi 

     #local Exes=( $Exes ) 
     tmux new-window 
     #unset Exes[0]; 
     for i in "${Exes[@]}"
	 do 
		 num=`pgrep -u ${user} $i|wc -l`
		 for((n=1;n<=${num};n++))
		 do
			 pid=`pgrep -u ${user} $i|sed -n ${n}p`
			 source_txt="source_"${i}"_"${n}".txt"
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
#bash -x tmux.sh 192.168.99.26 192.168.99.27 192.168.99.28 192.168.99.29 192.168.99.30 192.168.99.31 192.168.99.32 192.168.99.33 192.168.99.34 192.168.99.35 192.168.99.36 192.168.99.37 192.168.99.38 192.168.99.39 192.168.99.40 192.168.99.41 

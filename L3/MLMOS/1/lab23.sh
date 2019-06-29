#!/bin/bash
countRun=0
timestamp="no"
first="first"
second="second"
third="third"
show_time() { 
	timestamp=$( date +"%y_%m_%d_%H_%M_%S")
}
while true
do
	show_time
	command="zip -r $timestamp.zip ."
	countRun=$countRun+1
	eval $command
	#printf "DATA: ${timestamp}\n"
	first=$second
	second=$third
	third=$timestamp
	if [[ $countRun -gt 4 ]]; then
		command="rm $first.zip"
		eval $command
	fi
	sleep 2
done
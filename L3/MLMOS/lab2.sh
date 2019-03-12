#!/bin/bash
target=$1
host=$(cut -d ',' -f 1 $target)
ip=$(cut -d ',' -f 2 $target)
web=$(cut -d ',' -f 3 $target)
backend=$(cut -d ',' -f 4 $target)
db=$(cut -d ',' -f 5 $target)
printf "Host: ${host}\n"
printf "Host: ${ip}\n"
printf "Host: ${web}\n"
printf "Host: ${backend}\n"
printf "Host: ${db}\n\n\n"


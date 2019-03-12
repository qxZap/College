#!/bin/bash
target=$1
awk -F"," '{printf("Host: %s\nIP: %s\nWeb: %s\nBackend: %s\nDatabase: %s\n",$1,$2,$3,$4,$5)}' $target
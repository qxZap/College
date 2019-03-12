#!/bin/bash
yum install git
yum install centos-release-scl
ln -s /opt/rh/rh-python36/root/usr/bin/python /usr/local/bin/python3
./conf.py conf.ini
#chmod +x conf.py
scl enable rh-python36 bash
yum install python3
py 
cd /root/MLMOSH1
#git pull
#git status
cd ..
cp -R -f MLMOSH1/* .
#bash bootstrap.sh
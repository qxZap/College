#!/bin/bash
#run update
#yum update
#sed -re 's/^(PasswordAuthentication)([[:space:]]+)yes/\1\2no/' -i.`date -I` /etc/ssh/sshd_config

echo "" > /root/config/sshkey.key 
echo "" > /root/config/sshkey.key.pub 
yes | ssh-keygen -b 4096 -t rsa -f /root/config/sshkey.key -q -N 'cuteXD' > /dev/null &> ~/config/tmp.err


#rsa_key="$(cat /root/config/sshkey.key)"
cat /root/config/sshkey.key > ~/.ssh/id_rsa
cat /root/config/sshkey.key.pub > ~/.ssh/id_rsa.pub


#command="rm -rf /root/config/sshkey.key"
echo "" > /root/config/sshkey.key 
echo "" > /root/config/sshkey.key.pub 

file_size=$(du -k ~/config/tmp.err | cut -f1)



if [ $file_size -eq "0" ]
then
	rm -rf ~/config/tmp.err
fi

#error handleing
if [ -f ~/config/tmp.err ]
then
	#command="touch ~/../var/log/system-boostrap.log"
	#eval $command
	#err=$(cat ~/config/tmp.err)
	curr_time=$(date +"%y_%m_%d_%H_%M_%S")
	curr_err=$(cat ~/config/tmp.err)
	printf "${curr_time}: ${curr_err}\n" >> ~/../var/log/system-boostrap.log
	#date +"%y_%m_%d_%H_%M_%S: " >> ~/../var/log/system-boostrap.log
	#cat ~/config/tmp.err >> ~/../var/log/system-boostrap.log
fi

#sed -re 's/^(SELINUX)(=)enforcing/\1\2disabled/' -i.`date -I` /etc/selinux/config
setenforce 0
#for reset: sed -re 's/^(SELINUX)(=)disabled/\1\2enforcing/' -i.`date -I` /etc/selinux/config


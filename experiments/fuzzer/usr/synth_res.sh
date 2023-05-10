#!/bin/bash

curr_folder=$PWD
folder_dat_info=0_cov
for FILE1 in ./$folder_dat_info/*;
do
	
	#getthe name
	echo $FILE1 > one.txt
        one=$(sed 's/.*\///' one.txt)
        name=${one%.*}
        rm one.txt
        echo $name



	cd /root;
	python2 /usr/local/bin/xencov_split $curr_folder/$folder_dat_info/$name.dat --output-dir=/;
	cd xen;
	rm -rf cov1.info cov2.info cov1_sort.info cov2_sort.info;
	geninfo -q . -o $name.info;
	mv $name.info $curr_folder/$folder_dat_info 
done

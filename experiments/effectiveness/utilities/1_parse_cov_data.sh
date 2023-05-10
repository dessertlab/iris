# Desc: this script generates parsed coverage data (.info)
# Where to execute: in the local machine where Xen patched is installed
# Usage example: ./1_parse_cov_data.sh /data/CPU_workload
# @param: path to the folder containing the workload data


#!/bin/bash

folder_wl_data=$1

# Create a mano e spostati gli opportuni file
# Da modificare il .c 
folder_raw_rec=$folder_wl_data/raw_coverage/cov_recorded
folder_raw_rep=$folder_wl_data/raw_coverage/cov_replayed

mkdir -p $folder_wl_data/parsed_coverage/cov_recorded
mkdir -p $folder_wl_data/parsed_coverage/cov_replayed

folder_parse_rec=$folder_wl_data/parsed_coverage/cov_recorded
folder_parse_rep=$folder_wl_data/parsed_coverage/cov_replayed

echo '0' > state_processing.txt
start=$(sed '1!d' state_processing.txt)
end=4999

for ((i=$start; i<=$end; i++))
do
	echo $i > './state_processing.txt'


	# Process recorded raw data (.dat)
	name="cov_record"$i
        echo $name
        cd /root;
        python2 /usr/local/bin/xencov_split $folder_raw_rec/$name.dat --output-dir=/;
        cd xen;
        geninfo -q . -o $name.info;
        mv $name.info $folder_parse_rec


	# Process replayed raw data (.dat)
	name="cov_replay"$i
	echo $name
	cd /root;
        python2 /usr/local/bin/xencov_split $folder_raw_rep/$name.dat --output-dir=/;
        cd xen;
        geninfo -q . -o $name.info;
        mv $name.info $folder_parse_rep
done


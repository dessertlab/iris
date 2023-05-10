#!/bin/bash

currdir=$PWD
workloadname=$1
mkdir $currdir/exp_results/${workloadname}_workload
mkdir $currdir/exp_results/${workloadname}_workload/recording
mkdir $currdir/exp_results/${workloadname}_workload/replaying
mkdir $currdir/exp_results/${workloadname}_workload/seeds


echo Copying the timestamps...

cp $currdir/timestamps_recording.txt $currdir/exp_results/${workloadname}_workload/recording
cp $currdir/timestamps_replaying.txt $currdir/exp_results/${workloadname}_workload/replaying


echo Copying the seeds...

cp $currdir/seeds_mon/* $currdir/exp_results/${workloadname}_workload/seeds

echo Copying the recording data...

cp $currdir/cov_mon/guest_workload_recording.dat $currdir/exp_results/${workloadname}_workload/recording


cd /root;
python2 /usr/local/bin/xencov_split $currdir/cov_mon/guest_workload_recording.dat  --output-dir=/;
cd xen;
rm -rf cov.info cov;
geninfo . -o cov.info;
cp cov.info  $currdir/exp_results/${workloadname}_workload/recording

genhtml cov.info -o $currdir/exp_results/${workloadname}_workload/recording;



echo Copying the replaying data...


cp $currdir/cov_inj/guest_workload_replaying.dat $currdir/exp_results/${workloadname}_workload/replaying

cd /root;
python2 /usr/local/bin/xencov_split $currdir/cov_inj/guest_workload_replaying.dat  --output-dir=/;
cd xen;
rm -rf cov.info cov;
geninfo . -o cov.info;
cp cov.info  $currdir/exp_results/${workloadname}_workload/replaying

genhtml cov.info -o $currdir/exp_results/${workloadname}_workload/replaying;



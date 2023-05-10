#!/bin/bash

curr_dir=$PWD

#variables
export PATH_G=/home/test/mutation_fuzzer_sequence

#definire lo state_test.txt (es. 0 a capo 10 per fare 11 test case)
#eseguire il programma ./run_exp login_seeds 0 
#0 è RAND_REG

#example "./run_exp login_seeds login 0"

seed_name=$1 #boot_seeds.txt $1
name= $2 #boot $2
mode= $3#1 #$3
dom_id=$4;
step= $5
end_seq=$6

file="./state_test.txt"
n=$(sed '1!d' state_test.txt)
n_tcs=$(sed '2!d' state_test.txt)
n_sequence=$(sed '3!d' state_test.txt)
#echo "Test case ${n}/${n_tcs}"
#step=50


while [ $n_sequence -lt $end_seq ]; do
n=0
n_tcs=1
sed -i "1s/.*/${n}/" $file
echo "running sequence ${n_sequence}" 

while [ $n -lt $n_tcs ]; do
    echo "Test case ${n}/$((n_tcs-1))"
   #xl destroy hvm_guest;
    pass_kvm=$log_host_pwd
    cd $PATH_G
   
    echo "Test case name ${name}-mode-${mode}-tc-${n}"
    tcname=${name}-mode-${mode}-n-${n_sequence}-tc-${n}

    ./main ${seed_name} "${tcname}" ${mode} ${dom_id} ${n_sequence}

    #tests the return value
    if [ $? -eq 0 ]
    then
    echo "No crashes or hangs"
    mv ./0_crashes_hangs/${tcname} ./0_testcases/
    elif [ $? -eq -1 ]
    then
    echo "GunFuzzer detected an hang"
    elif  [ $? -eq -2 ]
    then
    echo "Unexpected error"
    fi
    

    #getting the test case info
    file="./state_test.txt"
    n=$(sed '1!d' state_test.txt)
    n_tcs=$(sed '2!d' state_test.txt)
#    echo "Test case ${n}/$((n_tcs-1))"

    #updating the fuzzer test case state
    let "n=$n+1"
    sed -i "1s/.*/${n}/" $file


    #incrementing the dom_id
    dom_id=$((dom_id+1));
done
n_sequence=$((n_sequence+step))
sed -i "3s/.*/${n_sequence}/" $file

done

#!/bin/bash
curr_dir=$PWD
wk_name="boot"
mode=1
#step=10
n_of_tc=1

#while [ $n_sequence -lt 5000 ]; do
for n_sequence in {50..500..50}
do
#for n in  0 .. ${n_of_tc}
for n in {0..9}
do
#mkdir calc_cov
echo "test"
tcname_wk_seq=${wk_name}-mode-${mode}-n-${n_sequence}-tc-${n}-0.info
tcname_wk_seq_mut=${wk_name}-mode-${mode}-n-${n_sequence}-tc-${n}-1.info
#cp $curr_dir/0_cov/${tcname_wk_seq} calc_cov
#cp $curr_dir/0_cov/${tcname_wk_seq_mut} calc_cov
python2 cumulative_cov.py  $curr_dir/0_cov/${tcname_wk_seq} $curr_dir/0_cov/${tcname_wk_seq_mut} res-${wk_name}-mode-${mode}tc-${n}



done
done

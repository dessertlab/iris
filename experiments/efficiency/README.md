# Efficiency experiments

This directory contains scripts to get performance data for recording and replaying for target workloads. The generated files are ``timestamps_recording.txt`` and ``timestamps_replaying.txt`` which contain timestamps for each VM exit executed.

## user\_timing\_workload

This directory includes the user-space program that aims to record the guest execution saving the timestamps (in terms of CPU cycles) for each *VM exit* and a cumulative coverage of the entire recording.
At the same time, it allows replaying the recorded sequence getting, again, the timestamps and the cumulative coverage of the entire replay.

### Usage example

1. The user runs (e.g., resuming the guest snapshot) the guest execution

2. The user starts recoring via ``IRIS_cli``:
	``./IRIS_cli v NUM_EXIT DOM_ID``

3. The user replays the sequence via ``IRIS_cli``: 
	``./IRIS_cli m NUM_EXIT 1 DOM_ID``
	
4. The ``./gen_info_script.sh ${workload_name}`` command creates and gathers the data of the entire experiment (code coverage html, .dat, .info, .timestamps, and seeds) in the experiment folder.


## user\_timing\_boot

This directory still includes the user-space program ``usr_efficiency_boot.c`` but differs from the program in ``usr_efficiency_cpu_idle.c`` because it is specialized for the *OS BOOT* workload. 

It runs *i)* a guest VM, *ii)* waits for the boot has finished, and *iii)* it starts to record 5000 VM Exits.
The program allows replaying these VM exits re-running the same operations.

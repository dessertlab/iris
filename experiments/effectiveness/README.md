# Effectivenss experiments

## 1. user-space programs

This directory contains user-space programs that allow using the interfaces provided by the *IRIS* framework to retrieve raw data during the recording and replaying of target workloads. All the user-space programs include a ``Makefile`` to compile them and obtain the ``IRIS_cli`` executable.

### cov\_fit

This directory includes a user-space program to start both *CPU-bound* and *IDLE* workloads. The user needs to:

1. Start a guest HVM by using the `hvm_configuration.cfg` Xen configuration file: 

	```
	$ xl create hvm_configuration.cfg
	```

2. Connect to the VM through `vnc`

	```
	$ vncviewer @_IP_XEN_MACHINE:1
	```

3. Start the workload to record within the guest VM:

	- In the case of *CPU-bound* workload run:
	
		```
		$ stress-ng --cpu 8 --timeout 10000s
		```

	- In the case of *IDLE* workload, let the VM booting until the login screen is presented.

4. Run ``make`` to compile and obtain the ``IRIS_cli`` executable. From *DOM0* VM, starts recording (``v`` flag) and replaying (``m`` flag):

	```
	$ ./IRIS_cli v <num_vmexit_to_record> <test_vm_id>
	$ ./IRIS_cli m <num_vmexit_to_replay> <dummy_vm_id>
	```

Note that is recommended to reboot Xen once the recorded workload is completed. Further, while replaying you don't have to create the *dummy VM*, because the script handles the *dummy VM* creation.  After a Xen reboot, the <dummy_vm_id> to use will be '1'.


### cov\_fit\_boot

This directory includes a specialization of the above scripts for the *OS BOOT* workload. In particular, the ``usr_boot`` already runs the target VM both in recording and replaying cases.

Run ``make`` to compile and obtain the ``IRIS_cli`` executable. From *DOM0* VM, starts recording (``v`` flag) and replaying (``m`` flag):

This test creates che VM itself, so no need to create ("xl create ..."). 
```
mkdir cov;

$ ./IRIS_cli v <num_vmexit_to_record> <granularity> <test_vm_id> e.g. ./IRIS_cli v 200 1 1
$ ./IRIS_cli m <num_vmexit_to_replay> <dummy_vm_id>
```

### vmwrite\_fit

In this scenario, the user-space program is used to evaluate the effectiveness in terms of VMWRITEs performed during recorded workload. In particular, the target workload is *OS BOOT*. We exploit a VM snapshot (``guest_snapshot``) to start from a stable state for recording and replaying. Run ``make`` to compile and obtain the ``IRIS_cli`` executable. Then:

```
cp <some_guest_snapshot> $this_folder e.g. cp ../cov_fit_boot/guest_snap ./

# You should rename guest_snap in guest_snapshot
mv guest_snap guest_snapshot

$ ./IRIS_cli v <num_vmexit_to_record> <test_vm_id> e.g. ./IRIS_cli v 200 1
$ ./IRIS_cli m <num_vmexit_to_replay> <gran_monitoring> <dummy_vm_id>	e.g. ./IRIS_cli m 200 200 2
```

## 2. utilities

This directory includes a set of facilities to parse raw data and get intermediate or final results. In particular:

* ``1_parse_cov_data.sh``: generates parsed coverage data (.info). You have to run it on the local machine where Xen patched. Useful for cov_info and cov_info_boot. 
	- Usage example: ``./1_parse_cov_data.sh /home/test/effectiveness/usr/cov_fit``
	- output -> parsed_coverage
	- You need to specify hard-coded the number of the last cov_file (end)
*  ``2_cumulative_cov.py``: evaluates cumulative coverage processing parsed coverage data. 
	- Usage example: ``python2 2_cumulative_cov.py /home/test/effectiveness/usr/cov_fit``
	- output -> results_cumulative
	- count_cov_rec is the new coverage evaluated by the X record file
	- count_cumul_rec is the cumulative coverage evaluated until X record file
*   ``3_concat_seeds.py``: concats single VM seeds files contained in a folder
	- Usage in "seeds_folder"
	- Output -> seeds_cat file 
* ``4_process_single_cov.py``: evaluates hit differences (in terms of coverage) between two .info coverage files (e.g. record and replay files).
	- Usage example: ``python2 4_process_single_cov.py cov_recorded.info cov_replayed.info``
	- 4_distribution.py is similar, it requires two directories (e.g. record and replay files) as params. 
* ``print_exit_distribution/5_print_exit_distr.c``: prints the exit distribution given a VM seeds file
	- Usage: ``./print_exit_distr ../usr/cov_fit/seeds 200``
	- Results: seed # 	exit_reason_number 	exit_reason_name
*  ``print_vmwrite_diff``: directory that includes source codes to evaluate vmwrites fitting starting from two seeds



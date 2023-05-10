# IRIS: a Record and Replay Framework to Enable Hardware-assisted Virtualization Fuzzing

This repository contains the artifacts provided within paper #333 "*IRIS: a Record and Replay Framework to Enable Hardware-assisted Virtualization Fuzzing*" submitted @DSN2023. The repository includes:

* The *IRIS* framework is provided as a hypervisor patch. Currently, *IRIS* supports only Xen hypervisor;
* Raw data about our experiments, scripts to reproduce obtained results;
* Proof-of-concept fuzzer built upon *IRIS*.

### Repository structure

The diagram below provides the organization of the project:

```
|-- iris_xen_patch
|-- experiments
|   -- effectiveness
|        -- usr
|        -- data
|            -- CPU_workload
|            -- ...
|        -- utilities
|   -- efficiency
|        -- usr
|        -- data
|   -- fuzzer
|        -- usr
|        -- data
|-- figures

```

1. `iris_xen_patch` directory contains the *IRIS* patch to be applied to Xen and a README for instructions.

2. `experiments` directory contains scripts, data, and user programs related to the three experiments of the paper.
For each of the experiments, the `usr` directory includes user-space programs to allow using the interfaces provided by the *IRIS* framework to retrieve raw data. Each of the `usr` directories includes README documentation for usage.

	About **effectiveness** experiment (`effectiveness` directory), `utilities` includes facilities to parse raw data and get intermediate or final results. For the same experiment, we also provide a `data` directory for each workload (i.e., *OS BOOT*, *CPU-bound*, *IDLE*): 

	*  **raw\_coverage**: directory that includes GCOV coverage data (.dat) related to both the record and replay phases;
	*  **parsed\_coverage**: directory that includes parsed coverage (.info) through the `geninfo` tool related to both the record and replay phases;
	*  **${WORKLOAD_NAME}\_snap**: VM snapshots used as starting point for the experiments;
	*  **recorded\_seeds**: seeds recorded during the experiments in terms of the pairs of VMCS {field, value}, and GPR;

	About **efficiency** experiment (`efficiency` directory), we provide a `data` directory for each workload (i.e., *OS BOOT*, *CPU-bound*, *IDLE*), which includes: *i)* .csv file with timestamps obtained during recording target workload; *ii)* .csv file with timestamps obtained during replaying target workload; *iii)* recorded VM seed for target workload.

	About **fuzzer** experiment (`fuzzer` directory), we provide a `/data` directory for each workload: 

	*  **cov**: coverage discovered during the test cases
	*  **crashes\_hang**: any crashes or hangs identified during testcases
	*  **logs**:
	*  **testcases**: seeds generated through mutation


3. `figures` directory contains MATLAB sources and scripts to generate figures in the paper.

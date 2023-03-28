# IRIS: a Record and Replay Framework to Enable Hardware-assisted Virtualization Fuzzing

This repository contains the artifacts provided within paper "*IRIS: a Record and Replay Framework to Enable Hardware-assisted Virtualization Fuzzing*" accepted for pubblication at DSN 2023. 

Please, add the following item to your bibliography if you want to use ``IRIS`` for your research (note that, at this stage of publication, you can cite the arXiv preprint):

```
@article{cesarano2023iris,
  title={IRIS: a Record and Replay Framework to Enable Hardware-assisted Virtualization Fuzzing},
  author={Cesarano, Carmine and Cinque, Marcello and Cotroneo, Domenico and De Simone, Luigi and Farina, Giorgio},
  journal={arXiv preprint arXiv:2303.12817},
  year={2023}
}
```

The repository includes:

* The *IRIS* framework is provided as a hypervisor patch. Currently, *IRIS* supports only Xen hypervisor;
* Raw data about our experiments, scripts to reproduce obtained results;
* Proof-of-concept fuzzer built upon *IRIS*.

### Repository structure

The diagram below provides the organization of the project:

```
|-- iris_xen_patch
|-- experiments
|   -- accuracy
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

	About **accuracy** experiment (`accuracy` directory), `utilities` includes facilities to parse raw data and get intermediate or final results. For the same experiment, we also provide a `data` directory for each workload (i.e., *OS BOOT*, *CPU-bound*, *IDLE*): 

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

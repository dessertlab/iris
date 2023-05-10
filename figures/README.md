#Description

This directory includes all scripts and data to generate figures used in the paper. The figures are built using MATLAB.
Specifically, we used ``mlx`` files, which are MATLAB live scripts and the figures are shown once the user run them. In addition, the figures are automatically exported.

## Workload characterization figures

To obtain *Fig. 4: VM exit reasons distribution over time during OS BOOT workload.*, run ``boot_dist_over_time.mlx``

To obtain *Fig. 5: VM exit reasons distribution over OS BOOT, CPU-bound, and IDLE workloads.*, run ``workload_dist.mlx``


## Effectiveness figures

To obtain *Fig. 6: Cumulative code coverage across OS BOOT, CPU-bound, and IDLE workloads*, run ``effectiveness/boot.mlx``, ``effectiveness/cpu.mlx`` and ``effectiveness/idle.mlx``.

To obtain *Fig. 7: Code coverage differences by VM exit reason across targeted workloads*, run ``effectiveness/diff_analysis.mlx``

To obtain *Fig. 8: Operating modes and virtual CPU states across VM exits during OS BOOT workload* run ``effectiveness/cr0_modes.mlx``

## Efficiency figures

To obtain *Fig. 9: Performance in submitting VM seeds across OS BOOT, CPU-bound, and IDLE workloads.*, run ``efficiency/boot.mlx``, ``efficiency/cpu.mlx`` and ``efficiency/idle.mlx``.

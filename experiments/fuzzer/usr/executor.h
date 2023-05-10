#define EXECUTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <xenctrl.h>
#include <inttypes.h>
#include <sched.h>
#include <time.h>
#ifndef MUTATOR_H
#include "mutator.h"
#endif

#define BIOS_EXITS 15000
#define BUFFER_DIM_FACTOR 500

#define PRINT_MODE 0
#define INJECTION_MODE 1

#define VMCS_MONITORING_START                   0
#define VMCS_MONITORING_STOP                    1
#define VMCS_BOOT_MONITORING_SETUP              2
#define VMCS_BOOT_MONITORING_STOP               3
#define VMCS_MUTATION_ENABLE                    4
#define VMCS_MUTATION_DISABLE                   5
#define VMCS_BOOT_MUTATION_SETUP                6
#define VMCS_MUTATION_START_NEW_ITERATION       7
#define VMCS_DEBUG_MODE_ENABLE                  8
#define VMCS_DEBUG_MODE_DISABLE                 9
#define VMCS_NON_BLOCKING_MODE_ENABLE           10
#define VMCS_NON_BLOCKING_MODE_DISABLE          11
#define VMCS_BOOT_MONITORING_SET_EXIT_N         12
#define VMCS_BOOT_MONITORING_CHECK              13
#define VMCS_BOOT_MUTATION_DISABLE              14
#define VMCS_MUTATION_START_NEW_ITERATION_BLOCKING 15
#define VMCS_MUTATION_START_NEW_ITERATION_NO_BLOCKING 16
#define WAITING_FOR_NEW_MUTATION 17
/*
const char* type[3] = {
    [0]="WR",
    [1]="RD",
    [2]="REG"
};
*/

void prepare_to_fire(int dom_id,xc_interface *pxch);
int fire(int dom_id,xc_interface *pxch, seeds_t mutations);
void stop_to_fire(xc_interface *pxch);

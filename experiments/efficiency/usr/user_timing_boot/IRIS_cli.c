#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <xenctrl.h>
#include <inttypes.h>
#include <sched.h>
#include <time.h>

#define MON_THOLD 30
#define BUFFER_DIM_FACTOR 200
#define MAX_BIOS_EXIT 15000

#define PRINT_MODE 0
#define INJECTION_MODE 1

#define VMCS_MONITORING_START 			0
#define VMCS_MONITORING_STOP 			1
#define VMCS_BOOT_MONITORING_SETUP		2
#define VMCS_BOOT_MONITORING_STOP		3
#define VMCS_MUTATION_ENABLE 			4
#define VMCS_MUTATION_DISABLE 			5
#define VMCS_BOOT_MUTATION_SETUP		6
#define VMCS_MUTATION_START_NEW_ITERATION 	7
#define VMCS_DEBUG_MODE_ENABLE 			8
#define VMCS_DEBUG_MODE_DISABLE 		9
#define VMCS_NON_BLOCKING_MODE_ENABLE 		10
#define VMCS_NON_BLOCKING_MODE_DISABLE 		11
#define VMCS_BOOT_MONITORING_SET_EXIT_N		12
#define VMCS_BOOT_MONITORING_CHECK		13
#define VMCS_BOOT_MUTATION_DISABLE		14
#define VMCS_MUTATION_START_NEW_ITERATION_BLOCKING 15
#define VMCS_MUTATION_START_NEW_ITERATION_NO_BLOCKING 16
#define VMCS_BOOT_MUTATION_CHECK 17
#define VMCS_BOOT_MONITORING_SET_NUM_SEC 18
#define VMCS_TIMING_BOOT_MONITORING_START 19
#define VMCS_TIMING_BOOT_MONITORING_STOP 20
#define VMCS_TIMING_MUTATION_START 21
#define VMCS_TIMING_MUTATION_STOP 22




const char* type[3] = {
    [0]="WR",
    [1]="RD",
    [2]="REG"
};

const char* VMCS_AREAS[4] = {
	[0] = "CONTROL",
	[1] = "VM-EXIT-INFORMATION",
	[2] = "GUEST-STATE",
	[3] = "HOST-STATE"
};

const char* exit_reason_name[65] = {
    [0]="EXCEPTION_NMI",
    [1]="EXTERNAL_INTERRUPT",
    [2]="TRIPLE_FAULT",
    [3]="INIT",
    [4]="SIPI",
    [5]="IO_SMI",
    [6]="OTHER_SMI",
    [7]="PENDING_VIRT_INTR",
    [8]="PENDING_VIRT_NMI",
    [9]="TASK_SWITCH",
    [10]="CPUID",
    [11]="GETSEC",
    [12]="HLT",
    [13]="INVD",
    [14]="INVLPG",
    [15]="RDPMC",
    [16]="RDTSC",
    [17]="RSM",
    [18]="VMCALL",
    [19]="VMCLEAR",
    [20]="VMLAUNCH",
    [21]="VMPTRLD",
    [22]="VMPTRST",
    [23]="VMREAD",
    [24]="VMRESUME",
    [25]="VMWRITE",
    [26]="VMXOFF",
    [27]="VMXON",
    [28]="CR_ACCESS",
    [29]="DR_ACCESS",
    [30]="IO_INSTRUCTION",
    [31]="MSR_READ",
    [32]="MSR_WRITE",
    [33]="INVALID_GUEST_STATE",
    [34]="MSR_LOADING",
    [36]="MWAIT_INSTRUCTION",
    [37]="MONITOR_TRAP_FLAG",
    [39]="MONITOR_INSTRUCTION",
    [40]="PAUSE_INSTRUCTION",
    [41]="MCE_DURING_VMENTRY",
    [43]="TPR_BELOW_THRESHOLD",
    [44]="APIC_ACCESS",
    [45]="EOI_INDUCED",
    [46]="ACCESS_GDTR_OR_IDTR",
    [47]="ACCESS_LDTR_OR_TR",
    [48]="EPT_VIOLATION",
    [49]="EPT_MISCONFIG",
    [50]="INVEPT",
    [51]="RDTSCP",
    [52]="VMX_PREEMPTION_TIMER_EXPIRED",
    [53]="INVVPID",
    [54]="WBINVD",
    [55]="XSETBV",
    [56]="APIC_WRITE",
    [58]="INVPCID",
    [59]="VMFUNC",
    [62]="PML_FULL",
    [63]="XSAVES",
    [64]="XRSTORS"
};



struct bin_data {
    uint64_t  field;
    uint64_t  value;
    uint64_t  type;
};
typedef struct bin_data bin_data_t;

struct seed {
    unsigned long id;
    unsigned long size;
    bin_data_t* seed_items;
};

typedef struct seed seed_t;

struct seeds {
    unsigned long  size;
    seed_t* seeds_items ;
};

typedef struct seeds seeds_t;

int raw_to_seeds(int size, uint64_t*buffer, seeds_t* seeds, int mode);

int main(int argc, char* argv[])
{
/*
        struct sched_param params;
        int pid = getpid();
   	params.sched_priority=90;
        sched_setscheduler(pid,SCHED_FIFO,&params);
*/
	long res = 0;
	xc_interface *pxch = xc_interface_open(NULL, NULL, 0);


	switch(*argv[1]){

	case 'v':{
		unsigned int exit_step = 1;	 	// exit granularity
		unsigned int num_slot  = 15000;		// num of iterations
		
		int dom_id = atoi(argv[4]);

		FILE *fp;
		char* file_name = malloc(sizeof(char)*50);
		char* cmd = malloc(sizeof(char)*90);

		bool bios_end = false;
		bool first_iter = true;
		int boot_exit_count = 0;

		struct timespec tim, tim2;
		tim.tv_sec = 0;
		tim.tv_nsec = 1L;

		// Sets the granularity of monitoring (num_exit triggers the preempt loop)
		xc_vmcs_fuzzing(pxch, 0, VMCS_DEBUG_MODE_DISABLE, 0, NULL);
		xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MUTATION_DISABLE, 0, NULL);
		//xc_vmcs_fuzzing(pxch, 0, VMCS_TIMING_BOOT_MONITORING_START, 0, NULL);
		xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_SET_NUM_SEC, 100, NULL);
		
		// Executes all BIOS exits in monitoring mode without saving
		for (int i=0; i<num_slot; i++) {
			if(bios_end)xc_vmcs_fuzzing(pxch, 0, VMCS_TIMING_BOOT_MONITORING_START, 0, NULL);
			// Enables boot monitoring mode and allocate the output buffer
                        xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_SET_EXIT_N, exit_step, NULL);
			xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_SETUP, BUFFER_DIM_FACTOR * exit_step, NULL);

			// Creates the VM on the first iteration and gets its dom_id
			if (first_iter) {
				system("xl create hvm_configuration.cfg");
				first_iter = false;
			}

			// Waits for the end of monitoring
			res = 0;
			while (res != 1) {
				nanosleep(&tim, &tim2);
				res = xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_CHECK, 0, NULL);
			}

			// BOOT exits
			if (bios_end) {

				// Retrieves coverage
                                sprintf(cmd, "xencov read > ./cov_mon/boot_cov%d.dat", boot_exit_count);
                                system(cmd);
			}

			// Reads monitored buffer
			uint64_t *buffer = malloc(sizeof(uint64_t) * BUFFER_DIM_FACTOR * exit_step);
			memset(buffer, 0, sizeof(uint64_t) * BUFFER_DIM_FACTOR * exit_step);
			res = xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MONITORING_STOP, BUFFER_DIM_FACTOR * exit_step, buffer);

			// BOOT exits
			if (bios_end) {

				// Writes seeds to file
				sprintf(file_name, "./seeds_mon/boot_seed%d", boot_exit_count);
				if((fp = fopen(file_name, "w"))==NULL) return -1;
	    			for (int j=0; j<res; j++) fprintf(fp,"%"PRIx64"\n", buffer[j]);
				printf("BOOT exit: #%d\n", boot_exit_count);
				boot_exit_count++;
				fclose(fp);
			}
			else {
				printf("BIOS exit: #%d\n", i);
			}

			// Checks if bios is finished
			if (buffer[4]==0x6f5e && buffer[10]==0x10) {

				// We monitor and save num_slot exits after the BIOS
				i = 0; num_slot = atoi(argv[3])+1; exit_step=atoi(argv[2]);
				bios_end = true;

				// Save last bios vmexit
                        /*      sprintf(file_name, "./seeds_mon/last_cr_access_mon");
                                if((fp = fopen(file_name, "w"))==NULL) return -1;
                                for (int j=0; j<res; j++) fprintf(fp,"%"PRIx64"\n", buffer[j]);
			*/
				printf("\n\n\n\n\n BIOS end \n\n\n\n\n");
			}

			free(buffer);
		}
		uint64_t buffer_timestamps [exit_step];
		xc_vmcs_fuzzing(pxch,dom_id , VMCS_TIMING_BOOT_MONITORING_STOP, exit_step, buffer_timestamps);
		if((fp = fopen("./timestamps_monitoring.txt", "w"))==NULL) return -1;
                for (int i=0;i<exit_step;i++){
                        fprintf(fp,"%"PRIu64"\n", buffer_timestamps[i]-buffer_timestamps[0]);
                }
                fclose(fp);
		// Destroy the test VM
		sprintf(cmd, "xl destroy %d", dom_id);
		system(cmd);
		break;
	}

	case 'm':{
		unsigned int exit_step = 1;//atoi(argv[2]); 		// granularity	
		unsigned int num_slot = atoi(argv[3]);			// iterations
		int dom_id = atoi(argv[4]);

		int boot_exit_count = 0;
		bool first_iter = true;
		bool bios_end = false;

		FILE *fp;
		FILE *fp_mon;
		char* cmd = malloc(sizeof(char)*50);
		char* file_name = malloc(sizeof(char)*50);
		char* file_name2 = malloc(sizeof(char)*50);

		struct timespec tim, tim2;
                tim.tv_sec = 0;
                tim.tv_nsec = 1L;

		// Sets boot mutation mode
		xc_vmcs_fuzzing(pxch, 0, VMCS_DEBUG_MODE_DISABLE, 0, NULL);
		xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MUTATION_DISABLE, 0, NULL);
	        xc_vmcs_fuzzing(pxch, 0, VMCS_NON_BLOCKING_MODE_ENABLE, 0, NULL);
		
		// Executes all BIOS exits in MONITORING MODE
		for (int i=0; i<MAX_BIOS_EXIT; i++) {

			// Enables boot monitoring mode and allocate the output buffer
			xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_SET_EXIT_N, exit_step, NULL);
			xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_SETUP, BUFFER_DIM_FACTOR * exit_step, NULL);

			// Creates the VM on the first iteration and gets its dom_id
			if (first_iter) {
				system("xl create hvm_configuration.cfg");
				first_iter = false;
			}

			// Waits for the end of monitoring
			res = 0;
			while (res != 1) {
				nanosleep(&tim, &tim2);
				res = xc_vmcs_fuzzing(pxch, 0, VMCS_BOOT_MONITORING_CHECK, 0, NULL);
			}

			// Reads monitored buffer
			uint64_t *buffer_bios = malloc(sizeof(uint64_t) * BUFFER_DIM_FACTOR * exit_step);
			memset(buffer_bios, 0, sizeof(uint64_t) * BUFFER_DIM_FACTOR * exit_step);
			res = xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MONITORING_STOP, BUFFER_DIM_FACTOR * exit_step, buffer_bios);

			// Checks if BIOS is finished
			if (buffer_bios[4]==0x6f5e && buffer_bios[10]==0x10) {

				// Save last bios vmexit
				//sprintf(file_name, "./seeds_inj/last_cr_access_inj");
				//if((fp=fopen(file_name, "w"))==NULL) return -1;
				//for (int j=0; j<res; j++) fprintf(fp,"%"PRIx64"\n", buffer_bios[j]);

				printf("\n\n\n\n\n BIOS end \n\n\n\n\n");
				break;
			}
			else {
				printf("BIOS exit: #%d\n", i);
			}

			free(buffer_bios);
		}

		exit_step=atoi(argv[2]);
		xc_vmcs_fuzzing(pxch, dom_id, VMCS_TIMING_MUTATION_START, exit_step, NULL);

		// Enable MUTATION MODE after the BIOS
                xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MUTATION_SETUP, 0, NULL);

		for (int i=0; i<num_slot; i++) {

			// Reading the seeds to be injected
			int size_buffer = 0;
			seeds_t *seeds = malloc(sizeof(seeds_t));
			uint64_t *buffer = malloc(sizeof(uint64_t) * BUFFER_DIM_FACTOR * exit_step);
			memset(buffer, 0, sizeof(uint64_t) * BUFFER_DIM_FACTOR * exit_step);

			sprintf(file_name, "./seeds_mon/boot_seed%d", i);
			if((fp = fopen(file_name, "r"))==NULL) return -1;
			while(!feof(fp)){
				fscanf(fp,"%"PRIx64"\n", &buffer[size_buffer]); 
				size_buffer++;
			}
			fclose(fp);


			// Get seeds from file (INJECTION MODE discards vmwrites)
			raw_to_seeds(size_buffer, buffer, seeds, INJECTION_MODE);
			if (seeds->size != exit_step) printf("Errore num exit: %lu\n", seeds->size);

			// Resets coverage
                        system("xencov reset");

			// Seeds injection
			for (int j=0; j<seeds->size; j++){
//				printf("BOOT injection: #%d, SEED #%lu\n", i, seeds->seeds_items[j].id);
				int dim_buffer = seeds->seeds_items[j].size*3;
				uint64_t buffer_inject[dim_buffer];
				int count = 0;

				for (int k=0; k<seeds->seeds_items[j].size; k++) {
					buffer_inject[count] = seeds->seeds_items[j].seed_items[k].field;
					buffer_inject[count+1] = seeds->seeds_items[j].seed_items[k].value;
					buffer_inject[count+2] = seeds->seeds_items[j].seed_items[k].type;
					count = count+3;
				}
				while(xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MUTATION_CHECK, 0 , NULL)==1);
				res = xc_vmcs_fuzzing(pxch, dom_id, VMCS_MUTATION_START_NEW_ITERATION_NO_BLOCKING, dim_buffer , buffer_inject);
//				printf("ret: %ld\n", res);
			}
			free(seeds);
			free(buffer);

			// Retrieve coverage after the injection of num_exit
			sprintf(cmd, "xencov read > ./cov_inj/cov_inj%d.dat", i);
			system(cmd);
		}
		 uint64_t buffer_timestamps [exit_step];
                int num_res=xc_vmcs_fuzzing(pxch,dom_id , VMCS_TIMING_MUTATION_STOP, exit_step, buffer_timestamps);
		if((fp = fopen("./timestamps_mutation.txt", "w"))==NULL) return -1;
                for (int i=0;i<exit_step;i++){
                        fprintf(fp,"%"PRIu64"\n", buffer_timestamps[i]-buffer_timestamps[0]);
                }
		fclose(fp);
		printf("Num res %d\n", num_res);
		xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MUTATION_DISABLE, 0, NULL);
		sprintf(cmd, "xl destroy %d", dom_id);
		system(cmd);
		break;

	}


	case 'p':{
		printf("[mode] printing mode\n");

		unsigned int exit_step = atoi(argv[3]);  
		seeds_t* seeds = malloc(sizeof(seeds_t));

		uint64_t *buffer = malloc(sizeof(uint64_t)*BUFFER_DIM_FACTOR * exit_step);
		uint64_t exit_reason = 0;
		int size_buffer = 0;
		FILE* fp;
		int i, j;

		if((fp=fopen(argv[2], "r"))==NULL) return -1;
		while(!feof(fp)){
			fscanf(fp,"%"PRIx64"\n", &buffer[size_buffer]);
			size_buffer++;
		}
		fclose(fp);

 		raw_to_seeds(size_buffer, buffer, seeds, PRINT_MODE);


		for (i = 0; i < seeds->size; i++){
			exit_reason = 52;
			for (j=0;j<seeds->seeds_items[i].size;j++){
				if (seeds->seeds_items[i].seed_items[j].field == 0x00004402) {
					exit_reason = seeds->seeds_items[i].seed_items[j].value;
				}
			}
			
			printf("\nSEED #%ld \n",seeds->seeds_items[i].id);
 			printf("EXIT REASON: %s\n", exit_reason_name[exit_reason]);
                
//			if (exit_reason != 52 && exit_reason != 1) {
				for (j=0;j<seeds->seeds_items[i].size;j++){
					printf("%s: FIELD(%"PRIx64"), VALUE(%"PRIx64")\n",
					type[seeds->seeds_items[i].seed_items[j].type],
					seeds->seeds_items[i].seed_items[j].field,
					seeds->seeds_items[i].seed_items[j].value);
				}
//			}
		}

		free(buffer);
		free(seeds);
		break;
		}

	

	default:
	{
		printf("This mode is not available \n");
		return -1;
	}

	}
	xc_interface_close(pxch);
	return 0;
}


int raw_to_seeds(int size_buffer, uint64_t*buffer, seeds_t* seeds, int mode){
	int i,j,i_count;
	int size = size_buffer;
	unsigned long *sizes = malloc(sizeof(unsigned long)*size);
	unsigned long *indexes = malloc(sizeof(unsigned long)*size);

	if (size%3==1) return -1;
	i_count=-1;

	for (i=0;i<size;i=i+3){
		if(buffer[i]==0xFFFFFFFF && buffer[i+1]==0xFFFFFFFF && buffer[i+2]==0){
			i_count++;			//number of exit reasons
			indexes[i_count]=i;		//index of first row related to an exit		
			sizes[i_count]=0;		//number of reads/writes within an exit reason
		}
		else if(i_count>=0) { 
			sizes[i_count]++;
		}
	}
	i_count++;

	seeds->size = i_count; 
	seed_t* seeds_items = malloc(sizeof(seed_t)*i_count);

	for (i=0;i<i_count;i++){			//foreach exit reason
		int count = 0;
		int field_type = 0;
		int field_area = 0;
		seeds_items[i].id = i;
		bin_data_t* data = malloc(sizeof(bin_data_t)*sizes[i]);

		for (j=0;j<sizes[i]*3;j=j+3){				// foreach read/write within an exit reason
			if (mode == INJECTION_MODE) {
				field_type = buffer[indexes[i]+j+5];
				field_area = ((1<<2)-1) & (buffer[indexes[i]+j+3] >> (11-1)); 
			}
			else if (mode == PRINT_MODE) {
				field_type = 999;
			}
 			if (field_type != 0){
				data[count].field=buffer[indexes[i]+j+3];
				data[count].value=buffer[indexes[i]+j+4];
				data[count].type=buffer[indexes[i]+j+5];
				count++;
			}
		}
		seeds_items[i].seed_items=data;
		seeds_items[i].size=count;
	}
	seeds->seeds_items=seeds_items;

	return 0;
}


// Usage example ./print_exit_distr seeds_file num_exit

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <sched.h>
#include <time.h>

#define BUFFER_DIM_FACTOR 500
#define BIOS_EXIT 15000

const char* type[3] = {
    [0]="WR",
    [1]="RD",
    [2]="REG"
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
    seed_t* seeds_items;
};
typedef struct seeds seeds_t;



int raw_to_seeds(int size, uint64_t*buffer, seeds_t* seeds);


int main(int argc, char* argv[])
{

	unsigned int num_exit = atoi(argv[2]);  
	seeds_t* seeds = malloc(sizeof(seeds_t));

	uint64_t *buffer = malloc(sizeof(uint64_t)*BUFFER_DIM_FACTOR*num_exit);
	uint64_t exit_reason = 0;
	int size_buffer = 0;
	FILE* fp;
	int i, j;

	if((fp=fopen(argv[1], "r"))==NULL) {exit(1);}
	while(!feof(fp)){
		fscanf(fp,"%"PRIx64"\n", &buffer[size_buffer]);
		size_buffer++;
	}
	fclose(fp);

	raw_to_seeds(size_buffer, buffer, seeds);

	for (i = 0; i < seeds->size; i++){
		printf("\nSEED #%ld ",seeds->seeds_items[i].id);
		for (j=0;j<seeds->seeds_items[i].size;j++){
			if (seeds->seeds_items[i].seed_items[j].field == 0x00004402) {
				exit_reason = seeds->seeds_items[i].seed_items[j].value;
			}
/*			printf("%s: FIELD(%"PRIx64"), VALUE(%"PRIx64")\n",
			type[seeds->seeds_items[i].seed_items[j].type],
			seeds->seeds_items[i].seed_items[j].field,
			seeds->seeds_items[i].seed_items[j].value);
*/
		}
		printf("EXIT REASON: %ld %s", exit_reason, exit_reason_name[exit_reason]);
	}

	free(buffer);
	free(seeds);

	return 0;
}


int raw_to_seeds(int size_buffer, uint64_t*buffer, seeds_t* seeds){

	int i,j,i_count;
	int size = size_buffer;
	unsigned long *sizes = malloc(sizeof(unsigned long)*size);
	unsigned long *indexes = malloc(sizeof(unsigned long)*size);

	if (size%3==1) return -1;
	i_count=-1;

	for (i=0;i<size;i=i+3){
		if(buffer[i]==0xFFFFFFFF && buffer[i+1]==0xFFFFFFFF && buffer[i+2]==0){
			i_count++;			// number of vmexits
			indexes[i_count]=i;		// index of first row related to a vmexit
			sizes[i_count]=0;		// number of reads/writes within a vmexit
		}
		else if(i_count>=0) {
			sizes[i_count]++;
		}
	}
	i_count++;

	seeds->size = i_count;
	seed_t* seeds_items = malloc(sizeof(seed_t)*i_count);

	// Foreach vmexit
	for (i=0;i<i_count;i++){
		int count = 0;
		seeds_items[i].id = i;
		bin_data_t* data = malloc(sizeof(bin_data_t)*sizes[i]);

		// Foreach read/write within a vmexit
		for (j=0;j<sizes[i]*3;j=j+3){
			data[count].field=buffer[indexes[i]+j+3];
			data[count].value=buffer[indexes[i]+j+4];
			data[count].type=buffer[indexes[i]+j+5];
			count++;
		}
		seeds_items[i].seed_items=data;
		seeds_items[i].size=count;
	}
	seeds->seeds_items=seeds_items;

	return 0;
}


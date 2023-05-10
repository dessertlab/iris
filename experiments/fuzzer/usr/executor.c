#ifndef EXECUTOR_H
#include "executor.h"
#endif
#include "time.h"

void prepare_to_fire(int dom_id, xc_interface *pxch){

    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 1L;
    xc_vmcs_fuzzing(pxch, 0, VMCS_NON_BLOCKING_MODE_ENABLE, 0, NULL);
    xc_vmcs_fuzzing(pxch, 0, VMCS_DEBUG_MODE_DISABLE, 0, NULL);
    xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MUTATION_SETUP, 0, NULL);
    system("xl restore ./hvm_configuration.cfg guest_snap");
    //system("xl create ./hvm_configuration.cfg");
//xc_vmcs_fuzzing(pxch, dom_id, VMCS_BOOT_MUTATION_SETUP, 0, NULL);
}

int fire(int dom_id, xc_interface *pxch, seeds_t mutations){
    int ret,c=0;
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 1L;
    for (int j=0; j<mutations.size; j++){
	//printf("mutation %d of the replayed sequence\n", j);
        if(j==3969) continue;
	int dim_buffer = mutations.seeds_items[j].size*3;
        uint64_t buffer_inject[dim_buffer];
        int count = 0;

        for (int k=0; k<mutations.seeds_items[j].size; k++) {
		if (mutations.seeds_items[j].seed_items[k].type!=0){
                buffer_inject[count] = mutations.seeds_items[j].seed_items[k].field;
                buffer_inject[count+1] = mutations.seeds_items[j].seed_items[k].value;
                buffer_inject[count+2] = mutations.seeds_items[j].seed_items[k].type;
                count = count+3;
		} 
        }
        ret=-1;
	//while(xc_vmcs_fuzzing(pxch, dom_id, WAITING_FOR_NEW_MUTATION, 0 , NULL)==1){printf("waiting\n");}
	//ret=0;
        
	for(uint64_t i=0; i<100000;i++){
            if(xc_vmcs_fuzzing(pxch, dom_id, WAITING_FOR_NEW_MUTATION, 0 , NULL)==0){
                ret=0;
                break;
            }
	    nanosleep(&tim,&tim2);
        }
	
        if (ret){printf("Hang on CPU3\n"); return -1;}
        c = xc_vmcs_fuzzing(pxch, dom_id, VMCS_MUTATION_START_NEW_ITERATION_NO_BLOCKING, dim_buffer , buffer_inject);
        //printf("res: %ld, mutation %d \n", c, j);
    }
   return ret;    

}

void stop_to_fire(xc_interface *pxch){
    xc_interface_close(pxch);
    system("xl destroy hvm_guest");
}

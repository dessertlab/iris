#ifndef MUTATOR_H
#include "mutator.h"
#endif
#include "time.h"
#define MAX_SIZE_SEED 200


/**
 * @brief Returns new consecutive mutations to do and uodates the test_case matrix
 * @param test_case matrix of all test case mutations
 * @param mutations new cons mutations
 * @param i starting mutation index in test_case
 * @return int (0) the cons. mutations are generated and saved (-1) are generated but not saved (-2) are not generated 
 */
int new_cons_mutations(seeds_t seeds, seeds_exit_reason_t* seeds_exit_reason, seeds_t* mutations, int i){
    unsigned long exit_reason;
    seed_t seed;
    seed.size=0;
    exit_reason=get_exit_reason();
    //#ifdef DEBUG_GUNFUZZER2
        printf("[new_cons_mutations] The choosed exit reason: [%s]\n",exit_reason_name[exit_reason]);
    //#endif
    
    get_seed(seeds, seeds_exit_reason[exit_reason], &seed);
    //#ifdef DEBUG_GUNFUZZER2
        if (seed.size!= 0){
            printf("[new_cons_mutations] New seed to mutate\n");
            for (int i=0;i<seed.size;i++){
                printf("VMREAD FIELD(%"PRIx64"), VALUE(%"PRIx64"), TYPE(%"PRIx64") \n",
                seed.seed_items[i].field,
                seed.seed_items[i].value,
                seed.seed_items[i].type
                );
            }
        }else {
            printf("[new_cons_mutations] No seeds available\n");
        }
    //#endif


    #ifdef DEBUG_GUNFUZZER2
        printf("[new_cons_mutations] Genarates the new mutations \n ");
    #endif
    gen_mutations(exit_reason, seed, mutations, i);

    #ifdef DEBUG_GUNFUZZER3
    for (int i=0;i<mutations->size;i++){
        printf("Mutation [%d], exit reason [%d], size %d\n",i, mutations->seeds_items[i].exit_reason, mutations->seeds_items[i].size);
        for(int j=0; j<mutations->seeds_items[i].size; j++){
            printf("field %"PRIx64", value %"PRIx64"\n", mutations->seeds_items[i].seed_items[j].field, mutations->seeds_items[i].seed_items[j].value );
        }
    }
    #endif

    return 0;
}

void get_seed(seeds_t seeds, seeds_exit_reason_t seeds_exit_reason, seed_t* seed){

    unsigned long seed_idx = 0; 
    if (seeds_exit_reason.size!=0){
        seed_idx = (rand() % (seeds_exit_reason.size + 1));
        printf("seed number %d \n",seed_idx);
    }else{
        seed->size=0;
        return;
    }

    seed->exit_reason=seeds.seeds_items[seeds_exit_reason.idx_seeds[seed_idx]].exit_reason;
    seed->id=seeds.seeds_items[seeds_exit_reason.idx_seeds[seed_idx]].id;
    seed->seed_items=seeds.seeds_items[seeds_exit_reason.idx_seeds[seed_idx]].seed_items;
    seed->size=seeds.seeds_items[seeds_exit_reason.idx_seeds[seed_idx]].size;
    return;
}


/**
 * @brief Choose a random exit reason
 * 
 * @return unsigned int 
 */
unsigned long get_exit_reason() {
	int lower = 0;
	int upper = 64;
	unsigned long reason = (rand() % (upper - lower + 1)) + lower;
	
	// Filter exit reason with a blacklist
	while (check_reason(reason)) {
		reason = (rand() % (upper - lower + 1)) + lower;
	}	
	return reason;
}


/**
 * @brief Checks whether a reason is blacklisted
 * 
 * @param reason 
 * @return int 
 */
int check_reason(unsigned long reason) {
	return reason_blacklist[reason];
}


/**
 * @brief Given a seed, (1) it generarates new (M_CONS-1) mutations, (2) updates the test_case matrix 
 * 
 * @param seed the seed to mutate to get the new mutations
 * @param mutations the new mutations under struct form
 * @param test_case the updated test_case with the new mutations
 * @param i current mutation index in test_case (the row)
 * @return int 
 */
int gen_mutations(unsigned long exit_reason, seed_t seed, seeds_t* mutations, int i){
    int j=0;
    int8_t regs_seed_idx_size=0;
    int8_t reads_seed_idx_size=0;
    int8_t regs_seed_idx[seed.size];
    int8_t reads_seed_idx[seed.size];
    int index_rand_read=0;

    if(!(seed.size>0)) return -1;
    seed_t* seeds_items = malloc(sizeof(seed_t)*M_CONS);
    for (j=0;j<M_CONS;j++){
	memset(fields_whitelist,0, sizeof(fields_whitelist));
        int8_t rand_reg_idx=0;
        int8_t rand_read_idx=0;
        int8_t lower=0;
        int8_t upper=0;
	//int index_rand_read=0;
        if(seed.size>0 && j==0){
            seeds_items[j].exit_reason=seed.exit_reason;
            seeds_items[j].size=seed.size;
            seeds_items[j].id=seed.id;
            seeds_items[j].seed_items=malloc(sizeof(bin_data_t)*seed.size);
            for(int k=0;k<seed.size;k++){
                seeds_items[j].seed_items[k].field=seed.seed_items[k].field;
                seeds_items[j].seed_items[k].value=seed.seed_items[k].value;
                seeds_items[j].seed_items[k].type=seed.seed_items[k].type;
                if(seed.seed_items[k].type==TYPE_RD){
                    if(seed.seed_items[k].field!=VM_EXIT_REASON&&seed.seed_items[k].field!=VM_EXIT_INSTRUCTION_LEN&&seed.seed_items[k].field!=GUEST_RIP&&seed.seed_items[k].field!=0x6400){
                    reads_seed_idx[reads_seed_idx_size]=k;
                    reads_seed_idx_size++;
		    
                    }
                }else if(seed.seed_items[k].type==TYPE_REG){
                    regs_seed_idx[regs_seed_idx_size]=k;
                    regs_seed_idx_size++;
                }
            }
            continue;
        }
        if (mutation_mode==RAND_REG){
            int index;
            rand_reg_idx = regs_seed_idx[(rand() % (regs_seed_idx_size - 0 + 1)) + 0];
            get_idx_mutation(seed.seed_items[rand_reg_idx].field,index);
            fields_whitelist[index]=1;
	   //printf("RAND REGISTER\n");

        }else if (mutation_mode==RAND_RD){
	    if(j==1){
            int index;
            rand_read_idx = reads_seed_idx[(rand() % (reads_seed_idx_size - 0 + 1)) + 0];
            get_idx_mutation(seed.seed_items[rand_read_idx].field,index);
            fields_whitelist[index]=1;
	    index_rand_read=index;
//            printf("RAND READ\n");
            }else{ fields_whitelist[index_rand_read]=1;} //mutates always the same field
        }
        gen_single_mutation(exit_reason, &seeds_items[j], seed);
    }
    mutations->size=M_CONS;
    mutations->seeds_items=seeds_items;
    return 0;
}


/**
 * @brief Muatates the seed getting a new mutation
 * 
 * @param mutation the new generated mutation
 * @param seed the seed
 */
void gen_single_mutation(unsigned long exit_reason, seed_t* mutation, seed_t seed){
    /**Seed available**/
    mutation->exit_reason=seed.exit_reason;
    mutation->size=seed.size;
    mutation->id=seed.id;
    mutation->seed_items=malloc(sizeof(bin_data_t)*seed.size);
    //Choose a random field to mutate
    int lower = 0;
	int upper = seed.size;
	unsigned long field_int_value = (rand() % (upper - lower + 1)) + lower;

    for(int k=0;k<seed.size;k++){
        /**Mutating all the reading nominal starting from a nominal value**/
        mutation->seed_items[k].field=seed.seed_items[k].field;
        mutation->seed_items[k].type=seed.seed_items[k].type;
        mutation_field_by_seed(exit_reason, seed.seed_items[k].field, seed.seed_items[k].value, &mutation->seed_items[k].value);
    }

}

void mutation_field_by_seed(uint64_t exit_reason, uint64_t field, uint64_t value, uint64_t* mutated_value){
    int index = 0;
    get_idx_mutation(field,index);
    if(fields_whitelist[index]){
        *mutated_value=random_bit_flip(value);
    }
    else{
        *mutated_value=value; 
    }
    return;
}



void mutation_field(uint64_t exit_reason, uint64_t field, uint64_t* mutated_value){
    printf("");
    return;
}



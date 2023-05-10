#define XEN
#ifndef MUTATOR_H
#include "mutator.h"
#endif
#ifdef XEN
#ifndef EXECUTOR_H
#include "executor.h"
#endif
#endif


#include <getopt.h>
#define _POSIX_C_SOURCE 2



/** test case **/
uint8_t vmreads_prof [M][N];
uint8_t vmwrites_prof [M][N];

/**One seed list for each exit reason**/
seeds_exit_reason_t seeds_exit_reason [N_EXIT_REASONS];

seeds_t seeds_all;

int8_t reason_blacklist[N_EXIT_REASONS];
int8_t fields_whitelist[MUTATION_SIZE];
int8_t mutation_mode=0;

#define PATH_MAIN "."

void print_seed(seed_t seed);
void print_seeds(seeds_t seeds);

int main(int argc, char*argv[])
{
    /**COMMAND LINE INPUTS HANDLING START*/
    int opt= 0;
    int cov_flag=0;
    char *in_fname = NULL;
    char *out_fname = NULL;
    int i,j,ret=0;
    int index=0;
    int dom_id=1;
    int n=100;
    seed_t seed;
    seeds_t mutations;
    mutation_mode=0;
        
    in_fname=argv[1];
    out_fname=argv[2];
    mutation_mode=atoi(argv[3]);
    dom_id=atoi(argv[4]);
    n=atoi(argv[5]);
    cov_flag=1;
    printf("Input seed %s, out fname %s, mutation mode: %d, dom id %d, n: %d, cov enabled",in_fname, out_fname, mutation_mode, dom_id, n);

  /**COMMAND LINE INPUTS HANDLING STOP*/

   

    /**new dom u**/
    #ifdef XEN
    xc_interface *pxch = xc_interface_open(NULL, NULL, 0);
    prepare_to_fire(dom_id, pxch);
    #endif

    /**file for persistent storage**/
    FILE* fp;
//    if((fp=fopen(argv[1], "a"))==NULL) {printf("Errore nell'apertura del file'");exit(1);}

    /**new PRG seed**/
    srand((unsigned) time(0));
    /** fetching from seed file**/
    char seed_filename[100];
    sprintf(seed_filename, "%s/seeds/%s", PATH_MAIN, in_fname);
    get_seeds(seed_filename, &seeds_all, seeds_exit_reason);
    #ifdef DEBUG_GUNFUZZER1
        printf("EXIT_REASON_IO_INSTRUCTION seeds\n");
        for (int i=0;i<seeds_exit_reason[EXIT_REASON_CPUID].size;i++){
            printf("New seed\n");
            for (int j=0;j<seeds_all.seeds_items[seeds_exit_reason[EXIT_REASON_CPUID].idx_seeds[i]].size;j++){
                printf("VMREAD FIELD(%"PRIx64"), VALUE(%"PRIx64"), TYPE(%"PRIx64") \n",
                seeds_all.seeds_items[seeds_exit_reason[EXIT_REASON_CPUID].idx_seeds[i]].seed_items[j].field,
                seeds_all.seeds_items[seeds_exit_reason[EXIT_REASON_CPUID].idx_seeds[i]].seed_items[j].value,
                seeds_all.seeds_items[seeds_exit_reason[EXIT_REASON_CPUID].idx_seeds[i]].seed_items[j].type
                );
            }
            
        }
    #endif

    

    /** white list of fields included foor fuzzing**/
    memset(fields_whitelist,0, sizeof(fields_whitelist));
    if(mutation_mode==ALL_REGS) include_gp_regs(fields_whitelist);
    if(mutation_mode==ALL_REGS) not_include_gp_regs(fields_whitelist);

    /** Coverage commands*/
    char cmd_cov_read [90];
    char cmd_cov_reset [90];
    sprintf(cmd_cov_read, "xencov read > %s/0_cov/%s-%d.dat ",PATH_MAIN, out_fname, 0);
    sprintf(cmd_cov_reset, "xencov reset", out_fname, i);
    if(cov_flag==1) system(cmd_cov_reset);
    
    //NEW CODE: MUTATION FUZZER
    //test case runs n-1 consecutive seeds without mutation. Then it mutates the n-ith seed for M_CONS times
    
    
    if (n>seeds_all.size) return -2;
    
    seed = seeds_all.seeds_items[n-1];
    
    //Generates n consecutive mutations
//    printf("Th %d-ith seed \n",n);
    print_seed(seed);
    printf("-	-	-	-	-	- \n");
    gen_mutations(seed.exit_reason, seed, &mutations ,0);
    printf("Mutations:\n");
    //print_seeds(mutations);

    // Save the consecutive mutations before the execution
    char mutation_filename[200];
    sprintf(mutation_filename, "%s/0_crashes_hangs/%s", PATH_MAIN, out_fname);
    if((fp=fopen(mutation_filename, "a"))==NULL) {printf("Errore nell'apertura del file'");exit(1);} 
    for (int j=0;j<n;j++){
        fprintf(fp,"ffffffff\n");
        fprintf(fp,"ffffffff\n");
        fprintf(fp,"0\n");
        for (int k=0;k<seeds_all.seeds_items[j].size;k++){
            fprintf(fp,"%"PRIx64"\n", seeds_all.seeds_items[j].seed_items[k].field);
            fprintf(fp,"%"PRIx64"\n", seeds_all.seeds_items[j].seed_items[k].value);
            fprintf(fp,"%"PRIx64"\n", seeds_all.seeds_items[j].seed_items[k].type);
        }
    }

    for (int j=0;j<mutations.size;j++){
        fprintf(fp,"ffffffff\n");
        fprintf(fp,"ffffffff\n");
        fprintf(fp,"0\n");
        for (int k=0;k<mutations.seeds_items[j].size;k++){
            fprintf(fp,"%"PRIx64"\n", mutations.seeds_items[j].seed_items[k].field);
            fprintf(fp,"%"PRIx64"\n", mutations.seeds_items[j].seed_items[k].value);
            fprintf(fp,"%"PRIx64"\n", mutations.seeds_items[j].seed_items[k].type);
        }
    }
    fclose(fp);
    char cmd_ssh[1000];
    sprintf(cmd_ssh,"sshpass -p cardamom00 scp ./%s/0_crashes_hangs/%s test@192.168.100.7:/home/test/gunfuzzer/testcases",PATH_MAIN,out_fname);
    system(cmd_ssh);
    //Limit the seeds sequence to n minus one
    seeds_all.size=n;

    //Runs the mutations
    #ifdef XEN
    if (fire(dom_id,pxch, seeds_all)) { ret=-1; goto close_test_case;};
    //if(cov_flag==1) system(cmd_cov_reset);
    if(cov_flag==1) system(cmd_cov_read);
    sprintf(cmd_cov_read, "xencov read > %s/0_cov/%s-%d.dat ",PATH_MAIN, out_fname, 1);
    printf("replayed the sequence until the %d-ith seed\n",n);
    if (fire(dom_id,pxch, mutations)) { ret=-1; goto close_test_case;};
    if(cov_flag==1) system(cmd_cov_read);
    #endif
    for (int j=0; j<mutations.size;j++){
        if(mutations.seeds_items[j].seed_items) free(mutations.seeds_items[j].seed_items);
    }
    if(mutations.seeds_items) free(mutations.seeds_items);
    

close_test_case:
    #ifdef XEN
    if(ret==-1&&cov_flag==1) system(cmd_cov_read);
    stop_to_fire(pxch);
    #endif
    return ret;
}



void print_seeds(seeds_t seeds){

    for (int i=0;i<seeds.size;i++){
        printf("New seed\n");
        for (int j=0;j<seeds.seeds_items[i].size;j++){
            printf("VMREAD FIELD(%"PRIx64"), VALUE(%"PRIx64"), TYPE(%"PRIx64") \n",
            seeds.seeds_items[i].seed_items[j].field,
            seeds.seeds_items[i].seed_items[j].value,
            seeds.seeds_items[i].seed_items[j].type
            );
        }
        
    }
}

void print_seed(seed_t seed){
    printf("New seed\n");
    for (int j=0;j<seed.size;j++){
        printf("VMREAD FIELD(%"PRIx64"), VALUE(%"PRIx64"), TYPE(%"PRIx64") \n",
        seed.seed_items[j].field,
        seed.seed_items[j].value,
        seed.seed_items[j].type
        );
    }
        
    
}






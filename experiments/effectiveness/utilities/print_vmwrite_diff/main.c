#include "mylib.h"
#include <inttypes.h>


#define GUEST_STATE_AREA 2

/** test case **/
uint8_t vmreads_prof [M][N];
uint8_t vmwrites_prof [M][N];

seeds_t seeds_all;
seeds_exit_reason_t seeds_exit_reason [N_EXIT_REASONS];

int8_t reason_blacklist[N_EXIT_REASONS];
int8_t fields_whitelist[MUTATION_SIZE];



#define WRnR 0
#define AllRegs 1


void convert_raw_to_matrix(int mode);

int main(int argc, char*argv[])
{
    int i,j,ret=0;
    int index=0;
    int mode=atoi(argv[1]);
    
    convert_raw_to_matrix(mode);

    return 0;
}



void convert_raw_to_matrix(int mode){
    /**file for persistent storage**/
    FILE* fp;
    FILE* fp1;
    char filename [100];
    char filename1 [100];
    get_seeds(&seeds_all, seeds_exit_reason);
    uint64_t** matrix=malloc(sizeof(int64_t*)*seeds_all.size);

    uint64_t idx_to_fields[MUTATION_SIZE];
    int index;
    for (int i=0; i<MUTATION_SIZE; i++) {
        get_idx_mutation(vmcs_fields_vett[i], index);
        idx_to_fields[index]=vmcs_fields_vett[i];
    }


    int count_matrix = 0;
    for (int i=0;i<seeds_all.size; i++){

        uint64_t* matrix_temp = malloc (MUTATION_SIZE*sizeof(uint64_t));
        memset(matrix_temp, 0, sizeof(matrix_temp));

        unsigned exit_reason;
        for (int j=0;j<seeds_all.seeds_items[i].size;j++){
            unsigned int type_r=seeds_all.seeds_items[i].seed_items[j].type;
            unsigned int field=seeds_all.seeds_items[i].seed_items[j].field;
            int64_t value = seeds_all.seeds_items[i].seed_items[j].value;
	    int vmcs_area = ((1<<2)-1) & (field >> (11-1));

            int index = 0;
            get_idx_mutation(field,index);
            //int index_filter;
            //get_idx_mutation(0x0000401c,index_filter);
 
            if (field == 0x00004402) {
                exit_reason = value;
            }

	    switch(mode){
	    case 1:{
        	if (type_r==0) {
                	//if(index!=index_filter)  matrix_temp[index] = value;
			if(vmcs_area==GUEST_STATE_AREA)  matrix_temp[index] = value;
            	}
		break;
	    }
	    case 2:{
		if (type_r==1 || type_r==2) {
			matrix_temp[index] = value;
		}
		break;
	    }
	    }

        }

        if (exit_reason!=49 && exit_reason!=18 && exit_reason!=12) {
            matrix[count_matrix] = matrix_temp;
            count_matrix++;
        }
        else {
            free(matrix_temp);
        }

    }
    printf("Number of rows %d\n", count_matrix);
    sprintf(filename,"./seeds_matrix");
    if((fp=fopen(filename, "w"))==NULL) {printf("Errore nell'apertura del file");exit(1);}

    // Print labels

    for (int k=0; k<MUTATION_SIZE-1; k++) {
	if (idx_to_fields[k]>0x00006c16) {
		fprintf(fp,"%llu,",idx_to_fields[k]);
	}
	else {
        	fprintf(fp,"%s,",VMCS_FIELDS[idx_to_fields[k]]);
    	}
    }
    fprintf(fp,"%s\n", VMCS_FIELDS[idx_to_fields[MUTATION_SIZE]]);

    // Print matrix
    for (int i=0;i<count_matrix;i++){
	int j;
        for (j=0;j<MUTATION_SIZE-1;j++){
            fprintf(fp,"%llu,",matrix[i][j]);
        }
        fprintf(fp,"%llu",matrix[i][j]);
        free(matrix[i]);
        fprintf(fp,"\n");
    }
    free(matrix);
    fclose(fp);


}






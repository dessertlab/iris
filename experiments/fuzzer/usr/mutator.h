#define MUTATOR_H
#ifndef MYLIB_H
#include "mylib.h"
#endif
/* Non-injectable exit reasons */



int new_cons_mutations(seeds_t seeds, seeds_exit_reason_t* seeds_exit_reason, seeds_t* new_mutations, int i);
void get_seed(seeds_t seeds, seeds_exit_reason_t seeds_exit_reason, seed_t* seed);
unsigned long get_exit_reason();
int check_reason(unsigned long reason);
int gen_mutations(unsigned long exit_reason, seed_t seed, seeds_t* mutations, int i);
void seed_to_row(uint64_t* mutation_o, seed_t mutation_i);
void gen_single_mutation(unsigned long exit_reason, seed_t* mutation,seed_t seed);
void mutation_field_by_seed(uint64_t exit_reason, uint64_t field, uint64_t value, uint64_t* mutated_value);
void mutation_field(uint64_t exit_reason, uint64_t field, uint64_t* mutated_value);





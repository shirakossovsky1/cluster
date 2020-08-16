/*
 * power_iteration.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef POWER_ITERATION_H_
#define POWER_ITERATION_H_

#include "leading_eigenpair.h"

void power_iteration(modularity_matrix* mod_matrix, leading_eigenpair leading_pair);
double* generate_rand_vec(double* vec_0, int vec_size);
void get_next_vec(modularity_matrix* mod_matrix, double* vec, double* next_vec, double* curr_row , int vec_size);
int smaller_than_eps(double* vec, double* next_vec, int vec_size);
double calc_multiplication(modularity_matrix* mod_matrix, double *vec, int row);


#endif /* POWER_ITERATION_H_ */

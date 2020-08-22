/*
 * power_iteration.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef POWER_ITERATION_H_
#define POWER_ITERATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "leading_eigenpair.h"
#include "modularity_matrix.h"

void power_iteration(modularity_matrix* mod_matrix, double* eigen_vector);
double* generate_rand_vec(double* vec_0, int vec_size);
void get_next_vec(modularity_matrix* mod_matrix, double* vec, double* next_vec, int vec_size);
int smaller_than_eps(double* vec, double* next_vec, int vec_size);
double calc_multiplication(modularity_matrix* mod_matrix, double *vec, int row, bool to_shift);
double mult_sparse_mat_row_by_vec(modularity_matrix* mod_matrix, double *vec, int row);
double dot_product(double* vector_1, double* vector_2, int vec_size);
double int_dot_product(int* vector_1, double* vector_2, int vec_size);
double sum_vec(double* vector, int vec_size);


#endif /* POWER_ITERATION_H_ */

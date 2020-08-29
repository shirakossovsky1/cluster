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

void power_iteration(modularity_matrix* mod_matrix, float* eigen_vector);
float* generate_rand_vec(float* vec_0, unsigned int vec_size);
void get_next_vec(modularity_matrix* mod_matrix, float* vec, float* next_vec, unsigned int vec_size);
unsigned int smaller_than_eps(float* vec, float* next_vec, unsigned int vec_size);
float calc_multiplication(modularity_matrix* mod_matrix, float *vec, unsigned int row, bool to_shift);
float mult_sparse_mat_row_by_vec(modularity_matrix* mod_matrix, float *vec, unsigned int row);
float dot_product(float* vector_1, float* vector_2, unsigned int vec_size);
float int_dot_product(unsigned int* vector_1, float* vector_2, unsigned int vec_size);
float sum_vec(float* vector, unsigned int vec_size);


#endif /* POWER_ITERATION_H_ */

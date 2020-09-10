/*
 * The module contains linear algebra operations used in the project.
 */

#ifndef LINEAR_ALGEBRA_UTILS_H_
#define LINEAR_ALGEBRA_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "error.h"
#include "modularity_matrix.h"

float* mult_matrix_by_vector(modularity_matrix* mod_matrix, float* vec, float* result_vec, unsigned int vec_size, bool to_shift,  bool to_hat);
float mult_modularity_mat_row_by_vec(modularity_matrix* mod_matrix, float *vec, unsigned int row, bool to_shift, bool to_hat);
float mult_sparse_mat_row_by_vec(modularity_matrix* mod_matrix, float *vec, unsigned int row);
float float_dot_product(float* vector_1, float* vector_2, unsigned int vec_size);
float int_dot_product(unsigned int* vector_1, float* vector_2, unsigned int vec_size);
float sum_vec(float* vector, unsigned int vec_size);

#endif /* LINEAR_ALGEBRA_UTILS_H_ */

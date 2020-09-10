/*
 * The module executes power iteration process for finding leading eigenvector.
 * contains all the functions that relevant for the process.
 */

#ifndef POWER_ITERATION_H_
#define POWER_ITERATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "leading_eigenpair.h"
#include "modularity_matrix.h"
#include "linear_algebra_utils.h"

void power_iteration(modularity_matrix* mod_matrix, float* eigen_vector);
float* generate_rand_vec(float* vec_0, unsigned int vec_size);
void get_next_vec(modularity_matrix* mod_matrix, float* vec, float* next_vec, unsigned int vec_size);
unsigned int smaller_than_eps(float* vec, float* next_vec, unsigned int vec_size);

#endif /* POWER_ITERATION_H_ */

/*
 * leading_eigenpair.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef LEADING_EIGENPAIR_H_
#define LEADING_EIGENPAIR_H_

#include "power_iteration.h"
#include "modularity_matrix.h"

typedef struct leading_eigenpair{

	float 			  *leading_eigenvector;
	float 			  leading_eigenvalue;
	modularity_matrix *mod_matrix;
	float 			  *division_vector;

}leading_eigenpair;

leading_eigenpair* create_eigenpair(modularity_matrix* mod_matrix);
float find_leading_eigenvalue(leading_eigenpair* eigenpair);
float* find_leading_eigenvector(modularity_matrix* mod_matrix);
float* mult_matrix_by_vector(modularity_matrix* mod_matrix, float* vec, float* result_vec, unsigned int vec_size, bool to_shift);
float* calc_division_vector(leading_eigenpair* eigenpair);
void free_leading_eigenpair(leading_eigenpair* eigenpair);

#endif /* LEADING_EIGENPAIR_H_ */

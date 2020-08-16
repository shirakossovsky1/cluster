/*
 * leading_eigenpair.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef LEADING_EIGENPAIR_H_
#define LEADING_EIGENPAIR_H_

#include "power_iteration.h"

typedef struct leading_eigenpair{

	double *leading_eigenvector;
	double leading_eigenvalue;
	modularity_matrix mod_matrix;
	int *division_vector;

}leading_eigenpair;

leading_eigenpair* create_eigenpair(modularity_matrix* mod_matrix);
double find_leading_eigenvalue(leading_eigenpair* eigenpair);
double* find_leading_eigenvector(modularity_matrix mod_matrix, leading_eigenpair* eigenpair);
double* mult_matrix_by_vector(modularity_matrix* mod_matrix, double* vec, double* result_vec, int vec_size);
int* calc_division_vector(leading_eigenpair* eigenpair);

#endif /* LEADING_EIGENPAIR_H_ */

/*
 * leading_eigenpair.c
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "leading_eigenpair.h"

leading_eigenpair* create_eigenpair(modularity_matrix* mod_matrix) {
	leading_eigenpair* eigenpair = (leading_eigenpair*)malloc(sizeof(leading_eigenpair));

	eigenpair -> leading_eigenvector = find_leading_eigenvector(eigenpair);
	eigenpair -> leading_eigenvalue = find_leading_eigenvalue(eigenpair);
	eigenpair -> mod_matrix = mod_matrix;
	eigenpair -> division_vector = calc_division_vector(eigenpair);

	return eigenpair;
}

double find_leading_eigenvalue(leading_eigenpair* eigenpair){
	 double* tmp_vec
	 double b;
	 double c;

	 tmp_vec = (double*)malloc(sizeof(double) * (eigenpair -> mod_matrix.sub_vertices_group_size));

	 tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, eigenpair -> leading_eigenvector, tmp_vec, eigenpair -> mod_matrix.sub_vertices_group_size);
	 b = dot_product(eigenpair -> leading_eigenvector, tmp_vec);
	 c = dot_product(eigenpair -> leading_eigenvector, eigenpair -> leading_eigenvector);

	 free(tmp_vec);

	 return ((b / c) - eigenpair -> mod_matrix.norm_1);
}

double* find_leading_eigenvector(modularity_matrix mod_matrix, leading_eigenpair* eigenpair){
	double* eigenvector;

	eigenvector = (double*)malloc(sizeof(double)*mod_matrix.sub_vertices_group_size);
	power_iteration(mod_matrix, eigenvector);

	return eigenvector;
}

double* mult_matrix_by_vector(modularity_matrix* mod_matrix, double* vec, double* result_vec, int vec_size){
	double result;

	double *vec_ptr;
	double *next_vec_ptr = result_vec;
	int curr_row = 0;

	for (vec_ptr=vec; vec_ptr < &vec[vec_size]; vec_ptr++) {
		result = calc_multiplication(mod_matrix, vec, curr_row);
		*vec_ptr = *next_vec_ptr;
		*next_vec_ptr = result;
		next_vec_ptr++;
		curr_row++;
 	}

	return result_vec;

}

int* calc_division_vector(leading_eigenpair* eigenpair) {
	int* division_vector;
	int* division_vector_ptr;
	double* eigen_vector_ptr;
	int i;

	division_vector = (int*)malloc(sizeof(int)*eigenpair->mod_matrix.sub_vertices_group_size);
	division_vector_ptr = division_vector;
	eigen_vector_ptr = eigenpair -> leading_eigenvector;

	for (i = 0; i < eigenpair -> mod_matrix.sub_vertices_group_size; i++) {
		if (*eigen_vector_ptr > 0) {
			*division_vector_ptr = 1;
		}
		else {
			*division_vector_ptr = -1;
		}
		division_vector_ptr++;
		eigen_vector_ptr++;
	}

	return division_vector;
}



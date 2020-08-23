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

	printf("%s\n","&&&&&&&&& starting inside eigenpair file &&&&&&&&&");

	eigenpair -> mod_matrix = mod_matrix;
	eigenpair -> leading_eigenvector = find_leading_eigenvector(mod_matrix);
	printf("eigenpair -> mod_matrix -> sub_vertices_group_size is %d\n",eigenpair -> mod_matrix -> sub_vertices_group_size);
	printf("%s\n","finding leading eigenvalue");
	eigenpair -> leading_eigenvalue = find_leading_eigenvalue(eigenpair);
	printf("%s\n","calc division vector");
	eigenpair -> division_vector = calc_division_vector(eigenpair);

	printf("%s\n","&&&&&&&&& finishing inside eigenpair file &&&&&&&&&");

	return eigenpair;
}

double find_leading_eigenvalue(leading_eigenpair* eigenpair){
	 double* tmp_vec;
	 double b;
	 double c;

	 printf("sub group size is %d\n",eigenpair -> mod_matrix -> sub_vertices_group_size);
	 tmp_vec = (double*)malloc(sizeof(double) * (eigenpair -> mod_matrix -> sub_vertices_group_size));
	 assert(tmp_vec != NULL);

	 tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, eigenpair -> leading_eigenvector, tmp_vec,
			 eigenpair -> mod_matrix -> sub_vertices_group_size, true);
	 b = dot_product(eigenpair -> leading_eigenvector, tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size);
	 c = dot_product(eigenpair -> leading_eigenvector, eigenpair -> leading_eigenvector, eigenpair -> mod_matrix -> sub_vertices_group_size);

	 free(tmp_vec);

	 printf("leading eigenvalue is %f\n",((b / c) - (eigenpair -> mod_matrix -> norm_1)));

	 return ((b / c) - (eigenpair -> mod_matrix -> norm_1));
}

double* find_leading_eigenvector(modularity_matrix* mod_matrix){
	double* eigenvector;

	eigenvector = (double*)malloc(sizeof(double)*mod_matrix -> sub_vertices_group_size);
	power_iteration(mod_matrix, eigenvector);

	return eigenvector;
}

double* mult_matrix_by_vector(modularity_matrix* mod_matrix, double* vec, double* result_vec, int vec_size, bool to_shift){
	double result;

	double *vec_ptr;
	double *next_vec_ptr = result_vec;
	int curr_row = 0;

	for (vec_ptr=vec; vec_ptr < &vec[vec_size]; vec_ptr++) {
		result = calc_multiplication(mod_matrix, vec, curr_row, to_shift);
		/**vec_ptr = *next_vec_ptr;*/
		*next_vec_ptr = result;
		printf("result is %f\n",result);
		next_vec_ptr++;
		curr_row++;
 	}

	return result_vec;

}

double* calc_division_vector(leading_eigenpair* eigenpair) {

	double* division_vector;
	double* division_vector_ptr;
	double* eigen_vector_ptr;
	int i;

	division_vector = (double*)malloc(sizeof(double) * eigenpair -> mod_matrix -> sub_vertices_group_size);


	division_vector_ptr = division_vector;
	eigen_vector_ptr = eigenpair -> leading_eigenvector;

	for (i = 0; i < eigenpair -> mod_matrix -> sub_vertices_group_size; i++) {
		if (*eigen_vector_ptr > 0) {
			*division_vector_ptr = 1.0;
		}
		else {
			*division_vector_ptr = -1.0;
		}
		printf("division_vector[%d] = %f\n",i,*division_vector_ptr);
		division_vector_ptr++;
		eigen_vector_ptr++;
	}

	return division_vector;
}

void free_leading_eigenpair(leading_eigenpair* eigenpair) {

	free(eigenpair -> leading_eigenvector);
	free(eigenpair -> division_vector);


}



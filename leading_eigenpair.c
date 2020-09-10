
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "leading_eigenpair.h"

/* main function which calculates all eigenpair properties into eigenpair object */
leading_eigenpair* create_eigenpair(modularity_matrix* mod_matrix) {
	leading_eigenpair* eigenpair = (leading_eigenpair*)malloc(sizeof(leading_eigenpair));

	eigenpair -> mod_matrix = mod_matrix;
	eigenpair -> leading_eigenvector = find_leading_eigenvector(mod_matrix);
	eigenpair -> leading_eigenvalue = find_leading_eigenvalue(eigenpair);
	eigenpair -> division_vector = calc_division_vector(eigenpair);

	return eigenpair;
}

/* calculate leading eigenvector using power iteration process */
float* find_leading_eigenvector(modularity_matrix* mod_matrix){

	float* eigenvector;

	eigenvector = (float*)malloc(sizeof(float)*mod_matrix -> sub_vertices_group -> size);
	power_iteration(mod_matrix, eigenvector);

	return eigenvector;
}

/* calculate leading eigenvalue based on the leading eigenvector */
float find_leading_eigenvalue(leading_eigenpair* eigenpair){

	float numerator, denominator, *tmp_vec;
	unsigned int vec_size;

	vec_size = eigenpair -> mod_matrix -> sub_vertices_group ->size;
	tmp_vec = (float*)malloc(sizeof(float) * (vec_size));

	tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, eigenpair -> leading_eigenvector,
			tmp_vec, vec_size, true, true);
	numerator = float_dot_product(eigenpair -> leading_eigenvector, tmp_vec, vec_size);
	denominator = float_dot_product(eigenpair -> leading_eigenvector, eigenpair -> leading_eigenvector,
			vec_size);
	check_float_division_by_zero(denominator);

	free(tmp_vec);

	return ((numerator / denominator) - (eigenpair -> mod_matrix -> norm_1));
}

/* calcualte the vector which represents the division indices based on the leading eigenvector (s) */
float* calc_division_vector(leading_eigenpair* eigenpair) {

	float *division_vector, *division_vector_ptr, *eigen_vector_ptr;
	unsigned int i;

	division_vector = (float*)malloc(sizeof(float) * eigenpair -> mod_matrix -> sub_vertices_group -> size);
	division_vector_ptr = division_vector;
	eigen_vector_ptr = eigenpair -> leading_eigenvector;

	for (i = 0; i < eigenpair -> mod_matrix -> sub_vertices_group -> size; i++) {
		if (*eigen_vector_ptr > 0) {
			*division_vector_ptr = 1.0;
		}
		else {
			*division_vector_ptr = -1.0;
		}
		division_vector_ptr++;
		eigen_vector_ptr++;
	}

	return division_vector;
}

/* free memory of eigenpair object */
void free_leading_eigenpair(leading_eigenpair* eigenpair) {

	free(eigenpair -> leading_eigenvector);
	free(eigenpair);
}



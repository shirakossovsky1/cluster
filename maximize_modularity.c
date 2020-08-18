/*
 * maximize_modularity.c
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#include "maximize_modularity.h"

double calc_modularity_delta(leading_eigenpair* eigenpair) {

	double *tmp_vec;
    double b;

	tmp_vec = (double*)malloc(sizeof(double) * (eigenpair -> mod_matrix -> sub_vertices_group_size));

	tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, (double*)eigenpair -> division_vector,
			tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size);
	b = dot_product((double*)eigenpair -> division_vector, tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size);

	free(tmp_vec);

	return 0.5 * b;
}


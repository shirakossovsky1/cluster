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

    printf("division vec[0] is %f\n", eigenpair->division_vector[0]);

	tmp_vec = (double*)calloc(sizeof(double), (eigenpair -> mod_matrix -> sub_vertices_group_size));
	assert(tmp_vec != NULL);

	tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, eigenpair -> division_vector,
			tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size, false);
	b = dot_product(eigenpair -> division_vector, tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size);

	free(tmp_vec);

	printf("modularity_delta is %f\n", 0.5 * b);
	return 0.5 * b;
}

double improve_modularity(leading_eigenpair* eigenpair) {

	int* unmoved;
	int* unmoved_ptr;
	int i;
	int j;
	int k;
	double q0;
	double curr_q;
	double delta_q = 0.0;
	double* division_vector = eigenpair->division_vector;
	double* score;
	double* improve;
	int* indices;

	double max_score = 0.0;
	int max_score_vertex;
	int max_accomulative_improvement_vertex = 0.0;
	double max_accomulative_improvement;

	unmoved = (int*)calloc(sizeof(int), (eigenpair->mod_matrix->sub_vertices_group_size));
	unmoved_ptr = unmoved;

	score = (double*)calloc(sizeof(double), (eigenpair->mod_matrix->sub_vertices_group_size));

	improve = (double*)calloc(sizeof(double), (eigenpair->mod_matrix->sub_vertices_group_size));

	indices = (int*)calloc(sizeof(int), (eigenpair->mod_matrix->sub_vertices_group_size));

	for (i = 0; i < eigenpair->mod_matrix->sub_vertices_group_size; i++) {
		q0 = calc_modularity_delta(eigenpair);
		for (j = 0; j < eigenpair -> mod_matrix -> adjacency_matrix -> dim; j++) {
			if (*unmoved_ptr == 0) {
				division_vector[j] *= -1.0;
				curr_q = calc_modularity_delta(eigenpair);
				if (curr_q > max_score) {
					max_score = curr_q;
					max_score_vertex = j;
				}
				score[j] = curr_q - q0;
				division_vector[j] *= -1.0;
			}
		}

		division_vector[max_score_vertex] *= -1.0;
		indices[i] = max_score_vertex;

		if (i == 0) {
			improve[i] = score[max_score_vertex];
		}
		else {
			improve[i] = improve[i-1] + score[max_score_vertex];
		}

		if (improve[i] > max_accomulative_improvement) {
			max_accomulative_improvement = improve[i];
			max_accomulative_improvement_vertex = i;
		}

		unmoved[max_score_vertex] = 1;

	}
	/* remove back to original group the vertices that did not contribute to improvement */
	for (k = eigenpair->mod_matrix->sub_vertices_group_size; k > max_accomulative_improvement_vertex; k--) {
		division_vector[indices[k]] *= -1.0;
	}

	if (max_accomulative_improvement_vertex == (eigenpair->mod_matrix->sub_vertices_group_size -1)) {
		delta_q = 0.0;
	}
	else {
		delta_q = max_accomulative_improvement;
	}

	return delta_q;
}





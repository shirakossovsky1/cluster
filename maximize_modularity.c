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

    /*printf("division vec[0] is %f\n", eigenpair->division_vector[0]);*/

	tmp_vec = (double*)calloc(sizeof(double), (eigenpair -> mod_matrix -> sub_vertices_group_size));
	assert(tmp_vec != NULL);

	tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, eigenpair -> division_vector,
			tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size, false);
	b = dot_product(eigenpair -> division_vector, tmp_vec, eigenpair -> mod_matrix -> sub_vertices_group_size);

	free(tmp_vec);

	return 0.5 * b;
}

double improve_modularity(leading_eigenpair* eigenpair) {

	int* unmoved;
	int i;
	double delta_q = 0.0;
	int* indices;
	bool first;
	int q0;

	max_in_array *max_score, *max_improvemnt;

	double* division_vector = eigenpair->division_vector;
	int group_size = eigenpair -> mod_matrix -> sub_vertices_group_size;

	unmoved = (int*)calloc(sizeof(int), (eigenpair->mod_matrix->sub_vertices_group_size));
	max_score = (max_in_array*)malloc(sizeof(max_in_array));
	max_improvemnt = (max_in_array*)malloc(sizeof(max_in_array));
	max_improvemnt -> array = (double*)calloc(sizeof(double), (eigenpair->mod_matrix->sub_vertices_group_size));
	max_improvemnt -> max_value = 0.0;
	indices = (int*)calloc(sizeof(int), (eigenpair->mod_matrix->sub_vertices_group_size));

	first = true;

	for (i = 0; i < group_size; i++) {

		printf("iteration %d\n", i);

		q0 = calc_modularity_delta(eigenpair);

		find_maximal_score(max_score, group_size, unmoved, eigenpair, q0);

		printf("maximal score is %f with index %d\n", max_score->max_value, max_score->arg_max);

		/* moving the vertex with the maximal score to the other group */
		division_vector[max_score -> arg_max] *= -1.0;
		indices[i] = max_score -> arg_max;
		unmoved[max_score -> arg_max] = 1;

		/* store the accomulative improvemnt of the score in iteration i */
		if (i == 0) {
			max_improvemnt -> array[i] = max_score->array[max_score-> arg_max];
		}
		else {
			max_improvemnt -> array[i] = max_improvemnt -> array[i-1] + max_score->array[max_score -> arg_max];
		}

		printf("max improvement is %f with index %d\n", max_improvemnt -> array[i], i);

		/* store the max accomulative improvemnt */
		if (first || max_improvemnt -> array[i] > max_improvemnt -> max_value) {
			max_improvemnt -> max_value = max_improvemnt -> array[i];
			max_improvemnt -> arg_max = i;
		}
		first = false;

		/*printf("vertex with index %d in g was moved to the other group\n", max_score -> arg_max);*/

	}

	delta_q = calc_final_division(group_size, max_improvemnt, division_vector, indices);

	return delta_q;
}

void find_maximal_score(max_in_array *max_score, int group_size, int* unmoved, leading_eigenpair* eigenpair, int q0) {
	int j;
	double curr_q;
	double* division_vector;
	bool first;

	division_vector = eigenpair -> division_vector;
	first = true;

	max_score -> max_value = 0.0 - q0;

	max_score -> array = (double*)calloc(sizeof(double), (eigenpair->mod_matrix->sub_vertices_group_size));

	/* trying to move each vertex to the other group and store the vretex's score */
	for (j = 0; j < group_size; j++) {
		if (unmoved[j] == 0) {

			division_vector[j] *= -1.0;
			curr_q = calc_modularity_delta(eigenpair);
			printf("modularity delta - inside find max score %f\n", curr_q - q0);

			/* store the max score and the vertex with the maximal score (which is going to be moved) */
			if (first || curr_q - q0 >= max_score->max_value) {
				max_score -> max_value = curr_q - q0;
				max_score -> arg_max = j;
			}
			max_score -> array[j] = curr_q - q0;
			division_vector[j] *= -1.0;
			first = false;
		}
	}
	return;
}

double calc_final_division(int group_size, max_in_array *max_improvemnt, double *division_vector, int *indices) {
	double delta_q = 0.0;
	int k;

	/* remove back to original group the vertices that did not contribute to improvement */
	for (k = group_size-1; k > max_improvemnt -> arg_max; k--) {
		division_vector[indices[k]] *= -1.0;
	}

	/* handle case the division is exactly the same */
	if (max_improvemnt -> arg_max == (group_size -1)) {
		printf("%s\n", "division is exactly the same");
		delta_q = 0.0;
	}
	else {
		delta_q = max_improvemnt -> max_value;
	}

	printf("max_improvemnt -> arg_max is %d\n", max_improvemnt -> arg_max);

	return delta_q;
}





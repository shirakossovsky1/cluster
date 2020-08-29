/*
 * maximize_modularity.c
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#include "maximize_modularity.h"

float calc_modularity_delta(leading_eigenpair* eigenpair) {

	float product_result, *tmp_vec;
    int vec_size;

    vec_size = eigenpair->mod_matrix->sub_vertices_group ->size;

    /*printf("division vec[0] is %f\n", eigenpair->division_vector[0]);*/

	tmp_vec = (float*)calloc(sizeof(float),(vec_size));
	assert(tmp_vec != NULL);

	tmp_vec = mult_matrix_by_vector(eigenpair->mod_matrix, eigenpair->division_vector, tmp_vec, vec_size, false);
	product_result = dot_product(eigenpair->division_vector, tmp_vec, vec_size);

	free(tmp_vec);

	return 0.5 * product_result;
}

float improve_modularity(leading_eigenpair* eigenpair) {

	unsigned int i, group_size, *unmoved, *indices, *indices_ptr;
	float delta_q = 0.0, q0, *max_improvemnt_ptr, *division_vector;
	bool first;
	max_in_array *score, *improvement;

	division_vector = eigenpair->division_vector;
	group_size = eigenpair->mod_matrix->sub_vertices_group->size;

	unmoved = (unsigned int*)calloc(sizeof(unsigned int), group_size);
	score = (max_in_array*)malloc(sizeof(max_in_array));
	score->array = (float*)calloc(sizeof(float),group_size);
	improvement = (max_in_array*)malloc(sizeof(max_in_array));
	improvement->array = (float*)calloc(sizeof(float), group_size);
	improvement->max_value = 0.0;
	indices = (unsigned int*)calloc(sizeof(unsigned int), group_size);

	first = true;
	
	indices_ptr = indices;
	max_improvemnt_ptr = improvement->array;

	for (i = 0; i < group_size; i++) {

		/*printf("iteration %d\n", i);*/

		q0 = calc_modularity_delta(eigenpair);
		/*printf("q0 is %f\n", q0);*/

		find_maximal_score(score, group_size, unmoved, eigenpair, q0);

		/*printf("maximal score is %f with index %d\n", max_score->max_value, max_score->arg_max);*/

		/* moving the vertex with the maximal score to the other group */
		division_vector[score->arg_max] *= -1.0;
		/*printf("the %d vertex was moved\n", max_score->arg_max);*/
		*indices_ptr = score->arg_max;
		unmoved[score->arg_max] = 1;

		/* store the accomulative improvemnt of the score in iteration i */
		if (i == 0) {
			*max_improvemnt_ptr = score->array[score-> arg_max];
		}
		else {
			*max_improvemnt_ptr = *(max_improvemnt_ptr-1) + score->array[score->arg_max];
		}

		/*printf("max improvement is %f with index %d\n", max_improvemnt->array[i], i);*/

		/* store the max accomulative improvemnt */
		if (first || *max_improvemnt_ptr > improvement->max_value) {
			improvement->max_value = *max_improvemnt_ptr;
			improvement->arg_max = i;
		}
		first = false;
		
		indices_ptr++;
		max_improvemnt_ptr++;

		/*printf("vertex with index %d in g was moved to the other group\n", max_score->arg_max);*/

	}

	delta_q = calc_final_division(group_size, improvement, division_vector, indices);

	free(unmoved);
	free_max_in_array(score);
	free_max_in_array(improvement);
	free(indices);

	return delta_q;
}

void find_maximal_score(max_in_array *score, unsigned int group_size, unsigned int* unmoved, leading_eigenpair* eigenpair, float q0) {
	unsigned int j, *unmoved_ptr;
	float curr_q, *division_vector_ptr, *score_ptr;
	bool first = true;

	unmoved_ptr = unmoved;
	division_vector_ptr = eigenpair->division_vector;

	score->max_value = 0.0 - q0;
	score_ptr = score->array;

	/* trying to move each vertex to the other group and store the vretex's score */
	for (j = 0; j < group_size; j++) {
		if (*unmoved_ptr == 0) {

			*division_vector_ptr *= -1.0;
			curr_q = calc_modularity_delta(eigenpair);
			/*printf("modularity delta - inside find max score %f\n", curr_q - q0);*/

			/* store the max score and the vertex with the maximal score (which is going to be moved) */
			if (first || curr_q - q0 >= score->max_value) {
				score->max_value = curr_q - q0;
				score->arg_max = j;
			}
			*score_ptr = curr_q - q0;
			*division_vector_ptr *= -1.0;
			first = false;
		}
		unmoved_ptr++;
		division_vector_ptr++;
		score_ptr++;
		
	}
	return;
}

float calc_final_division(unsigned int group_size, max_in_array *improvement, float *division_vector, unsigned int *indices) {
	float delta_q = 0.0;
	unsigned int k, *indices_ptr;

	indices_ptr = indices + (group_size-1);

	/* remove back to original group the vertices that did not contribute to improvement */
	for (k = group_size-1; k > improvement->arg_max; k--) {
		division_vector[*indices_ptr] *= -1.0;
		indices_ptr--;
	}

	/* handle case the division is exactly the same */
	if (improvement->arg_max == (group_size -1)) {
		/*printf("%s\n", "division is exactly the same");*/
		delta_q = 0.0;
	}
	else {
		delta_q = improvement->max_value;
	}

	/*printf("max_improvemnt->arg_max is %d\n", max_improvemnt->arg_max);*/

	return delta_q;
}





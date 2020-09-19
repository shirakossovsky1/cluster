
#include "maximize_modularity.h"

/* allocate memory for all optimization set properties */
optimization_set* create_optimization_set(int group_size) {
	optimization_set *opt;

	opt = (optimization_set*)malloc(sizeof(optimization_set));
	opt->unmoved = (unsigned int*)calloc(sizeof(unsigned int), group_size);
	opt->score = (max_in_array*)malloc(sizeof(max_in_array));
	opt->score->array = (float*)calloc(sizeof(float),group_size);
	opt->improvement = (max_in_array*)malloc(sizeof(max_in_array));
	opt->improvement->array = (float*)calloc(sizeof(float), group_size);
	opt->improvement->max_value = 0.0;
	opt->indices = (unsigned int*)calloc(sizeof(unsigned int), group_size);

	return opt;
}

float calc_modularity_delta(leading_eigenpair* eigenpair) {

	float product_result, *tmp_vec;
    int vec_size;

    vec_size = eigenpair->mod_matrix->sub_vertices_group ->size;

	tmp_vec = (float*)calloc(sizeof(float),(vec_size));

	tmp_vec = mult_matrix_by_vector(eigenpair->mod_matrix, eigenpair->division_vector, tmp_vec, vec_size, false, true);
	product_result = float_dot_product(eigenpair->division_vector, tmp_vec, vec_size);

	free(tmp_vec);

	return 0.5 * product_result;
}

/* main function which optimizes the modularity of given division */
float improve_modularity(leading_eigenpair* eigenpair) {

	unsigned int i, group_size, *indices_ptr;
	float delta_q = 0.0, *max_improvemnt_ptr, *division_vector;
	bool first = true;
	optimization_set *opt;

	division_vector = eigenpair->division_vector;
	group_size = eigenpair->mod_matrix->sub_vertices_group->size;

	/*printf("%s","sub group is ");
	for (i = 0; i < group_size; i++) {
		printf("%d ",eigenpair->mod_matrix->sub_vertices_group->array[i]);
	}
	printf("%s\n"," ");

	printf("%s","division vector is ");
	for (i = 0; i < group_size; i++) {
		printf("%f ",division_vector[i]);
	}
	printf("%s\n"," ");*/

	opt = create_optimization_set(group_size);

	indices_ptr = opt->indices;
	max_improvemnt_ptr = opt->improvement->array;

	/* fill score array with modularity values for each vertex potential group change */
	for (i = 0; i < group_size; i++) {
		if (first) {
			calc_score_first_iteration(opt->score, group_size, eigenpair);
		}
		else {
			calc_score(opt->score, group_size, opt->unmoved, eigenpair);
		}

		/* moving the vertex with the maximal score to the other group */
		division_vector[opt->score->arg_max] *= -1.0;
		*indices_ptr = opt->score->arg_max;
		opt->unmoved[opt->score->arg_max] = 1;

		/* store the accomulative improvemnt of the score in iteration i */
		if (i == 0) {
			*max_improvemnt_ptr = opt->score->array[opt->score-> arg_max];
		}
		else {
			*max_improvemnt_ptr = *(max_improvemnt_ptr-1) + opt->score->array[opt->score->arg_max];
		}

		/* store the max accomulative improvemnt */
		if (first || *max_improvemnt_ptr >= opt->improvement->max_value) {
			opt->improvement->max_value = *max_improvemnt_ptr;
			opt->improvement->arg_max = i;
			first = false;
		}
		
		indices_ptr++;
		max_improvemnt_ptr++;
	}

	/*printf("%s","improvement array is ");
	for (i = 0; i < group_size; i++) {
		printf("%f ",opt->improvement->array[i]);
	}
	printf(", arg max is %f \n",opt->improvement->max_value);*/

	delta_q = calc_final_division(group_size, opt->improvement, division_vector, opt->indices);

	free_optimization_set(opt);

	return delta_q;
}

/* fill score array values for the first iteration */
void calc_score_first_iteration(max_in_array *score, unsigned int group_size, leading_eigenpair* eigenpair) {

	unsigned int j, *sub_degrees_vec_ptr;
	float *division_vector_ptr, *score_ptr, *mult_result, *tmp_vec_ptr;
	bool first = true;

	division_vector_ptr = eigenpair->division_vector;
	sub_degrees_vec_ptr = eigenpair->mod_matrix->sub_degrees_vector;

	score->max_value = 0.0;
	score_ptr = score->array;

	mult_result = (float*)calloc(sizeof(float),(group_size));
	mult_result = mult_matrix_by_vector(eigenpair->mod_matrix, eigenpair->division_vector, mult_result, group_size, false, false);
	tmp_vec_ptr = mult_result;

	for (j = 0; j < group_size; j++) {
		*score_ptr = -2.0 * (*division_vector_ptr * *tmp_vec_ptr + ((float)*sub_degrees_vec_ptr * (float)*sub_degrees_vec_ptr / (float)eigenpair->mod_matrix->total_degrees_num));

		/* store the max score and the vertex with the maximal score (which is going to be moved) */
		if (first || *score_ptr >= score->max_value) {
			score->max_value = *score_ptr;
			score->arg_max = j;
			first = false;
		}

		division_vector_ptr++;
		tmp_vec_ptr++;
		sub_degrees_vec_ptr++;
		score_ptr++;
	}

	/*printf("%s","score array is ");
	for (j = 0; j < group_size; j++) {
		printf("%f ",score->array[j]);
	}
	printf(", arg max is %f\n",score->max_value);*/

	free(mult_result);

	return;
}

/* fill score array values for all the rest iterations */
void calc_score(max_in_array *score, unsigned int group_size, unsigned int* unmoved, leading_eigenpair* eigenpair) {

	unsigned int j, row, col, curr_arg_max, *unmoved_ptr;
	float s_k, *division_vector_ptr, *score_ptr;
	bool first = true;

	unmoved_ptr = unmoved;
	division_vector_ptr = eigenpair->division_vector;
	score_ptr = score->array;

	curr_arg_max = score->arg_max;
	s_k = eigenpair->division_vector[curr_arg_max];
	for (j = 0; j < group_size; j++) {

		/* update score array */
		if (j == curr_arg_max) {
			*score_ptr *= -1.0;
		}
		else if (*unmoved_ptr == 0){
			row = eigenpair->mod_matrix->sub_vertices_group->array[j];
			col = eigenpair->mod_matrix->sub_vertices_group->array[curr_arg_max];
			*score_ptr -= 4.0 * *division_vector_ptr * s_k * calc_mod_matrix_cell(eigenpair->mod_matrix, row, col, 0, false);
			/*printf("matrix cell (%d,%d) is %f\n",row, col, calc_mod_matrix_cell(eigenpair->mod_matrix, row, col, 0, false));*/

			/* restore the max score and the vertex with the maximal score (which is going to be moved) */
			if (first || *score_ptr >= score->max_value) {
				score->max_value = *score_ptr;
				score->arg_max = j;
				first = false;
			}
		}

		division_vector_ptr++;
		score_ptr++;
		unmoved_ptr++;
	}
	/*printf("%s","score array is ");
	for (j = 0; j < group_size; j++) {
		printf("%f ",score->array[j]);
	}
	printf(", arg max is %f\n",score->max_value);*/

	return;
}

/* update the division vector based on the optimal division */
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
		delta_q = 0.0;
	}
	else {
		delta_q = improvement->max_value;
	}

	return delta_q;
}

/* free memory of optimization set object */
void free_optimization_set(optimization_set *opt) {

	free(opt->unmoved);
	free_max_in_array(opt->score);
	free_max_in_array(opt->improvement);
	free(opt->indices);
	free(opt);
}



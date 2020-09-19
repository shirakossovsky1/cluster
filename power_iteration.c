
#include "power_iteration.h"

#define MAX_ITERATIONNS() (1000000)
#define EPSILON 0.0001

/* main function which executes the process of power iteration */
void power_iteration(modularity_matrix* mod_matrix, float* eigen_vector) {

	float *vec_0, *next_vec, *curr_vec;
	float *curr_vec_ptr, *next_vec_ptr, *eigen_vector_ptr;
	bool first = true;
	unsigned int i = 0, num_rows;

	/* initializing random vector and properties */
	num_rows = mod_matrix->sub_vertices_group ->size;

	printf("%s\n", "starting power iteration");

	vec_0 = (float*) calloc(num_rows, sizeof(float));
	curr_vec = (float*) calloc(num_rows, sizeof(float));

	eigen_vector_ptr = eigen_vector;
	next_vec = eigen_vector;
	curr_vec_ptr = curr_vec;

	vec_0 = generate_rand_vec(vec_0,num_rows);
	get_next_vec(mod_matrix, vec_0, next_vec, num_rows);

	printf("%s\n", "starting while");
	/* run the iterative process until the change in the new vector is small enough */
	while (first || ((smaller_than_eps(curr_vec, next_vec, num_rows)) != 1 && (i < MAX_ITERATIONNS()))) {

		first = false;
		curr_vec_ptr = curr_vec;
		next_vec_ptr = next_vec;

		/*copy next_vec into curr_vec*/
		for (next_vec_ptr=next_vec ; next_vec_ptr <= &next_vec[num_rows-1] ; next_vec_ptr++){
				*curr_vec_ptr = *next_vec_ptr;
				curr_vec_ptr++;
			}

		get_next_vec(mod_matrix, curr_vec, next_vec, num_rows);
		i++;
	}

	printf("i = %d\n", i);

	printf("%s\n", "finishing while");
	next_vec_ptr = next_vec;

	/* copy the final vector values into the eigenvector */
	for (next_vec_ptr=next_vec ; next_vec_ptr <= &next_vec[num_rows-1] ; next_vec_ptr++){
			*eigen_vector_ptr = *next_vec_ptr;
			eigen_vector_ptr++;
		}

	free(vec_0);
	free(curr_vec);

	printf("%s\n", "finishing power iteration");

	return;

}

/* generate the first vector to start the process */
float* generate_rand_vec(float* vec_0, unsigned int vec_size) {

	float* vec_0_ptr;
	unsigned int i=0;

	for (vec_0_ptr=vec_0 ; vec_0_ptr < &vec_0[vec_size] ; vec_0_ptr++){
		*vec_0_ptr = rand()%10;
		i++;
	}

	return vec_0;
}

/* check wether the change between the current and the previous vectors is small enough */
unsigned int smaller_than_eps(float* vec, float* next_vec, unsigned int vec_size) {

	float *vec_ptr, *next_vec_ptr;

	next_vec_ptr = next_vec;

	for (vec_ptr=vec; vec_ptr <= &vec[vec_size-1]; vec_ptr++) {
		if (fabs(*next_vec_ptr - *vec_ptr) >= EPSILON) {
			return 0;
		}
		next_vec_ptr++;
	}
	return 1;
}

/* main calculation for the iterative process */
void get_next_vec(modularity_matrix* mod_matrix, float* vec, float* next_vec,
		unsigned int vec_size){

	float result, vec_norm = 0.0, *next_vec_ptr;
	unsigned int curr_row = 0;

	next_vec_ptr = next_vec;

	for (curr_row = 0; curr_row < vec_size ; curr_row++) {
		result = mult_modularity_mat_row_by_vec(mod_matrix, vec, curr_row, true, true);

		vec_norm += result * result;
		*next_vec_ptr = result;
		next_vec_ptr++;
 	}

	vec_norm = sqrt(vec_norm);
	check_float_division_by_zero(vec_norm);

	for (next_vec_ptr=next_vec; next_vec_ptr <= &next_vec[vec_size-1]; next_vec_ptr++){
		*next_vec_ptr = *(next_vec_ptr) / vec_norm;
	}

	return;
}

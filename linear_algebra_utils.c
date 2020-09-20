
#include "linear_algebra_utils.h"

/* compute multiplication of a matrix by vector */
float* mult_matrix_by_vector(modularity_matrix* mod_matrix, float* vec, float* result_vec, unsigned int vec_size, bool to_shift, bool to_hat){

	float result, *vec_ptr, *next_vec_ptr = result_vec;
	unsigned int curr_row = 0;

	for (vec_ptr=vec; vec_ptr < &vec[vec_size]; vec_ptr++) {
		result = mult_modularity_mat_row_by_vec(mod_matrix, vec, curr_row, to_shift, to_hat);
		*next_vec_ptr = result;
		next_vec_ptr++;
		curr_row++;
 	}

	return result_vec;

}

/* compute multiplication of a matrix by vector */
float* mult_sparse_matrix_by_vector(modularity_matrix* mod_matrix, float* vec, float* result_vec, unsigned int vec_size){

	float result, *vec_ptr, *next_vec_ptr = result_vec;
	unsigned int curr_row = 0, row_in_original_matrix;

	for (vec_ptr=vec; vec_ptr < &vec[vec_size]; vec_ptr++) {
		row_in_original_matrix = mod_matrix->sub_vertices_group->array[curr_row];
		result = mult_sparse_mat_row_by_vec(mod_matrix, vec, row_in_original_matrix);
		*next_vec_ptr = result;

		next_vec_ptr++;
		curr_row++;
 	}

	return result_vec;

}

/* compute multiplication of a modularity matrix by vector */
float mult_modularity_mat_row_by_vec(modularity_matrix* mod_matrix, float *vec, unsigned int row, bool to_shift, bool to_hat){

	float sparse_product, degrees_product, diagonal_product = 0.0, result, *vec_ptr;
	unsigned int row_in_original_matrix;

	row_in_original_matrix = mod_matrix->sub_vertices_group->array[row];

	vec_ptr = vec;
	sparse_product = mult_sparse_mat_row_by_vec(mod_matrix, vec_ptr, row_in_original_matrix);

	vec_ptr = vec;
	degrees_product = (float)(mod_matrix->degrees_vector[row_in_original_matrix]) * int_dot_product(mod_matrix->sub_degrees_vector, vec_ptr, mod_matrix->sub_vertices_group ->size)
			/ (float)mod_matrix->total_degrees_num;

	vec_ptr = vec;
	if (to_hat && to_shift) {
		diagonal_product = vec_ptr[row] * ((mod_matrix->norm_1) - mod_matrix->vertices_mod_vec[row]);
	}
	else if (to_hat){
		diagonal_product = -1.0 * (vec_ptr[row] * (mod_matrix->vertices_mod_vec[row]));
	}

	result = sparse_product - degrees_product + diagonal_product;
	return result;
}

/* compute multiplication of a sparse matrix by vector */
float mult_sparse_mat_row_by_vec(modularity_matrix* mod_matrix, float *vec, unsigned int row){

	float result = 0.0, *vec_ptr;
	unsigned int *sub_vertices_group_ptr; /* pointer for g */
	node *neighbor_ptr;

	vec_ptr = vec;
	sub_vertices_group_ptr = mod_matrix->sub_vertices_group->array;
	neighbor_ptr = mod_matrix->adjacency_matrix->rows[row].head;

	while (sub_vertices_group_ptr <= &mod_matrix->sub_vertices_group->array[(mod_matrix->sub_vertices_group ->size)-1] &&
			neighbor_ptr != NULL) {

		if (neighbor_ptr->data < *sub_vertices_group_ptr) {
			neighbor_ptr = neighbor_ptr->next;
		}
		else if (neighbor_ptr->data > *sub_vertices_group_ptr) {
			sub_vertices_group_ptr++;
			vec_ptr++;
		}
		else {
			result += (float)*vec_ptr;
			sub_vertices_group_ptr++;
			vec_ptr++;
			neighbor_ptr = neighbor_ptr->next;
		}
	}
	return result;
}

/* compute dot product of 2 vectors with float values */
float float_dot_product(float* vector_1, float* vector_2, unsigned int vec_size){

	float result = 0.0, *vector_1_ptr, *vector_2_ptr;
	unsigned int i;

	vector_1_ptr = vector_1;
	vector_2_ptr = vector_2;

	for (i = 0; i < vec_size; i++) {
		result += (float)*vector_1_ptr * *vector_2_ptr;
		vector_1_ptr++;
		vector_2_ptr++;
	}

	return result;
}

/* compute dot product of 2 vectors with int values */
float int_dot_product(unsigned int* vector_1, float* vector_2, unsigned int vec_size){

	float result = 0.0, *vector_2_ptr;
	unsigned int i, *vector_1_ptr;

	vector_1_ptr = vector_1;
	vector_2_ptr = vector_2;

	for (i = 0; i < vec_size; i++) {
		result += (float)(*vector_1_ptr) * *vector_2_ptr;
		vector_1_ptr++;
		vector_2_ptr++;
	}

	return result;
}

/* compute multiplication of two vectors, value by value */
float* mult_vec_by_vec_and_shift(float* vector_1, float* vector_2, float* result_vec, unsigned int vec_size, float shift_value) {

	unsigned int i;
	float *result_vec_ptr, *vector_1_ptr, *vector_2_ptr;

	vector_1_ptr = vector_1;
	vector_2_ptr = vector_2;
	result_vec_ptr = result_vec;

	for (i = 0; i < vec_size; i++) {
		*result_vec_ptr = (*vector_1_ptr - shift_value) * *vector_2_ptr;
		vector_1_ptr++;
		vector_2_ptr++;
		result_vec_ptr++;
	}

	return result_vec;
}



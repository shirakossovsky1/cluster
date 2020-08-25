/*
 * power_iteration.c
 *
 *  Created on: 10 Apr 2020
 *      Author: shira
 */


#include "power_iteration.h"


void power_iteration(modularity_matrix* mod_matrix, double* eigen_vector) {

	double* vec_0;
	double* next_vec;
	double* curr_vec;
	bool first = true;

	double *curr_vec_ptr, *next_vec_ptr, *eigen_vector_ptr;

	int num_rows;

	printf("%s\n","&&&&&&&&& starting power interation file &&&&&&&&&&");

	num_rows = mod_matrix -> sub_vertices_group_size;

	vec_0 = (double*) calloc(num_rows, sizeof(double));
	curr_vec = (double*) calloc(num_rows, sizeof(double));

	eigen_vector_ptr = eigen_vector;
	next_vec = eigen_vector;

	curr_vec_ptr = curr_vec;

	printf("%s\n","creating b0");
	vec_0 = generate_rand_vec(vec_0,num_rows);
	/*vec_0[0] = 1.0;
	vec_0[1] = 1.0;
	vec_0[2] = 1.0;*/

	/*copy b0 into curr_bk vector
	for (vec_0_ptr=vec_0 ; vec_0_ptr < &vec_0[num_rows] ; vec_0_ptr++){
		*curr_vec_ptr = *vec_0_ptr;
		curr_vec_ptr++;
	}
	vec_0_ptr = vec_0;*/

	printf("%s\n","get next bk first");
	get_next_vec(mod_matrix, vec_0, next_vec, mod_matrix->sub_vertices_group_size);

	printf("%s\n","starting iterative power iteration");
	while (first || (smaller_than_eps(curr_vec, next_vec, num_rows) != 1)) {
		first = false;
		curr_vec_ptr = curr_vec;
		next_vec_ptr = next_vec;
		/*copy next_vec into curr_vec*/
		for (next_vec_ptr=next_vec ; next_vec_ptr <= &next_vec[num_rows-1] ; next_vec_ptr++){
				*curr_vec_ptr = *next_vec_ptr;
				curr_vec_ptr++;
			}
		get_next_vec(mod_matrix, curr_vec, next_vec, mod_matrix->sub_vertices_group_size);
	}

	next_vec_ptr = next_vec;

	printf("\n%s","eigenvector vector is ");
	for (next_vec_ptr=next_vec ; next_vec_ptr <= &next_vec[num_rows-1] ; next_vec_ptr++){
			*eigen_vector_ptr = *next_vec_ptr;
			printf("%f, ",*eigen_vector_ptr);
			eigen_vector_ptr++;
		}
	printf("\n");

	free(vec_0);
	free(curr_vec);

	printf("%s\n","&&&&&&&&& finishing power interation file &&&&&&&&&&");

	return;

}

double* generate_rand_vec(double* vec_0, int vec_size) {
	double* vec_0_ptr;

	printf("\n%s","vec 0 is ");
	for (vec_0_ptr=vec_0 ; vec_0_ptr < &vec_0[vec_size] ; vec_0_ptr++){
		*vec_0_ptr = rand()%10;
		printf("%f, ",*vec_0_ptr);
	}
	printf("\n");

	return vec_0;

}


int smaller_than_eps(double* vec, double* next_vec, int vec_size) {
	double *vec_ptr ;
	double *next_vec_ptr = next_vec;
	double epsilon = 0.00001;

	for (vec_ptr=vec; vec_ptr <= &vec[vec_size-1]; vec_ptr++) {
		/*printf("diff is %f\n",fabs(*next_vec_ptr - *vec_ptr));*/
		if (fabs(*next_vec_ptr - *vec_ptr) >= epsilon) {
			return 0;
		}
		next_vec_ptr++;
	}
	return 1;
}

void get_next_vec(modularity_matrix* mod_matrix, double* vec, double* next_vec, int vec_size){
	double result;
	double vec_norm = 0.0;
	int curr_row = 0;

	double *next_vec_ptr = next_vec;

	for (curr_row = 0; curr_row < vec_size ; curr_row++) {
		result = calc_multiplication(mod_matrix, vec, curr_row, true);

		vec_norm += result * result;
		/**vec_ptr = *next_vec_ptr;*/
		*next_vec_ptr = result;
		next_vec_ptr++;
 	}
	vec_norm = sqrt(vec_norm);

	printf("norm is %f\n",vec_norm);

	/*printf("\n%s","vector is ");*/
	for (next_vec_ptr=next_vec; next_vec_ptr <= &next_vec[vec_size-1]; next_vec_ptr++){
		*next_vec_ptr = *(next_vec_ptr)/vec_norm;
		/*printf("%f, ",*next_vec_ptr);*/
	}
	/*printf("\n");*/
	return;

}

/* calculates iteration of Bg hat * vector (power_iteration) */
double calc_multiplication(modularity_matrix* mod_matrix, double *vec, int row, bool to_shift){
	double result;
	double a;
	double b;
	/*double c;*/
	double d;
	double *vec_ptr;
	int row_in_original_matrix;

	row_in_original_matrix = mod_matrix->sub_vertices_group[row];


	vec_ptr = vec;
	/*printf("row is = %d\n",row);
	printf("row_in_original_matrix is = %d\n",row_in_original_matrix);*/
	/*printf("1 - vec_ptr[1] = %f\n",vec_ptr[1]);
	printf("1 - vec_ptr[2] = %f\n",vec_ptr[2]);
	printf("to_shift = %d\n",to_shift);
	printf("(power iteration) sub_degrees_vector[0] = %d\n",mod_matrix -> sub_degrees_vector[0]);

	printf("vec_ptr[0] = %f\n",vec_ptr[0]);*/
	a = mult_sparse_mat_row_by_vec(mod_matrix, vec_ptr, row_in_original_matrix);

	vec_ptr = vec;
	/*printf("2 - vec_ptr[0] = %f\n",vec_ptr[0]);
	printf("2 - vec_ptr[1] = %f\n",vec_ptr[1]);
	printf("2 - vec_ptr[2] = %f\n",vec_ptr[2]);*/
	b = (double)(mod_matrix->degrees_vector[row_in_original_matrix]) * int_dot_product(mod_matrix -> sub_degrees_vector, vec_ptr, mod_matrix -> sub_vertices_group_size)
			/ (double)mod_matrix->total_degrees_num;
	/*c = mod_matrix->vertices_mod_vec[row] * sum_vec(vec, mod_matrix -> sub_vertices_group_size);*/

	vec_ptr = vec;
	/*printf("3 - vec_ptr[0] = %f\n",vec_ptr[0]);
	printf("3 - vec_ptr[1] = %f\n",vec_ptr[1]);
	printf("3 - vec_ptr[2] = %f\n",vec_ptr[2]);*/
	if (to_shift) {
		/*printf("%s\n","shifting");*/
		d = vec_ptr[row] * ((mod_matrix -> norm_1) - mod_matrix->vertices_mod_vec[row_in_original_matrix]);
	}
	else {
		/*printf("vec_ptr[row] = %f\n",vec_ptr[row]);
		printf("mod_matrix->vertices_mod_vec[row] = %f\n",mod_matrix->vertices_mod_vec[row]);*/
		d = -1.0 * (vec_ptr[row] * (mod_matrix->vertices_mod_vec[row_in_original_matrix]));
	}

	result = a - b + d;

	/*printf("a is %f, ",a);
	printf("b is %f, ",b);
	printf("d is %f, ",d);*/

	return result;
	
	
}

/* Multiplies matrix A by vector v, into result (result is pre-allocated), with link-list */
double mult_sparse_mat_row_by_vec(modularity_matrix* mod_matrix, double *vec, int row){

	double 			result = 0.0;
	node 			*neighbor_ptr;
	int				*sub_vertices_group_ptr; /* pointer for g */
	double				*vec_ptr;

	vec_ptr = vec;
	sub_vertices_group_ptr = mod_matrix -> sub_vertices_group;

	neighbor_ptr = mod_matrix -> adjacency_matrix -> rows[row].head;

	while (*sub_vertices_group_ptr <= mod_matrix -> sub_vertices_group[mod_matrix -> sub_vertices_group_size-1] &&
			neighbor_ptr != NULL) {

		/*printf("curr neighbor is %d\n ",neighbor_ptr->matrix_index);
		printf("vertex in g is %d\n ",*sub_vertices_group_ptr);
		printf("vec_ptr is %f\n ",*vec_ptr);
		printf("double vec_ptr is %f\n ",(double)*vec_ptr);*/


		if (neighbor_ptr -> matrix_index < *sub_vertices_group_ptr) {
			neighbor_ptr = neighbor_ptr -> next;
		}

		else if (neighbor_ptr -> matrix_index > *sub_vertices_group_ptr) {
			sub_vertices_group_ptr++;
			vec_ptr++;
		}

		else {

			result += (double)*vec_ptr;
			/*printf("neighbor in g, result =  %f\n, ",result);*/
			sub_vertices_group_ptr++;
			vec_ptr++;
			neighbor_ptr = neighbor_ptr -> next;
		}
	}

	return result;
}

/* Multiplies matrix A by vector v, into result (result is pre-allocated), with link-list */
/*double mult_sparse_mat_by_vec(modularity_matrix* mod_matrix, double *vec){
	

}*/

double dot_product(double* vector_1, double* vector_2, int vec_size){
	double result = 0.0;
	double *vector_1_ptr;
	double *vector_2_ptr;
	int i;

	vector_1_ptr = vector_1;
	vector_2_ptr = vector_2;

	for (i = 0; i < vec_size; i++) {
		result += (double)*vector_1_ptr * *vector_2_ptr;
		/*printf("vector_1_ptr[%d] = %f, vector_2_ptr[%d] = %f\n",i, *vector_1_ptr, i, *vector_2_ptr);*/
		vector_1_ptr++;
		vector_2_ptr++;
	}
	/*printf("result of dot product =  %f\n ",result);*/
	return result;
}

double int_dot_product(int* vector_1, double* vector_2, int vec_size){
	double result = 0.0;
	int *vector_1_ptr;
	double *vector_2_ptr;
	int i;

	vector_1_ptr = vector_1;
	vector_2_ptr = vector_2;

	for (i = 0; i < vec_size; i++) {
		result += (double)(*vector_1_ptr) * *vector_2_ptr;
		vector_1_ptr++;
		vector_2_ptr++;
	}

	return result;
}

double sum_vec(double* vector, int vec_size){
	double result = 0.0;
	double *vector_ptr;
	int i;

	vector_ptr = vector;

	for (i = 0; i < vec_size; i++) {
		result += (double)*vector_ptr;
		vector_ptr++;
	}

	return result;
}










/*
 * power_iteration.c
 *
 *  Created on: 10 Apr 2020
 *      Author: shira
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "modularity_matrix.h"
#include "power_iteration.h"


void power_iteration(modularity_matrix* mod_matrix, double* eigen_vector) {

	double* vec_0;
	double* next_vec;
	double* curr_vec;
	double* curr_row;

	double *curr_vec_ptr,*vec_0_ptr;

	int num_rows;
	int i = 0;
	clock_t	start_time, end_time;

	num_rows = mod_matrix -> sub_vertices_group_size;

	vec_0 = (double*) calloc(num_rows, sizeof(double));
	curr_vec = (double*) calloc(num_rows, sizeof(double));
	curr_row = (double*) calloc(num_rows, sizeof(double));

	next_vec = eigen_vector;

	curr_vec_ptr = curr_vec;

	vec_0 = generate_rand_vec(vec_0,num_rows);

	/* copy b0 into curr_bk vector */
	for (vec_0_ptr=vec_0 ; vec_0_ptr < &vec_0[num_rows] ; vec_0_ptr++){
		*curr_vec_ptr = *vec_0_ptr;
		curr_vec_ptr++;
	}
	vec_0_ptr = vec_0;

	get_next_vec(mod_matrix, vec_0, next_vec, curr_row , mod_matrix->sub_vertices_group_size);

	while (smaller_than_eps(curr_vec, next_vec, num_rows) != 1) {
		get_next_vec(mod_matrix, curr_vec, next_vec, curr_row , mod_matrix->sub_vertices_group_size);
	}

	free(vec_0);
	free(curr_vec);
	free(curr_row);

	return;

}

double* generate_rand_vec(double* vec_0, int vec_size) {
	double* vec_0_ptr;

	for (vec_0_ptr=vec_0 ; vec_0_ptr < &vec_0[vec_size] ; vec_0_ptr++){
		*vec_0_ptr = rand()%10;
	}

	return vec_0;

}


int smaller_than_eps(double* vec, double* next_vec, int vec_size) {
	double *vec_ptr ;
	double *next_vec_ptr = next_vec;
	double epsilon = 0.00001;

	for (vec_ptr=vec; vec_ptr < &vec[vec_size]; vec_ptr++) {
		if (fabs(*next_vec_ptr - *vec_ptr) >= epsilon) {
			return 0;
		}
		next_vec_ptr++;
	}
	return 1;
}

void get_next_vec(modularity_matrix* mod_matrix, double* vec, double* next_vec, double* curr_row , int vec_size){
	double result;
	double vec_norm = 0;

	double *vec_ptr;
	double *next_vec_ptr = next_vec;

	for (vec_ptr=vec; vec_ptr < &vec[vec_size]; vec_ptr++) {
		result = calc_multiplication(mod_matrix, vec, curr_row);
		*vec_ptr = *next_vec_ptr;
		vec_norm += result*result;
		*next_vec_ptr = result;
		next_vec_ptr++;
 	}
	vec_norm = sqrt(vec_norm);

	for (next_vec_ptr=next_vec; next_vec_ptr < &next_vec[vec_size]; next_vec_ptr++){
		*next_vec_ptr = *next_vec_ptr/vec_norm;
	}
	return;

}

/* calculates iteration of Bg hat * vector (power_iteration) */
double calc_multiplication(modularity_matrix* mod_matrix, double *vec, int row){
	double result;
	double a;
	double b;
	double c;
	double d;
	
	a = mult_sparse_mat_row_by_vec(mod_matrix, vec, row);
	b = dot_product(mod_matrix -> sub_degrees_vector, vec, mod_matrix -> sub_vertices_group_size);
	c = mod_matrix->vertices_mod_vec[row] * sum_vec(vec, mod_matrix -> sub_vertices_group_size);
	d = vec[row] * (mod_matrix -> norm_1);

	result = a - b -c + d;

	return result;
	
	
}

/* Multiplies matrix A by vector v, into result (result is pre-allocated), with link-list */
double mult_sparse_mat_row_by_vec(modularity_matrix* mod_matrix, double *vec, int row){

	int 			j = 0;
	double 			result = 0.0;
	node 			*neighbor_ptr;
	int				*sub_vertices_group_ptr; /* pointer for g */
	int				*vec_ptr;

	vec_ptr = vec;

	neighbor_ptr = mod_matrix -> adjacency_matrix -> rows[row] -> head;

	while (*sub_vertices_group_ptr <= mod_matrix -> sub_vertices_group[mod_matrix -> sub_vertices_group_size] &&
			neighbor_ptr != NULL) {

		if (neighbor_ptr -> matrix_index < *sub_vertices_group_ptr) {
			neighbor_ptr = neighbor_ptr -> next;
		}

		else if (neighbor_ptr -> matrix_index > *sub_vertices_group_ptr) {
			sub_vertices_group_ptr++;
			vec_ptr++;
		}

		else {
			result += *vec_ptr;
			sub_vertices_group_ptr++;
			vec_ptr++;
			neighbor_ptr = neighbor_ptr -> next;
		}
	}

	return result;
}

/* Multiplies matrix A by vector v, into result (result is pre-allocated), with link-list */
double mult_sparse_mat_by_vec(modularity_matrix* mod_matrix, double *vec){
	

}

double dot_product(int* vector_1, double* vector_2, int vec_size){
	double result;
	int *vector_1_ptr;
	double *vector_2_ptr;
	int i;

	for (i = 0; i < vec_size; i++) {
		result += *vector_1_ptr * *vector_2_ptr;
		vector_1_ptr++;
		vector_2_ptr++;
	}

	return result;
}

double sum_vec(double* vector, int vec_size){
	double result;
	double *vector_ptr;
	int i;

	for (i = 0; i < vec_size; i++) {
		result += *vector_ptr;
		vector_ptr++;
	}

	return result;
}










/*
 * modularity_matrix.c
 *
 *  Created on: 14 Aug 2020
 *      Author: shira
 */
#define EXIT_FAILURE    1

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "modularity_matrix.h"

modularity_matrix* create_modularity_matrix(sparse_matrix *adjacency_matrix, int* sub_vertices_group, int group_size) {

	modularity_matrix* mod_matrix = (modularity_matrix*)malloc(sizeof(modularity_matrix));

	/* address the case that sum of degrees in the graph (M) is zero */
	if (adjacency_matrix -> total_degrees == 0) {
		return mod_matrix;
	}

	mod_matrix -> total_degrees_num = adjacency_matrix -> total_degrees;
	mod_matrix -> degrees_vector = adjacency_matrix -> degrees_vector;
	mod_matrix -> sub_degrees_vector = calc_sub_degrees_vector(sub_vertices_group, mod_matrix);
	mod_matrix -> adjacency_matrix = adjacency_matrix;
	mod_matrix -> sub_vertices_group = sub_vertices_group;
	mod_matrix -> sub_vertices_group_size = group_size;
	mod_matrix -> vertices_mod_vec = calc_vertices_mod_vec(mod_matrix);
	mod_matrix -> norm_1 = calc_norm_1(mod_matrix);

	return mod_matrix;
}

int* calc_sub_degrees_vector(int* sub_vertices_group, modularity_matrix* mod_matrix) {
	int* sub_degrees_vector = (int*)calloc(mod_matrix -> sub_vertices_group_size, sizeof(int));

	int* sub_vertices_group_ptr;
	int* sub_degrees_vector_ptr;
	int* degrees_vector_ptr;
	int i;

	sub_vertices_group_ptr = sub_vertices_group;
	sub_degrees_vector_ptr = sub_degrees_vector;
	degrees_vector_ptr = mod_matrix -> degrees_vector;

	for (i = 0; i < *(sub_vertices_group + mod_matrix->sub_vertices_group_size -1); i++) {
		if (i == *(sub_vertices_group_ptr)) {
			*sub_degrees_vector_ptr = *degrees_vector_ptr;
			sub_vertices_group_ptr++;
			sub_degrees_vector_ptr++;
		}
		degrees_vector_ptr++;
	}
	return sub_degrees_vector;
}

double* calc_vertices_mod_vec(modularity_matrix* mod_matrix) {
	int vertex;
	int total_expected_degrees;
	int total_subgroup_neighbors;
	double* vertices_mod_vec = (double*)calloc(mod_matrix -> sub_vertices_group_size, sizeof(double));
	int* sub_vertices_group_ptr = mod_matrix -> sub_vertices_group;
	double* vertices_mod_vec_ptr = vertices_mod_vec; /* f */


	for (vertex = 0; vertex < mod_matrix -> adjacency_matrix -> dim; vertex++) {
		if (vertex < *sub_vertices_group_ptr) {
			vertices_mod_vec_ptr++;
			continue;
		}

/*		modularity_row_sum = 0;
		total_expected_degrees = 0;
		total_subgroup_neighbors = 0; */

		total_expected_degrees = get_total_expected_degree(vertex, mod_matrix, mod_matrix -> sub_vertices_group);
		total_subgroup_neighbors = get_total_subgroup_neighbors(vertex, mod_matrix, mod_matrix -> sub_vertices_group);

		*vertices_mod_vec_ptr = total_subgroup_neighbors - total_expected_degrees;

		if (*sub_vertices_group_ptr == mod_matrix -> sub_vertices_group[mod_matrix -> sub_vertices_group_size]) {
			break;
		}

		sub_vertices_group_ptr++;
		vertices_mod_vec_ptr++;

/*		for (g_vertex = 0; g_vertex < mod_matrix -> sub_vertices_group_size; g_vertex++) {
			total_expected_degrees += get_expected_degree(vertex, g_vertex, mod_matrix -> adjacency_matrix -> total_degrees);
		total_subgroup_neighbors += is_
*/

	}

	return vertices_mod_vec;

}

int get_total_expected_degree(int vertex, modularity_matrix* mod_matrix, int* sub_vertices_group_ptr) {
	int g_vertex;
	int total_expected_degrees = 0;
	int sum_of_neighbors_degrees = 0;
	int vertex_degree = mod_matrix -> degrees_vector[vertex];

	for (g_vertex = 0; g_vertex < mod_matrix -> sub_vertices_group_size; g_vertex++) {
		sum_of_neighbors_degrees += mod_matrix -> degrees_vector[*sub_vertices_group_ptr];
		sub_vertices_group_ptr++;
	}

	total_expected_degrees = sum_of_neighbors_degrees * vertex_degree / mod_matrix -> total_degrees_num;

	return total_expected_degrees;
}

int get_total_subgroup_neighbors(int vertex, modularity_matrix* mod_matrix, int* sub_vertices_group_ptr) {

	int total_subgroup_neighbors = 0;
	node* neighbor_ptr;

	neighbor_ptr = mod_matrix -> adjacency_matrix -> rows[vertex].head;

	while (*sub_vertices_group_ptr <= mod_matrix -> sub_vertices_group[mod_matrix -> sub_vertices_group_size] &&
			neighbor_ptr != NULL) {

		if (neighbor_ptr -> matrix_index < *sub_vertices_group_ptr) {
			neighbor_ptr = neighbor_ptr -> next;
		}

		else if (neighbor_ptr -> matrix_index > *sub_vertices_group_ptr) {
			sub_vertices_group_ptr++;
		}

		else {
			total_subgroup_neighbors++;
			sub_vertices_group_ptr++;
			neighbor_ptr = neighbor_ptr -> next;
		}
	}

	return total_subgroup_neighbors;

}

double calc_norm_1(modularity_matrix* mod_matrix) {

	double max_col_sum = 0.0;
	double col_sum = 0.0;
	int col;
	int row;

	for (col = 0; col < mod_matrix -> adjacency_matrix -> dim; col++) {
		col_sum = 0;

		for (row = 0; row < mod_matrix -> adjacency_matrix -> dim; row++) {
			col_sum += fabs(calc_mod_matrix_cell(mod_matrix, row, col));
		}

		if (col_sum > max_col_sum) {
			max_col_sum = col_sum;
		}
	}
	return max_col_sum;
}

/* given i,j  - return Bij hat */
double calc_mod_matrix_cell(modularity_matrix* mod_matrix, int row, int col) {
	double is_neighbors = 0.0;
	double expected_degree;
	double vertex_mod;

	node* neighbor_ptr;
	double Bij_hat;

	vertex_mod = mod_matrix -> vertices_mod_vec[row];
	expected_degree = mod_matrix -> degrees_vector[row] * mod_matrix -> degrees_vector[col] /
			mod_matrix -> adjacency_matrix -> total_degrees;

	neighbor_ptr = mod_matrix -> adjacency_matrix -> rows[row].head;
	while (neighbor_ptr != NULL) {
		if (col == neighbor_ptr -> matrix_index) {
			is_neighbors = 1.0;
		}
		neighbor_ptr = neighbor_ptr -> next;
	}

	Bij_hat = is_neighbors - expected_degree - vertex_mod;

	return Bij_hat;

}

/*void free_modularity_matrix(sparse_matrix *adjacency_matrix, int *degrees_vector) {

}*/



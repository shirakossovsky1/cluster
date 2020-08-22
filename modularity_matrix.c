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

	modularity_matrix* mod_matrix;

	printf("%s\n","starting inside mod matrix file");

	mod_matrix = (modularity_matrix*)malloc(sizeof(modularity_matrix));

	/* address the case that sum of degrees in the graph (M) is zero */
	if (adjacency_matrix -> total_degrees == 0) {
		return mod_matrix;
	}

	mod_matrix -> total_degrees_num = adjacency_matrix -> total_degrees;
	printf("total_degrees_num %d\n",mod_matrix -> total_degrees_num);
	/*printf("degrees vector [0] is %d\n",adjacency_matrix -> degrees_vector[0]);
	printf("degrees vector [1] is %d\n",adjacency_matrix -> degrees_vector[1]);
	printf("degrees vector [2] is %d\n",adjacency_matrix -> degrees_vector[2]);*/
	mod_matrix -> degrees_vector = adjacency_matrix -> degrees_vector;
	/*printf("%s\n","hey 2");*/
	mod_matrix -> adjacency_matrix = adjacency_matrix;
	mod_matrix -> sub_vertices_group = sub_vertices_group;
	mod_matrix -> sub_vertices_group_size = group_size;
	printf("mod_matrix -> sub_vertices_group_size is %d\n",mod_matrix -> sub_vertices_group_size);
	mod_matrix -> sub_degrees_vector = calc_sub_degrees_vector(sub_vertices_group, mod_matrix);
	printf("sub_degrees_vector[0] = %d\n",mod_matrix -> sub_degrees_vector[0]);
	mod_matrix -> vertices_mod_vec = calc_vertices_mod_vec(mod_matrix);
	mod_matrix -> norm_1 = calc_norm_1(mod_matrix);
	printf("norm 1 is %f\n",mod_matrix -> norm_1);

	return mod_matrix;
}

int* calc_sub_degrees_vector(int* sub_vertices_group, modularity_matrix* mod_matrix) {
	int* sub_degrees_vector = (int*)calloc(mod_matrix -> sub_vertices_group_size, sizeof(int));

	int* sub_vertices_group_ptr;
	int* sub_degrees_vector_ptr;
	int* degrees_vector_ptr;
	int i;
	int vertices_num;

	sub_vertices_group_ptr = sub_vertices_group;
	sub_degrees_vector_ptr = sub_degrees_vector;
	degrees_vector_ptr = mod_matrix -> degrees_vector;

	vertices_num = mod_matrix -> sub_vertices_group_size;
	printf("vertices_num is %d\n",vertices_num);

	for (i = 0; i < vertices_num; i++) {
		printf("sub_vertices_group[i] is %d \n", sub_vertices_group[i]);
	}

	printf("for loop limit is %d\n",sub_vertices_group[vertices_num -1]);
	for (i = 0; i < sub_vertices_group[vertices_num - 1]+1; i++) {
		printf("i is %d, sub_vertices_group_ptr is %d\n", i, *(sub_vertices_group_ptr));
		if (i == *(sub_vertices_group_ptr)) {
			*sub_degrees_vector_ptr = *degrees_vector_ptr;
			printf("added %d to sub degrees vec\n",*sub_degrees_vector_ptr);
			sub_vertices_group_ptr++;
			sub_degrees_vector_ptr++;
		}
		degrees_vector_ptr++;
	}
	return sub_degrees_vector;
}

/* calculate F(i,g) vector values for certain subgroup */
double* calc_vertices_mod_vec(modularity_matrix* mod_matrix) {
	int vertex;
	int vertex_degree;
	double total_expected_degrees;
	int total_subgroup_neighbors;
	double* vertices_mod_vec_ptr;

	double* vertices_mod_vec = (double*)calloc(mod_matrix -> sub_vertices_group_size, sizeof(double));
	int* sub_vertices_group_ptr = mod_matrix -> sub_vertices_group;
	printf("*(sub_vertices_group_ptr) is %d\n", *(sub_vertices_group_ptr));
	vertices_mod_vec_ptr = vertices_mod_vec; /* f */

	for (vertex = 0; vertex < mod_matrix -> adjacency_matrix -> dim; vertex++) {
		/* if vertex is not in subgroup then skip -> value in the vector will be 0 */

		printf("sub_vertices_group_ptr is %d\n",*sub_vertices_group_ptr);

		if (vertex < *sub_vertices_group_ptr) {
			vertices_mod_vec_ptr++;
			continue;
		}

		vertex_degree = mod_matrix -> degrees_vector[vertex];

/*		modularity_row_sum = 0;
		total_expected_degrees = 0;
		total_subgroup_neighbors = 0; */

		/* calculate the sum of Aij for i = vertex and j is in subgroup*/
		total_subgroup_neighbors = get_total_subgroup_neighbors(vertex, mod_matrix, mod_matrix -> sub_vertices_group);

		/* calculate the sum of ki*kj/M for i = vertex and 1 <= j <= |V|*/
		total_expected_degrees = get_total_expected_degree(vertex, mod_matrix, mod_matrix -> sub_vertices_group);

		/*total_subgroup_neighbors = result[0];
		total_expected_degrees = result[1]* vertex_degree / (mod_matrix -> total_degrees_num);;*/


		printf("total_expected_degrees for vertex %d is %f\n",vertex,total_expected_degrees);
		printf("total_subgroup_neighbors for vertex %d is %d\n",vertex,total_subgroup_neighbors);

		/* apply the value for Fig */
		*vertices_mod_vec_ptr = total_subgroup_neighbors - (double)total_expected_degrees;
		printf("added %f to vertices_mod_vec (f)\n",*vertices_mod_vec_ptr);

		/* if the last vertex in subgroup is reached then quit */
		printf("sub_vertices_group[(mod_matrix -> sub_vertices_group_size) - 1] is %d\n",mod_matrix -> sub_vertices_group[(mod_matrix -> sub_vertices_group_size) - 1]);
		printf("*(sub_vertices_group_ptr) is %d\n", *(sub_vertices_group_ptr));
		if (*(sub_vertices_group_ptr) == mod_matrix -> sub_vertices_group[(mod_matrix -> sub_vertices_group_size) - 1]) {
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

double get_total_expected_degree(int vertex, modularity_matrix* mod_matrix, int* sub_vertices_group) {
	int g_vertex;
	int total_expected_degrees = 0;
	int sum_of_subgroup_vertices_degrees = 0;
	int vertex_degree = mod_matrix -> degrees_vector[vertex];
	int* sub_vertices_group_ptr = sub_vertices_group;

	for (g_vertex = 0; g_vertex < mod_matrix -> sub_vertices_group_size; g_vertex++) {
		sum_of_subgroup_vertices_degrees += mod_matrix -> degrees_vector[*sub_vertices_group_ptr];
		sub_vertices_group_ptr++;
	}

	total_expected_degrees = ((double)sum_of_subgroup_vertices_degrees * (double)vertex_degree) /
			((double)(mod_matrix -> total_degrees_num));

	return total_expected_degrees;
}

int get_total_subgroup_neighbors(int vertex, modularity_matrix* mod_matrix, int* sub_vertices_group) {

	int total_subgroup_neighbors = 0;
	node* neighbor_ptr;
	int* sub_vertices_group_ptr;

	neighbor_ptr = mod_matrix -> adjacency_matrix -> rows[vertex].head;
	sub_vertices_group_ptr = sub_vertices_group;

	while (*sub_vertices_group_ptr <= mod_matrix -> sub_vertices_group[mod_matrix -> sub_vertices_group_size -1] &&
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

	/* go over matrix's columns */
	for (col = 0; col < mod_matrix -> adjacency_matrix -> dim; col++) {
		col_sum = 0.0;

		/* sum each row */
		for (row = 0; row < mod_matrix -> adjacency_matrix -> dim; row++) {
			col_sum += fabs(calc_mod_matrix_cell(mod_matrix, row, col));
			printf("summing col %d, sum so far is %f\n",col, col_sum);
		}

		/* update maximum */
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

	/* get Fig for i=row */
	vertex_mod = mod_matrix -> vertices_mod_vec[row];

	/* get Ki*Kj/M for i=row, j=col */
	expected_degree = (double)mod_matrix -> degrees_vector[row] * (double)mod_matrix -> degrees_vector[col] /
			(double)mod_matrix -> adjacency_matrix -> total_degrees;

	/* get Aij for i=row, j=col */
	neighbor_ptr = mod_matrix -> adjacency_matrix -> rows[row].head;
	while (neighbor_ptr != NULL) {
		if (col == neighbor_ptr -> matrix_index) {
			is_neighbors = 1.0;
		}
		neighbor_ptr = neighbor_ptr -> next;
	}

	if (row == col){
		/* Aij - Ki*Kj/M - Fig */
		Bij_hat = is_neighbors - expected_degree - vertex_mod;
	}
	else {
		/* Aij - Ki*Kj/M - (0 * Fig) */
		Bij_hat = is_neighbors - expected_degree;
	}

	return Bij_hat;

}

void free_modularity_matrix(modularity_matrix *mod_matrix) {

	free(mod_matrix -> sub_degrees_vector);
	free(mod_matrix -> degrees_vector);
	free(mod_matrix -> vertices_mod_vec);
	free(mod_matrix -> sub_vertices_group);
}


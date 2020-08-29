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

modularity_matrix* create_modularity_matrix(sparse_matrix *adjacency_matrix, array *sub_vertices_group) {

	modularity_matrix* mod_matrix;

	/*printf("%s\n","&&& starting inside mod matrix file &&&");*/

	mod_matrix = (modularity_matrix*)malloc(sizeof(modularity_matrix));

	/* address the case that sum of degrees in the graph (M) is zero */
	if (adjacency_matrix->total_degrees == 0) {
		return mod_matrix;
	}

	mod_matrix->total_degrees_num = adjacency_matrix->total_degrees;
	mod_matrix->degrees_vector = adjacency_matrix->degrees_vector;
	mod_matrix->adjacency_matrix = adjacency_matrix;
	mod_matrix->sub_vertices_group = sub_vertices_group;
	/*printf("sub_vertices_group_size is %d\n",mod_matrix->sub_vertices_group->size);*/
	mod_matrix->sub_degrees_vector = calc_sub_degrees_vector(sub_vertices_group, mod_matrix);
	mod_matrix->vertices_mod_vec = calc_vertices_mod_vec(mod_matrix);
	mod_matrix->norm_1 = calc_norm_1(mod_matrix);
	/*printf("norm 1 is %f\n",mod_matrix->norm_1);*/

	/*printf("%s\n","&&& finishing inside mod matrix file &&&");*/

	return mod_matrix;
}

unsigned int* calc_sub_degrees_vector(array* sub_vertices_group, modularity_matrix* mod_matrix) {
	unsigned int i, subgroup_size, *sub_degrees_vector, *sub_degrees_vector_ptr, *degrees_vector_ptr, *sub_vertices_group_ptr;

	sub_degrees_vector = (unsigned int*)calloc(sub_vertices_group->size, sizeof(unsigned int));

	sub_vertices_group_ptr = sub_vertices_group->array;
	sub_degrees_vector_ptr = sub_degrees_vector;
	degrees_vector_ptr = mod_matrix->degrees_vector;

	subgroup_size = sub_vertices_group->size;
	/*printf("vertices_num is %d\n",subgroup_size);*/

	for (i = 0; i < subgroup_size; i++) {
		/*printf("sub_vertices_group[%d] is %d \n", i, sub_vertices_group->array[i]);*/
	}

	for (i = 0; i <= sub_vertices_group->array[subgroup_size - 1]; i++) {
		if (i == *(sub_vertices_group_ptr)) {
			*sub_degrees_vector_ptr = *degrees_vector_ptr;
			sub_vertices_group_ptr++;
			sub_degrees_vector_ptr++;
		}
		degrees_vector_ptr++;
	}
	return sub_degrees_vector;
}

/* calculate F(i,g) vector values for certain subgroup */
float* calc_vertices_mod_vec(modularity_matrix* mod_matrix) {
	unsigned int i, vertex, vertex_degree, total_subgroup_neighbors, *sub_vertices_group_ptr;
	float total_expected_degrees, *vertices_mod_vec, *vertices_mod_vec_ptr;

	vertices_mod_vec = (float*)calloc(mod_matrix->sub_vertices_group->size , sizeof(float));
	sub_vertices_group_ptr = mod_matrix->sub_vertices_group->array;
	vertices_mod_vec_ptr = vertices_mod_vec; /* f */

	for (i = 0; i < mod_matrix->sub_vertices_group->size; i++){

		/*printf("%s\n", "hey 4");
		if (vertex < *sub_vertices_group_ptr) {
			vertices_mod_vec_ptr++;
			continue;
		}*/

		vertex_degree = mod_matrix->sub_degrees_vector[i];
		vertex = mod_matrix->sub_vertices_group->array[i];

/*		modularity_row_sum = 0;
		total_expected_degrees = 0;
		total_subgroup_neighbors = 0; */

		/* calculate the sum of Aij for i = vertex and j is in subgroup*/
		total_subgroup_neighbors = get_total_subgroup_neighbors(vertex, mod_matrix, mod_matrix->sub_vertices_group);


		/* calculate the sum of ki*kj/M for i = vertex and 1 <= j <= |V|*/
		total_expected_degrees = get_total_expected_degree(vertex_degree, mod_matrix, mod_matrix->sub_vertices_group);

		/*total_subgroup_neighbors = result[0];
		total_expected_degrees = result[1]* vertex_degree / (mod_matrix->total_degrees_num);;*/


		/*printf("total_expected_degrees for vertex %d is %f\n",vertex,total_expected_degrees);
		printf("total_subgroup_neighbors for vertex %d is %d\n",vertex,total_subgroup_neighbors);*/

		/*apply the value for Fig*/
		*vertices_mod_vec_ptr = (float)total_subgroup_neighbors - total_expected_degrees;
		/*printf("added %f to f_vector (f)\n",*vertices_mod_vec_ptr);

		if the last vertex in subgroup is reached then quit
		printf("sub_vertices_group[(mod_matrix->sub_vertices_group_size) - 1] is %d\n",mod_matrix->sub_vertices_group[(mod_matrix->sub_vertices_group_size) - 1]);*/
		/*printf("*(vertices_mod_vec_ptr) is %f\n", *(vertices_mod_vec_ptr));*/
		if (*(sub_vertices_group_ptr) == mod_matrix->sub_vertices_group->array[(mod_matrix->sub_vertices_group->size) - 1]) {
			break;
		}

		sub_vertices_group_ptr++;
		vertices_mod_vec_ptr++;

/*		for (g_vertex = 0; g_vertex < mod_matrix->sub_vertices_group_size; g_vertex++) {
			total_expected_degrees += get_expected_degree(vertex, g_vertex, mod_matrix->adjacency_matrix->total_degrees);
		total_subgroup_neighbors += is_
*/
	}

	return vertices_mod_vec;

}

float get_total_expected_degree(unsigned int vertex_degree, modularity_matrix* mod_matrix, array* sub_vertices_group) {
	unsigned int g_vertex, sum_of_subgroup_vertices_degrees = 0, *sub_vertices_group_ptr;
	float total_expected_degrees = 0;

	sub_vertices_group_ptr = sub_vertices_group->array;

	for (g_vertex = 0; g_vertex < mod_matrix->sub_vertices_group->size; g_vertex++) {
		sum_of_subgroup_vertices_degrees += mod_matrix->degrees_vector[*sub_vertices_group_ptr];
		sub_vertices_group_ptr++;
	}
	/*printf("sum_of_subgroup_vertices_degrees is %d\n", sum_of_subgroup_vertices_degrees);
	printf("vertex_degree is %d\n", vertex_degree);*/

	total_expected_degrees = ((float)sum_of_subgroup_vertices_degrees * (float)vertex_degree) /
			((float)(mod_matrix->total_degrees_num));

	/*printf("total_expected_degrees is %f\n", total_expected_degrees);*/

	return total_expected_degrees;
}

unsigned int get_total_subgroup_neighbors(unsigned int vertex, modularity_matrix* mod_matrix, array* sub_vertices_group) {

	unsigned int total_subgroup_neighbors = 0, *sub_vertices_group_ptr;
	node* neighbor_ptr;

	neighbor_ptr = mod_matrix->adjacency_matrix->rows[vertex].head;
	sub_vertices_group_ptr = sub_vertices_group->array;

	while (sub_vertices_group_ptr <= &sub_vertices_group->array[sub_vertices_group->size -1] &&
			neighbor_ptr != NULL) {

		if (neighbor_ptr->matrix_index < *sub_vertices_group_ptr) {
			neighbor_ptr = neighbor_ptr->next;
		}

		else if (neighbor_ptr->matrix_index > *sub_vertices_group_ptr) {
			sub_vertices_group_ptr++;
		}

		else {
			total_subgroup_neighbors++;
			sub_vertices_group_ptr++;
			neighbor_ptr = neighbor_ptr->next;
		}
	}
	return total_subgroup_neighbors;
}

float calc_norm_1(modularity_matrix* mod_matrix) {

	unsigned int *col, *row, iteration = 0;
	float max_col_sum = 0.0, col_sum = 0.0;

	/* go over matrix's columns */
	for (col = mod_matrix->sub_vertices_group->array; col <= &(mod_matrix->sub_vertices_group->array[mod_matrix->sub_vertices_group->size-1]); col++){
		col_sum = 0.0;
		iteration = 0;

		/* sum each row */
		for (row = mod_matrix->sub_vertices_group->array; row <= &(mod_matrix->sub_vertices_group->array[mod_matrix->sub_vertices_group->size-1]); row++){
			col_sum += fabs(calc_mod_matrix_cell(mod_matrix, *row, *col, iteration));
			/*printf("matrix cell in place (%d,%d) is %f\n",*row,*col,calc_mod_matrix_cell(mod_matrix, *row, *col, iteration));*/
			iteration++;
		}

		/* update maximum */
		if (col_sum > max_col_sum) {
			max_col_sum = col_sum;
		}

	}

	return max_col_sum;

	}


/* given i,j  - return Bij hat */
float calc_mod_matrix_cell(modularity_matrix* mod_matrix, unsigned int row, unsigned int col, unsigned int iteration) {
	float is_neighbors = 0.0, expected_degree, vertex_mod, Bij_hat;
	node* neighbor_ptr;

	/* get Fig for i=row */
	vertex_mod = mod_matrix->vertices_mod_vec[iteration];
	/*printf("aaa vertex mod is %f\n",vertex_mod);*/

	/* get Ki*Kj/M for i=row, j=col */
	expected_degree = (float)mod_matrix->degrees_vector[row] * (float)mod_matrix->degrees_vector[col] /
			(float)mod_matrix->adjacency_matrix->total_degrees;
	/*printf("aaa expected degree is %f\n",expected_degree);*/

	/* get Aij for i=row, j=col */
	neighbor_ptr = mod_matrix->adjacency_matrix->rows[row].head;
	while (neighbor_ptr != NULL) {
		if (col == neighbor_ptr->matrix_index) {
			is_neighbors = 1.0;
		}
		neighbor_ptr = neighbor_ptr->next;
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

	free(mod_matrix->sub_degrees_vector);
	free(mod_matrix->vertices_mod_vec);
	free_array(mod_matrix->sub_vertices_group);
	free(mod_matrix);

}



#include <math.h>
#include "modularity_matrix.h"

/* main function which calculates all modularity matrix properties into modularity matrix object. */
modularity_matrix* create_modularity_matrix(const sparse_matrix *adjacency_matrix, array *sub_vertices_group) {

	modularity_matrix* mod_matrix;
	mod_matrix = (modularity_matrix*)malloc(sizeof(modularity_matrix));

	/* address the case that sum of degrees in the graph (M) is zero */
	check_int_division_by_zero(adjacency_matrix->total_degrees);

	mod_matrix->total_degrees_num = adjacency_matrix->total_degrees;
	mod_matrix->degrees_vector = adjacency_matrix->degrees_vector;
	mod_matrix->adjacency_matrix = adjacency_matrix;
	mod_matrix->sub_vertices_group = sub_vertices_group;
	mod_matrix->sub_degrees_vector = calc_sub_degrees_vector(sub_vertices_group, mod_matrix);
	mod_matrix->vertices_mod_vec = calc_vertices_mod_vec(mod_matrix);
	mod_matrix->norm_1 = calc_norm_1(mod_matrix);

	return mod_matrix;
}

/* extract sub degrees vector from the original degrees vector */
unsigned int* calc_sub_degrees_vector(array* sub_vertices_group, modularity_matrix* mod_matrix) {

	unsigned int i, subgroup_size;
	unsigned int *sub_degrees_vector, *sub_degrees_vector_ptr, *sub_vertices_group_ptr;
	const unsigned int *degrees_vector_ptr;

	sub_degrees_vector = (unsigned int*)calloc(sub_vertices_group->size, sizeof(unsigned int));
	sub_degrees_vector_ptr = sub_degrees_vector;

	sub_vertices_group_ptr = sub_vertices_group->array;
	degrees_vector_ptr = mod_matrix->degrees_vector;
	subgroup_size = sub_vertices_group->size;

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

/* calculate F(i,g) vector values for vertices sub-group */
float* calc_vertices_mod_vec(modularity_matrix* mod_matrix) {
	unsigned int i, vertex, vertex_degree, total_subgroup_neighbors, *sub_vertices_group_ptr;
	float total_expected_degrees, *vertices_mod_vec, *vertices_mod_vec_ptr;

	vertices_mod_vec = (float*)calloc(mod_matrix->sub_vertices_group->size , sizeof(float));
	sub_vertices_group_ptr = mod_matrix->sub_vertices_group->array;
	vertices_mod_vec_ptr = vertices_mod_vec; /* f */

	for (i = 0; i < mod_matrix->sub_vertices_group->size; i++){

		vertex_degree = mod_matrix->sub_degrees_vector[i];
		vertex = mod_matrix->sub_vertices_group->array[i];

		/* calculate the sum of Aij for i = vertex and j is in subgroup */
		total_subgroup_neighbors = get_total_subgroup_neighbors(vertex, mod_matrix, mod_matrix->sub_vertices_group);

		/* calculate the sum of ki*kj/M for i = vertex and 1 <= j <= |V| */
		total_expected_degrees = get_total_expected_degree(vertex_degree, mod_matrix, mod_matrix->sub_vertices_group);

		/* apply the value for Fig */
		*vertices_mod_vec_ptr = (float)total_subgroup_neighbors - total_expected_degrees;

		/* if the last vertex in sub-group is reached then quit */
		if (*(sub_vertices_group_ptr) ==
				mod_matrix->sub_vertices_group->array[(mod_matrix->sub_vertices_group->size) - 1]) {
			break;
		}

		sub_vertices_group_ptr++;
		vertices_mod_vec_ptr++;
	}

	return vertices_mod_vec;

}

/* calculate the sum of ki*kj/M for i = vertex and 1 <= j <= |V| */
float get_total_expected_degree(unsigned int vertex_degree,
		modularity_matrix* mod_matrix, array* sub_vertices_group) {

	unsigned int g_vertex, sum_of_subgroup_vertices_degrees = 0, *sub_vertices_group_ptr;
	float total_expected_degrees = 0;

	sub_vertices_group_ptr = sub_vertices_group->array;

	for (g_vertex = 0; g_vertex < mod_matrix->sub_vertices_group->size; g_vertex++) {
		sum_of_subgroup_vertices_degrees += mod_matrix->degrees_vector[*sub_vertices_group_ptr];
		sub_vertices_group_ptr++;
	}

	total_expected_degrees = ((float)sum_of_subgroup_vertices_degrees * (float)vertex_degree) /
			((float)(mod_matrix->total_degrees_num));

	return total_expected_degrees;
}

/* calculate the sum of Aij for i = vertex and j is in subgroup */
unsigned int get_total_subgroup_neighbors(unsigned int vertex, modularity_matrix* mod_matrix, array* sub_vertices_group) {

	unsigned int total_subgroup_neighbors = 0, *sub_vertices_group_ptr;
	node* neighbor_ptr;

	neighbor_ptr = mod_matrix->adjacency_matrix->rows[vertex].head;
	sub_vertices_group_ptr = sub_vertices_group->array;

	while (sub_vertices_group_ptr <= &sub_vertices_group->array[sub_vertices_group->size -1] &&
			neighbor_ptr != NULL) {

		if (neighbor_ptr->data < *sub_vertices_group_ptr) {
			neighbor_ptr = neighbor_ptr->next;
		}

		else if (neighbor_ptr->data > *sub_vertices_group_ptr) {
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

/* calculate norm 1 of modularity matrix */
float calc_norm_1(modularity_matrix* mod_matrix) {

	unsigned int *col, *row, iteration = 0;
	float max_col_sum = 0.0, col_sum = 0.0;

	/* go over matrix's columns */
	for (col = mod_matrix->sub_vertices_group->array; col <= &(mod_matrix->sub_vertices_group->array[mod_matrix->sub_vertices_group->size-1]); col++){
		col_sum = 0.0;
		iteration = 0;

		/* sum each row */
		for (row = mod_matrix->sub_vertices_group->array; row <=
			&(mod_matrix->sub_vertices_group->array[mod_matrix->sub_vertices_group->size-1]); row++){

			col_sum += fabs(calc_mod_matrix_cell(mod_matrix, *row, *col, iteration, true));
			iteration++;
		}

		/* update maximum */
		if (col_sum > max_col_sum) {
			max_col_sum = col_sum;
		}

	}

	return max_col_sum;

	}

/* given i,j  - return Bij = modularity matrix cell. substruct f(i,g) if needed. */
float calc_mod_matrix_cell(modularity_matrix* mod_matrix, unsigned int row, unsigned int col,
		unsigned int iteration, bool to_hat) {

	float is_neighbors = 0.0, expected_degree, vertex_mod = 0.0, Bij_hat;
	node* neighbor_ptr;

	/* get Ki*Kj/M for i=row, j=col */
	expected_degree = (float)mod_matrix->degrees_vector[row] * (float)mod_matrix->degrees_vector[col] /
			(float)mod_matrix->adjacency_matrix->total_degrees;

	/* get Aij for i=row, j=col */
	neighbor_ptr = mod_matrix->adjacency_matrix->rows[row].head;

	while (neighbor_ptr != NULL) {
		if (col == neighbor_ptr->data) {
			is_neighbors = 1.0;
		}
		neighbor_ptr = neighbor_ptr->next;
	}

	if (to_hat && row == col){
		/* get Fig for i=row */
		vertex_mod = mod_matrix->vertices_mod_vec[iteration];
		/* Aij - Ki*Kj/M - Fig */
		Bij_hat = is_neighbors - expected_degree - vertex_mod;
	}
	else {
		/* Aij - Ki*Kj/M - (0 * Fig) */
		Bij_hat = is_neighbors - expected_degree;
	}

	return Bij_hat;

}

/* free memory of modularity matrix object */
void free_modularity_matrix(modularity_matrix *mod_matrix) {

	free(mod_matrix->sub_degrees_vector);
	free(mod_matrix->vertices_mod_vec);
	free_array(mod_matrix->sub_vertices_group);
	free(mod_matrix);

}


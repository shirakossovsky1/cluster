/*
 * input_matrix.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef ADJACENCY_MATRIX_H_
#define ADJACENCY_MATRIX_H_

#include "utils.h"
#include "error.h"

/* matrix object that will store modularity values for each vertex */
typedef struct sparse_matrix {
	/* Matrix size (n*n) */
	unsigned int	dim;
	unsigned int 	total_degrees;
	unsigned int*	degrees_vector;
	linked_list* 	rows;

} sparse_matrix;

sparse_matrix* read_input_into_sparse(FILE* input_file, unsigned int vertices_num);
linked_list* add_row(FILE* input_file, unsigned int vertex_degree, linked_list *curr_list);
void add_node(unsigned int curr_vertex, linked_list *curr_list);
void write_input_matrix(sparse_matrix sparse_mat, FILE* output_file);
void free_sparse_matrix(sparse_matrix *sparse_mat);
void print_degrees_vector(sparse_matrix sparse_mat);



#endif /* ADJACENCY_MATRIX_H_ */

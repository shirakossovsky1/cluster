/*
 * The module reads the input file into an adjacency matrix and a vector of the vertices' degrees.
 * The adjacency matrix is implemented with sparse matrix, and inner implementation of linked lists.
 */

#ifndef ADJACENCY_MATRIX_H_
#define ADJACENCY_MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "utils.h"
#include "error.h"

/* sparse matrix object that will store adjacency matrix
	dim - the matrix dimensions = number of vertices.
	total degrees - sum of all vertices degrees.
	degrees vector - index i will contain the degree of vertex i.
	rows - a vector of linked lists where each index represents a vertex and contains a linked lists of it's neighbors
 */
typedef struct sparse_matrix {
	/* matrix size (n*n) */
	unsigned int	dim;
	unsigned int 	total_degrees;
	unsigned int*	degrees_vector;
	linked_list* 	rows;

} sparse_matrix;

sparse_matrix* read_input_into_sparse(FILE* input_file, unsigned int vertices_num);
linked_list* add_row(FILE* input_file, unsigned int vertex_degree, linked_list *curr_list);
void add_node(unsigned int curr_vertex, linked_list *curr_list);
void free_sparse_matrix(sparse_matrix *sparse_mat);

#endif /* ADJACENCY_MATRIX_H_ */

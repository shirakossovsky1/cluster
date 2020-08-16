/*
 * input_matrix.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef INPUT_MATRIX_H_
#define INPUT_MATRIX_H_

/* node object for linked list */
typedef struct node{
	int 	matrix_index;
    struct 	node *next;
} node;

/* linked list object */
typedef struct linked_list{
	node 	*head;
	node 	*tail;

} linked_list;

/* matrix object that will store modularity values for each vertex */
typedef struct sparse_matrix {
	/* Matrix size (n*n) */
	int		dim;
	int 	total_degrees;
	int*	degrees_vector;
	linked_list* rows;

} sparse_matrix;

sparse_matrix* read_input_into_sparse(FILE* input_file, int vertices_num);
linked_list* add_row(FILE* input_file, int vertex_degree);
void add_node(int curr_vertex, linked_list *curr_list);
void write_input_matrix(sparse_matrix sparse_mat, FILE* output_file);
void free_sparse_matrix(sparse_matrix *sparse_mat);
void print_degrees_vector(sparse_matrix sparse_mat);



#endif /* INPUT_MATRIX_H_ */

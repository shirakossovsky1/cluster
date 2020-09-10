/*
 * The module holds all the properties that relevant for the group's modularity.
 * instead of holding the modularity matrix itself the object stores all the needed data
 * in order to calculate the matrix cells.
 */

#ifndef MODULARITY_MATRIX_H_
#define MODULARITY_MATRIX_H_

#include "adjacency_matrix.h"

/* modularity matrix object of vertices sub-group (Bg-hat).
	adjacency_matrix , total_degrees_num , degrees_vector - original graph properties (A).
	sub_vertices_group - an array of the sub-group vertices (g).
	sub_degrees_vector - a vector containing the degrees of sub-group vertices.
	vertices_mod_vec - a vector containing the modularity of a row in the matrix (f(i,g)).
	norm_1 - used to shift the matrix for finding eigenpair process (||C||1).
 */
typedef struct modularity_matrix{
	const sparse_matrix 	*adjacency_matrix; /* A as a sparse matrix */
	unsigned int 			total_degrees_num;
	const unsigned int 		*degrees_vector;
	array					*sub_vertices_group; /* g */
	unsigned int 			*sub_degrees_vector;
	float 					*vertices_mod_vec;
	float 					norm_1;
} modularity_matrix;

modularity_matrix* create_modularity_matrix(const sparse_matrix *adjacency_matrix, array* sub_vertices_group);
unsigned int* calc_sub_degrees_vector(array* sub_vertices_group, modularity_matrix* mod_matrix);
float* calc_vertices_mod_vec(modularity_matrix* mod_matrix);
float get_total_expected_degree(unsigned int vertex_degree, modularity_matrix* mod_matrix, array* sub_vertices_group);
unsigned int get_total_subgroup_neighbors(unsigned int vertex, modularity_matrix* mod_matrix, array* sub_vertices_group);
float calc_norm_1(modularity_matrix* mod_matrix);
float calc_mod_matrix_cell(modularity_matrix* mod_matrix, unsigned int row, unsigned int col, unsigned int iteration, bool to_hat);
void free_modularity_matrix(modularity_matrix *mod_matrix);

#endif /* MODULARITY_MATRIX_H_ */

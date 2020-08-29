/*
 * modularity_matrix.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef MODULARITY_MATRIX_H_
#define MODULARITY_MATRIX_H_

#include "adjacency_matrix.h"

/* modularity matrix (Bg) */

typedef struct modularity_matrix{
	sparse_matrix 	*adjacency_matrix; /* A as a sparse matrix */
	unsigned int 	total_degrees_num;
	unsigned int 	*degrees_vector;
	unsigned int 	*sub_degrees_vector;
	float 			*vertices_mod_vec; /*f(i,g) */
	array			*sub_vertices_group; /* g */
	float 			norm_1;
} modularity_matrix;

modularity_matrix* create_modularity_matrix(sparse_matrix *adjacency_matrix, array* sub_vertices_group);
unsigned int* calc_sub_degrees_vector(array* sub_vertices_group, modularity_matrix* mod_matrix);
float* calc_vertices_mod_vec(modularity_matrix* mod_matrix);
float get_total_expected_degree(unsigned int vertex_degree, modularity_matrix* mod_matrix, array* sub_vertices_group);
unsigned int get_total_subgroup_neighbors(unsigned int vertex, modularity_matrix* mod_matrix, array* sub_vertices_group);
float calc_norm_1(modularity_matrix* mod_matrix);
float calc_mod_matrix_cell(modularity_matrix* mod_matrix, unsigned int row, unsigned int col, unsigned int iteration);
void free_modularity_matrix(modularity_matrix *mod_matrix);

#endif /* MODULARITY_MATRIX_H_ */

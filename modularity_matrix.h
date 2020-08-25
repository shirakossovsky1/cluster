/*
 * modularity_matrix.h
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#ifndef MODULARITY_MATRIX_H_
#define MODULARITY_MATRIX_H_

#include "input_matrix.h"

/* modularity matrix (Bg) */

typedef struct modularity_matrix{
	sparse_matrix 	*adjacency_matrix; /* A as a sparse matrix */
	int 			total_degrees_num;
	int 			*degrees_vector;
	int 			*sub_degrees_vector;
	double 			*vertices_mod_vec; /*f(i,g) */
	int 			*sub_vertices_group; /* g */
	int 			sub_vertices_group_size; /* size of g */
	double 			norm_1;
} modularity_matrix;

modularity_matrix* create_modularity_matrix(sparse_matrix *adjacency_matrix, int* sub_vertices_group, int group_size);
int* calc_sub_degrees_vector(int* sub_vertices_group, modularity_matrix* mod_matrix);
double* calc_vertices_mod_vec(modularity_matrix* mod_matrix);
double get_total_expected_degree(int vertex_degree, modularity_matrix* mod_matrix, int* sub_vertices_group_ptr);
int get_total_subgroup_neighbors(int vertex, modularity_matrix* mod_matrix, int* sub_vertices_group_ptr);
double calc_norm_1(modularity_matrix* mod_matrix);
double calc_mod_matrix_cell(modularity_matrix* mod_matrix, int row, int col);
void free_modularity_matrix(modularity_matrix *mod_matrix);

#endif /* MODULARITY_MATRIX_H_ */

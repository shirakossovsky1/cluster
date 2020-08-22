/*
 ============================================================================
 Name        : cluster.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "modularity_matrix.h"
#include "leading_eigenpair.h"
#include "maximize_modularity.h"

#define IS_NOT_POSITIVE(X) ((X) <= 0.00001)

int main(int argc, char* argv[]) {

	/*int** P;
	int** O;*/
	sparse_matrix *input_matrix;
	int vertices_num;
	int n;
	int i;
	int* sub_vertices_group;
	int* sub_vertices_group_ptr;
	modularity_matrix* mod_matrix;
	leading_eigenpair* leading_pair;
	double modularity_delta;

	FILE* input_file;
	FILE* output_file;

	/*pre-process inputs before reading input into sparse matrix*/
	clock_t	start_time, end_time;

	input_file = fopen(argv[1], "r");
	assert(input_file != NULL);
	output_file = fopen(argv[2], "w");

	start_time = clock();

	setvbuf(stdout, NULL, _IONBF, 0);

	assert(argc == 3);

	n = fread(&vertices_num, sizeof(int), 1, input_file);
	assert(n == 1);
	printf("vertices num is %d\n", vertices_num);

	printf("%s\n","starting read input into sparse");

	input_matrix = read_input_into_sparse(input_file, vertices_num);
	print_degrees_vector(*input_matrix);
	printf("%s\n","finishing read input into sparse");

	write_input_matrix(*input_matrix, output_file);
	printf("%s\n","finishing write input");

	sub_vertices_group = (int*)calloc(vertices_num, sizeof(int));
	sub_vertices_group_ptr = sub_vertices_group;

	for (i = 0; i < vertices_num; i++) {
		*sub_vertices_group_ptr = i;
		sub_vertices_group_ptr++;
		printf("sub_vertices_group[i] is %d \n", sub_vertices_group[i]);
	}

	mod_matrix = create_modularity_matrix(input_matrix, sub_vertices_group, vertices_num);
	printf("%s\n","finishing create modularity matrix");

	leading_pair = create_eigenpair(mod_matrix);
	printf("%s\n","finishing eigenpair");

	printf("%s\n","starting modularity delta");

	modularity_delta = calc_modularity_delta(leading_pair);

	printf("modularity_delta is %f\n",modularity_delta);
	printf("%s\n","finishing modularity delta");

	fclose(input_file);
	fclose(output_file);

	end_time = clock();

	printf("Execution time is %f seconds\n", ((double)(end_time-start_time) / CLOCKS_PER_SEC));

	/*free_sparse_matrix(input_matrix);
	free_modularity_matrix(mod_matrix);*/

	free_leading_eigenpair(leading_pair);
	free(leading_pair);
	free(input_matrix);
	free(mod_matrix);

	return EXIT_SUCCESS;


/*	while (length(P) > 0) {
		int* sub_graph = pop(P);

		struct Bg {
				// M - sum of degress
				// degrees vec
				//sparse matrix
				//s - division vec
			};

		shifted_matrix = shift_matrix();
		eigen_pair = find_leading_eigenpair(norm);

		if IS_NOT_POSITIVE(eigen_value):
			add_g_to_O();
			continue;

		division_vec = compute_division_vec(); //s - kalllil
		modularity_delta = compute_modularity_delta(division_vec, Bg);

		if IS_NOT_POSITIVE(eigen_value):
			add_g_to_O();
			continue;

		optimizied_division_vec = maximize_modularity(division_vec); //algorithm 4

		if is_trivial(optimizied_division_vec) {
			add_g_to_O();
			continue;
		}

		else {
			is size_2()
			handle somehow(); //almost trivial_handling
		}

	}

	create_ouput_graph();

	sparse_input_matrix = read_matrix_into_sparse(argv[]);

	struct Bg {
		// M - sum of degress
		// degrees vec
		//sparse matrix
		//s - division vec
	};

	shifted_matrix = shift_matrix(); //need to get Bg matrix as input
	sum_col();
	norm = calc_norm();
	add_norm_to_diag();

	eigen_pair = find_leading_eigenpair(norm); //need to have norm as input
	find_leading_eigenvalue();
	find_leading_eigenvector();
	power_iteration();

	if IS_NOT_POSITIVE(eigen_value):
		add_g_to_O();
		break;

	division_vec = compute_division_vec(); //s - kalllil
	modularity_delta = compute_modularity_delta(division_vec, Bg);

	if IS_NOT_POSITIVE(eigen_value):
		add_g_to_O();
		break;

	maximize_modularity(division_vec); //algorithm 4



	return EXIT_SUCCESS; */
}

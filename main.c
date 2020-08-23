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
/*#include "modularity_matrix.h"*/
#include "leading_eigenpair.h"
#include "maximize_modularity.h"
#include "division.h"

#define IS_NOT_POSITIVE(X) ((X) <= 0.00001)

int main(int argc, char* argv[]) {

	queue *P;
	queue *O;
	sparse_matrix *input_matrix;
	int vertices_num;
	int n;
	int i;
	array* sub_vertices_group;
	array *all_vertices;
	int *all_vertices_ptr;
	modularity_matrix* mod_matrix;
	leading_eigenpair* leading_pair;
	double modularity_delta, delta_Q;
	division *curr_division;

	FILE* input_file;
	FILE* output_file;

	/*int vertices[14] = {5,2,1,2,2,0,2,2,0,1,1,4,1,3};*/

	/*pre-process inputs before reading input into sparse matrix*/
	clock_t	start_time, end_time;

	input_file = fopen(argv[1], "r");
	assert(input_file != NULL);
	output_file = fopen(argv[2], "w");

	start_time = clock();

	setvbuf(stdout, NULL, _IONBF, 0);

	assert(argc == 3);

	/*fwrite(vertices, sizeof(int), 14, input_file);*/

	n = fread(&vertices_num, sizeof(int), 1, input_file);
	/*assert(n == 1);*/
	printf("vertices num is %d\n", vertices_num);

	/*printf("%s\n","starting read input into sparse");

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

	free_sparse_matrix(input_matrix);
	free_modularity_matrix(mod_matrix);

	free_leading_eigenpair(leading_pair);
	free(leading_pair);
	free(input_matrix);
	free(mod_matrix);

	return EXIT_SUCCESS; */

	printf("%s\n", "initializing O and P");
	P = (queue*)calloc(vertices_num, sizeof(queue));
	P -> list = (linked_list_of_arrays*)calloc(vertices_num, sizeof(linked_list));
	P -> size = 0;

	all_vertices = (array*)calloc(vertices_num, sizeof(array));
	all_vertices -> array = (int*)calloc(vertices_num, sizeof(int));
	all_vertices -> size = vertices_num;
	all_vertices_ptr = all_vertices -> array;

	for (i = 0; i < all_vertices -> size; i++) {
		/*printf("i is %d\n", i);*/
		*all_vertices_ptr = i;
		all_vertices_ptr++;
		/*printf("all_vertices_ptr[i] is %d \n", all_vertices->array[i]);*/
	}
	push(P, all_vertices);

	O = (queue*)calloc(vertices_num, sizeof(queue));
	O -> list = (linked_list_of_arrays*)calloc(vertices_num, sizeof(linked_list));
	O -> size = 0;

	input_matrix = read_input_into_sparse(input_file, vertices_num);
	printf("%s\n","finishing read input into sparse");


	while (P -> size > 0) {
		sub_vertices_group = pop(P);

		mod_matrix = create_modularity_matrix(input_matrix, sub_vertices_group->array, sub_vertices_group->size);
		printf("%s\n","finishing create modularity matrix");

		leading_pair = create_eigenpair(mod_matrix);
		printf("%s\n","finishing eigenpair");


		if IS_NOT_POSITIVE(leading_pair->leading_eigenvalue){
			printf("%s\n","eigenvalue is negative - moving to next group in g");
			push(O, sub_vertices_group);
			continue;
		}

		printf("%s\n","starting modularity delta");

		modularity_delta = calc_modularity_delta(leading_pair);

		printf("modularity_delta is %f\n",modularity_delta);
		printf("%s\n","finishing modularity delta");

		if IS_NOT_POSITIVE(modularity_delta){
			printf("%s\n","modularity delta is negative - moving to next group in g");
			push(O, sub_vertices_group);
			continue;
		}

		delta_Q = improve_modularity(leading_pair); /*algorithm 4*/

		curr_division =  create_division(leading_pair->division_vector, mod_matrix->sub_vertices_group,
				mod_matrix->sub_vertices_group_size);

		/*handle a case when one sub group is empty*/
		if (curr_division->g1->size == 0 || curr_division->g2->size == 0) {
			push(O, sub_vertices_group);
			printf("added orignal group of size %d to O\n",sub_vertices_group->size);
			continue;
		}

		else {
			if (curr_division -> g1 -> size == 1){
				push(O, curr_division -> g1);
				printf("added g1 of size %d to O\n",curr_division -> g1 -> size);

			}
			else {
				push(P, curr_division -> g1);
				printf("added g1 of size %d to P\n",curr_division -> g1 -> size);
			}
			if (curr_division -> g2 -> size == 1){
				push(O, curr_division -> g2);
				printf("added g2 of size %d to O\n",curr_division -> g2 -> size);
			}
			else {
				push(P, curr_division -> g2);
				printf("added g2 of size %d to P\n",curr_division -> g2 -> size);
			}
		}

		for (i = 0; i < sub_vertices_group->size; i++) {
			printf("division_vector[%d] after optimization = %f\n", i, curr_division ->division_vector[i]);
		}

		printf("size of O is %d, size of P is %d\n",O->size, P->size);
	}

	create_output_graph(O, output_file);

	fclose(input_file);
	fclose(output_file);

	end_time = clock();

	printf("Execution time is %f seconds\n", ((double)(end_time-start_time) / CLOCKS_PER_SEC));

	return EXIT_SUCCESS;

}


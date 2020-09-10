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
#include "leading_eigenpair.h"
#include "maximize_modularity.h"
#include "division.h"

#define IS_NOT_POSITIVE(X) ((X) <= 0.00001)

int main(int argc, char* argv[]) {

	int n;
	unsigned int vertices_num, i, *sub_vertices_group_ptr;
	float modularity_delta;
	queue *groups_queue, *final_groups_queue;
	sparse_matrix *adjacency_matrix;
	array *sub_vertices_group;
	modularity_matrix* mod_matrix;
	leading_eigenpair* leading_pair;
	division *division;

	FILE* input_file;
	FILE* output_file;
	char r = 'r';

	/*int vertices[15] = {6,2,1,2,2,0,2,2,0,1,1,4,1,3,0};*/

	/*pre-process inputs before reading input into sparse matrix*/
	clock_t	start_time, end_time, max_end_time;

	input_file = fopen(argv[1], "r");
	assert(input_file != NULL);
	output_file = fopen(argv[2], "w");

	start_time = clock();

	setvbuf(stdout, NULL, _IONBF, 0);

	assert(argc > 0);

	/*fwrite(vertices, sizeof(int), 15, input_file);*/

	n = fread(&vertices_num, sizeof(unsigned int), 1, input_file);
	check_reading_writing(n, 1, r);

	groups_queue = (queue*)calloc(vertices_num, sizeof(queue));
	groups_queue->list = (linked_list_of_arrays*)calloc(vertices_num, sizeof(linked_list));
	groups_queue->size = 0;

	sub_vertices_group = (array*)calloc(vertices_num, sizeof(array));
	sub_vertices_group->array = (unsigned int*)calloc(vertices_num, sizeof(unsigned int));
	sub_vertices_group->size = vertices_num;
	sub_vertices_group_ptr = sub_vertices_group->array;

	for (i = 0; i < sub_vertices_group->size; i++) {
		/*printf("i is %d\n", i);*/
		*sub_vertices_group_ptr = i;
		sub_vertices_group_ptr++;
		/*printf("all_vertices_ptr[i] is %d \n", all_vertices->array[i]);*/
	}
	push(groups_queue, sub_vertices_group);

	final_groups_queue = (queue*)calloc(vertices_num, sizeof(queue));
	final_groups_queue->list = (linked_list_of_arrays*)calloc(vertices_num, sizeof(linked_list));
	final_groups_queue->size = 0;

	adjacency_matrix = read_input_into_sparse(input_file, vertices_num);
	/*printf("%s\n","finishing read input into sparse");*/


	while (groups_queue->size > 0) {
		sub_vertices_group = pop(groups_queue);

		mod_matrix = create_modularity_matrix(adjacency_matrix, sub_vertices_group);

		/*printf("%s\n","finishing create modularity matrix");*/

		leading_pair = create_eigenpair(mod_matrix);
		/*printf("%s\n","finishing eigenpair");*/


		/*if IS_NOT_POSITIVE(leading_pair->leading_eigenvalue){
			printf("%s\n","eigenvalue is negative - moving to next group in g");
			push(O, sub_vertices_group);
			continue;
		}*/

		/*printf("%s\n","starting modularity delta");*/

		/*modularity_delta = calc_modularity_delta(leading_pair);*/

		/*printf("modularity_delta is %f\n",modularity_delta);
		printf("%s\n","finishing modularity delta");*/

		/*if IS_NOT_POSITIVE(modularity_delta){
			printf("%s\n","modularity delta is negative - moving to next group in g");
			push(final_groups_queue, sub_vertices_group);
			continue;
		}*/

		end_time = clock();

		modularity_delta = improve_modularity(leading_pair);

		while (!IS_NOT_POSITIVE(modularity_delta)) {
			modularity_delta = improve_modularity(leading_pair); /*algorithm 4*/
		}

		max_end_time = clock();

		printf("maximize modularity execution took %f\n", ((float)(max_end_time-end_time) / CLOCKS_PER_SEC));

		division = create_division(leading_pair->division_vector, sub_vertices_group);

		free_leading_eigenpair(leading_pair);
		free_modularity_matrix(mod_matrix);
		/* free_array(sub_vertices_group); */

		/*handle a case when one sub group is empty*/
		if (division->g1->size == 0) {
			free_array(division->g1);
			push(final_groups_queue, division->g2);
			/*printf("added orignal group of size %d to final_groups_queue\n",division->g2->size);*/
		}

		else if (division->g2->size == 0) {
			free_array(division->g2);
			push(final_groups_queue, division->g1);
			/*printf("added orignal group of size %d to final_groups_queue\n",division->g1->size);*/
		}

		else {
			if (division->g1->size == 1){
				push(final_groups_queue, division->g1);
				/*printf("added g1 of size %d to O\n",division->g1->size);*/

			}
			else {
				push(groups_queue, division->g1);
				/*printf("added g1 of size %d to P\n",division->g1->size);*/
			}
			if (division->g2->size == 1){
				push(final_groups_queue, division->g2);
				/*printf("added g2 of size %d to O\n",division->g2->size);*/
			}
			else {
				push(groups_queue, division->g2);
				/*printf("added g2 of size %d to P\n",division->g2->size);*/
			}
		}

		free_division(division);


		/*printf("size of final_groups_queue is %d, size of P is %d\n",final_groups_queue->size, groups_queue->size);*/
	}

	create_output_graph(final_groups_queue, output_file);

	free_queue(groups_queue);
	free_queue(final_groups_queue);
	free_sparse_matrix(adjacency_matrix);

	fclose(input_file);
	fclose(output_file);

	end_time = clock();

	printf("Execution time is %f seconds\n", ((float)(end_time-start_time) / CLOCKS_PER_SEC));

	return EXIT_SUCCESS;

}


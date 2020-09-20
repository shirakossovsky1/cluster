/*
 ============================================================================
 Name        : Cluster
 Author      : Noam Keiny & Shira Kossovsky
 ============================================================================
 */

#include "leading_eigenpair.h"
#include "maximize_modularity.h"
#include "division.h"

#define IS_NOT_POSITIVE(X) ((X) <= 0.00001)
#define IS_POSITIVE(X) ((X) > 0.00001)

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

	clock_t	start_time, end_time;

	FILE *input_file, *output_file;

	/* read inputs and check variables */
	input_file = fopen(argv[1], "r");
	check_input_file(input_file);
	output_file = fopen(argv[2], "wb");
	check_variables(argc);

	start_time = clock();

	n = fread(&vertices_num, sizeof(unsigned int), 1, input_file);
	check_reading_writing(n, 1, 'r');

	/* initialize queue for subgroups that their modularity can be improved */
	groups_queue = (queue*)calloc(vertices_num, sizeof(queue));
	groups_queue->list = (linked_list_of_arrays*)calloc(vertices_num, sizeof(linked_list));
	groups_queue->size = 0;

	/* initialize group that holds the current vertices */
	sub_vertices_group = (array*)calloc(vertices_num, sizeof(array));
	sub_vertices_group->array = (unsigned int*)calloc(vertices_num, sizeof(unsigned int));
	sub_vertices_group->size = vertices_num;
	sub_vertices_group_ptr = sub_vertices_group->array;

	for (i = 0; i < sub_vertices_group->size; i++) {
		*sub_vertices_group_ptr = i;
		sub_vertices_group_ptr++;
	}
	push(groups_queue, sub_vertices_group);

	/* initialize queue for the final communities */
	final_groups_queue = (queue*)calloc(vertices_num, sizeof(queue));
	final_groups_queue->list = (linked_list_of_arrays*)calloc(vertices_num, sizeof(linked_list));
	final_groups_queue->size = 0;

	adjacency_matrix = read_input_into_sparse(input_file, vertices_num);

	/* for each group in the group's queue - the algorithm calculate the optimal division
	 * and divide the group accordingly */
	while (groups_queue->size > 0) {

		/* calculate group's modularity */
		sub_vertices_group = pop(groups_queue);
		mod_matrix = create_modularity_matrix(adjacency_matrix, sub_vertices_group);
		leading_pair = create_eigenpair(mod_matrix);
		modularity_delta = calc_modularity_delta(leading_pair);

		/* if eigenvalue or modularity are negative, the group is indivisible */
		if (IS_NOT_POSITIVE(leading_pair->leading_eigenvalue) ||
				IS_NOT_POSITIVE(modularity_delta)) {
			change_to_homogenic_devision(leading_pair->division_vector, sub_vertices_group->size);
		}

		/* improve group's modularity, only if eigenvalue is positive */
		if (IS_POSITIVE(leading_pair->leading_eigenvalue)) {
			modularity_delta = improve_modularity(leading_pair);
			while (IS_POSITIVE(modularity_delta)) {
				modularity_delta = improve_modularity(leading_pair);
			}
		}

		/* create division into two new groups */
		division = create_division(leading_pair->division_vector, sub_vertices_group);

		free_leading_eigenpair(leading_pair);
		free_modularity_matrix(mod_matrix);

		/* handle a case when one sub-group is empty, insert into relevant queues */
		if (division->g1->size == 0) {
			free_array(division->g1);
			push(final_groups_queue, division->g2);
		}

		else if (division->g2->size == 0) {
			free_array(division->g2);
			push(final_groups_queue, division->g1);
		}

		/* insert the new sub-groups into the relevant queues */
		else {
			if (division->g1->size == 1){
				push(final_groups_queue, division->g1);
			}
			else {
				push(groups_queue, division->g1);
			}
			if (division->g2->size == 1){
				push(final_groups_queue, division->g2);
			}
			else {
				push(groups_queue, division->g2);
			}
		}

		free_division(division);
	}

	create_output_graph(final_groups_queue, output_file);

	/* free memory and close files */
	free_queue(groups_queue);
	free_queue(final_groups_queue);
	free_sparse_matrix(adjacency_matrix);

	fclose(input_file);
	fclose(output_file);

	end_time = clock();

	printf("Execution time is %f seconds\n", ((float)(end_time-start_time) / CLOCKS_PER_SEC));

	return EXIT_SUCCESS;

}


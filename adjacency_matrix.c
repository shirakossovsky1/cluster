/*
 * input_matrix.c
 *
 *  Created on: 14 Aug 2020
 *      Author: shira
 */

#include "adjacency_matrix.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

sparse_matrix* read_input_into_sparse(FILE* input_file, unsigned int vertices_num) {
	unsigned int k = 0, n = 0, vertex_degree, total_degrees = 0, *degrees_vector, *degree_vector_ptr;
	linked_list *curr_row, *rows, *curr_list;
	sparse_matrix *adjancency_mat;

	rows = (linked_list*)malloc(sizeof(linked_list) * vertices_num);
	assert(rows != NULL);

	degrees_vector = (unsigned int*)calloc(sizeof(unsigned int), vertices_num);
	assert(degrees_vector != NULL);

	curr_row = rows;
	degree_vector_ptr = degrees_vector;

	adjancency_mat = (sparse_matrix*)malloc(sizeof(sparse_matrix));
	assert(adjancency_mat != NULL);

	adjancency_mat -> dim = vertices_num;
	adjancency_mat -> rows = rows;

	for (k = 0; k < vertices_num; k++) {

		n = fread(&vertex_degree, sizeof(unsigned int), 1, input_file);
		check_reading_writing(n, 1, 'r');

		curr_list = (linked_list*)malloc(sizeof(linked_list));

		add_row(input_file, vertex_degree,curr_list);
		*curr_row = *curr_list;

		curr_row++;

		/*insert value into degrees vector*/
		*degree_vector_ptr = vertex_degree;
		degree_vector_ptr++;

		/*printf("degrees_vector[%d] is %d \n", k, degrees_vector[k]);*/

		/*calculate total degrees of the graph*/
		total_degrees += vertex_degree;
	}

	adjancency_mat -> total_degrees = total_degrees;
	adjancency_mat -> degrees_vector = degrees_vector;
	/*printf("total_degrees = %d\n", adjancency_mat -> total_degrees);*/

	return adjancency_mat;
}

linked_list* add_row(FILE* input_file, unsigned int vertex_degree, linked_list *curr_list){

	unsigned int 	j = 0, neighbors, *neighbors_arr, *curr_neighbor;

	/* allocate a new linked list */

	curr_list -> head = NULL;
	curr_list -> tail = NULL;

	if (vertex_degree == 0) {
		return curr_list;
	}

	/* read vertex's neighbors into array */
	neighbors_arr = (unsigned int*)malloc(sizeof(unsigned int)*vertex_degree);
	assert(neighbors_arr != NULL);
	neighbors = fread(neighbors_arr, sizeof(unsigned int), vertex_degree, input_file);
	check_reading_writing(neighbors, vertex_degree, 'r');
	curr_neighbor = neighbors_arr;

	/* add all non zero row values to the link list */
	for (j = 0; j < neighbors; j++){
		add_node(*curr_neighbor, curr_list);

		/*printf("curr_neighbor = %d\n", *curr_neighbor);*/
		curr_neighbor++;
	}

	free(neighbors_arr);

	return curr_list;
}

/* insert a new node to the link list */

void add_node(unsigned int curr_vertex, linked_list *curr_list){

	node *new_node;

	/*printf("curr_vertex = %d\n", curr_vertex);*/
	/* create a new node */
	new_node = (node*)malloc(sizeof(node));
	assert(new_node != NULL);

	/* insert data */
    new_node -> matrix_index = curr_vertex;
    new_node -> next = NULL;

	/* if link list is empty insert at start */
    if (curr_list -> head == NULL) {
    	curr_list -> head = new_node;
       }

    /* else insert to the end */
    else {
    	curr_list -> tail -> next = new_node;
    }

    curr_list -> tail = new_node;
}

void write_input_matrix(sparse_matrix sparse_mat, FILE* output_file){
	unsigned int n, row = 0;
	node *node;

	/*printf("%s\n","starting");*/
	for(row = 0; row < sparse_mat.dim; row++){
		/*printf("row = %d\n", row);*/
		n = fwrite(&row, sizeof(unsigned int), 1, output_file);
		check_reading_writing(n, 1, 'w');

		node = (sparse_mat.rows)[row].head;
		/*printf("rows[row] head = %d\n", node.matrix_index);*/
		do {
			/*printf("node.matrix_index = %d\n", node -> matrix_index);*/
			n = fwrite(&(node -> matrix_index), sizeof(unsigned int), 1, output_file);
			check_reading_writing(n, 1, 'w');
			node = node -> next;
		}
		while (node != NULL);
		/*fwrite("\n", sizeof(char), 1, output_file);*/
	}
}

/*void print_degrees_vector(sparse_matrix sparse_mat){
	int* degrees_vector;
	int vertex;

	degrees_vector = sparse_mat.degrees_vector;

	printf("%s\n","starting");
	for(vertex = 0; vertex < sparse_mat.dim; vertex++){
		printf("degree for vertex %d is %d\n", vertex, degrees_vector[vertex]);
	}
}*/

void free_sparse_matrix(sparse_matrix *sparse_mat){

	unsigned int 	i = 0;
	linked_list 	*list, *tmp_list;

	/* free degrees vector */
	free(sparse_mat->degrees_vector);

	/* free rows and nodes */
	list = sparse_mat -> rows;
	for (i = 0 ; i < sparse_mat -> dim ; i++){
		tmp_list = list+1;
		free_linked_list(list);
		list = tmp_list;
	}
	free(sparse_mat->rows);
	free(sparse_mat);
}



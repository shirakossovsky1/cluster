
#include "adjacency_matrix.h"

/* main function which reads the input file into sparse matrix object,
 * calculates total degrees amount and updates the degrees vector.
 */
sparse_matrix* read_input_into_sparse(FILE* input_file, unsigned int vertices_num) {

	unsigned int k = 0, n = 0, vertex_degree, total_degrees = 0, *degrees_vector, *degree_vector_ptr;
	linked_list *curr_row, *rows;
	sparse_matrix *adjancency_mat;

	rows = (linked_list*)calloc(sizeof(linked_list), vertices_num);
	curr_row = rows;

	degrees_vector = (unsigned int*)calloc(sizeof(unsigned int), vertices_num);
	degree_vector_ptr = degrees_vector;

	adjancency_mat = (sparse_matrix*)malloc(sizeof(sparse_matrix));
	adjancency_mat -> dim = vertices_num;
	adjancency_mat -> rows = rows;

	for (k = 0; k < vertices_num; k++) {

		n = fread(&vertex_degree, sizeof(int), 1, input_file);
		check_reading_writing(n, 1, 'r');

		add_row(input_file, vertex_degree, curr_row);
		curr_row++;

		/* insert value into degrees vector */
		*degree_vector_ptr = (unsigned int)vertex_degree;
		degree_vector_ptr++;

		/* calculate total degrees of the graph */
		total_degrees += vertex_degree;
	}

	adjancency_mat -> total_degrees = total_degrees;
	adjancency_mat -> degrees_vector = degrees_vector;

	return adjancency_mat;
}

/* insert a row into adjacency matrix (from the input file) */
linked_list* add_row(FILE* input_file, unsigned int vertex_degree, linked_list *curr_list){

	unsigned int j = 0, neighbors, *neighbors_arr, *curr_neighbor;

	/* allocate a new linked list */
	curr_list -> head = NULL;
	curr_list -> tail = NULL;

	if (vertex_degree == 0) {
		return curr_list;
	}

	/* read vertex's neighbors into array */
	neighbors_arr = (unsigned int*)malloc(sizeof(unsigned int)*vertex_degree);
	neighbors = fread(neighbors_arr, sizeof(int), vertex_degree, input_file);
	check_reading_writing(neighbors, vertex_degree, 'r');
	curr_neighbor = neighbors_arr;

	/* add all non zero row values to the link list */
	for (j = 0; j < neighbors; j++){
		add_node(*curr_neighbor, curr_list);
		curr_neighbor++;
	}

	free(neighbors_arr);

	return curr_list;
}

/* insert a new neighbor into the vertex's neighbors list */
void add_node(unsigned int curr_vertex, linked_list *curr_list){

	node *new_node;

	/* create a new node */
	new_node = (node*)malloc(sizeof(node));

	/* insert data */
    new_node -> data = curr_vertex;
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

/* free memory of sparse matrix object */
void free_sparse_matrix(sparse_matrix *sparse_mat){

	unsigned int 	i = 0;
	linked_list 	*list;

	/* free degrees vector */
	free(sparse_mat->degrees_vector);

	/* free rows and nodes */
	list = sparse_mat -> rows;
	for (i = 0 ; i < sparse_mat -> dim ; i++){
		list = &sparse_mat->rows[i];
		free_linked_list(list);
	}
	free(sparse_mat->rows);
	free(sparse_mat);
}



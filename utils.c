
#include "utils.h"

/* remove and return the first item in the queue */
array* pop(struct queue *queue) {

	node_of_array *first_node, *second_node;
	array* result;

	first_node = queue->list->head;

	if (first_node == NULL) {
		return NULL;
	}
	else if (first_node == queue->list->tail){
		queue->list->head = NULL;
		queue->list->tail = NULL;
	}
	else {
		second_node = first_node->next;
		queue->list->head = second_node;
	}

	result = first_node->data;
	free(first_node);
	queue->size -= 1;

	return result;
}

/* add a new item to the queue */
void push(struct queue *queue, array* item) {

	node_of_array *new_node, *last_node;

	new_node = (node_of_array*)malloc(sizeof(node_of_array));
	new_node->data = item;
	new_node->next = NULL;

	if (queue->list->head != NULL) {
		last_node = queue->list->tail;
		last_node->next = new_node;
	}
	else {
		queue->list->head = new_node;
	}

	queue->list->tail = new_node;
	queue->size += 1;

	return;
}

/* write the graph final division into binary file */
void create_output_graph(queue *O, FILE* output_file){
	unsigned int i, n, size_of_subgroup;
	node_of_array *curr_node_array;

	n = fwrite(&(O->size), sizeof(int), 1, output_file);
	check_reading_writing(n, 1, 'w');

	curr_node_array = O->list->head;
	for (i = 0; i < O->size; i++) {
		size_of_subgroup = curr_node_array ->data->size;
		n = fwrite(&size_of_subgroup, sizeof(unsigned int), 1, output_file);
		check_reading_writing(n, 1, 'w');
		n = fwrite((curr_node_array->data->array), sizeof(unsigned int), size_of_subgroup, output_file);
		check_reading_writing(n, size_of_subgroup, 'w');
		curr_node_array = curr_node_array->next;
	}
}

/* free memory of array object */
void free_array(array *array){

	free(array->array);
	free(array);
}

/* free memory of queue object */
void free_queue(queue *queue){

	free_linked_list_of_arrays(queue->list);
	free(queue);
}

/* free memory of linked list of arrays object */
void free_linked_list_of_arrays(linked_list_of_arrays *linked_list){

	node_of_array *node,*tmp_node;
	node = linked_list->head;

	while(node != NULL){
		tmp_node = node;
		node = node->next;
		free_node_of_array(tmp_node);
	}

	free(linked_list);
}

/* free memory of node of array object */
void free_node_of_array(node_of_array *node){

	free_array(node->data);
	free(node);
}

/* free memory of max in array object */
void free_max_in_array(max_in_array *max_in_array) {

	free(max_in_array->array);
	free(max_in_array);
}

/* free memory of linked list object */
void free_linked_list(linked_list *linked_list) {

	node 	*node, *tmp_node;
	node = linked_list->head;

	while(node != NULL){
		tmp_node = node;
		node = node->next;
		free(tmp_node);
	}
}

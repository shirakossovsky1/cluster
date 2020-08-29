/*
 * utils.c
 *
 *  Created on: 22 Aug 2020
 *      Author: shira
 */

#include "utils.h"

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

void create_output_graph(queue *O, FILE* output_file){
	unsigned int i, size_of_subgroup;
	node_of_array *curr_node_array;

	fwrite(&(O->size), sizeof(int), 1, output_file);

	curr_node_array = O->list->head;
	for (i = 0; i < O->size; i++) {
		size_of_subgroup = curr_node_array ->data->size;
		fwrite(&size_of_subgroup, sizeof(unsigned int), 1, output_file);
		fwrite((curr_node_array->data->array), sizeof(unsigned int), size_of_subgroup, output_file);
		curr_node_array = curr_node_array->next;
		/*for (j = 0; j < size_of_subgroup; j++) {
			fwrite((curr_node_array->data->array), sizeof(int), size_of_subgroup, output_file);*/
	}
}

void free_array(array *array){
	unsigned int i;
	printf("%s","array is ");
	for (i=0; i < array->size;i++){
		printf("%d ",array->array[i]);
	}
	printf("%s","\n");
	free(array->array);
	free(array);
}

void free_queue(queue *queue){
	free_linked_list_of_arrays(queue->list);
	free(queue);
}

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

void free_node_of_array(node_of_array *node){
	free_array(node->data);
	free(node);
}

void free_max_in_array(max_in_array *max_in_array) {
	free(max_in_array->array);
	free(max_in_array);
}

void free_linked_list(linked_list *linked_list) {
	node 			*node, *tmp_node;

	node = linked_list->head;
	while(node != NULL){
		tmp_node = node;
		node = node->next;
		free(tmp_node);
	}

	/*free(linked_list);*/
}


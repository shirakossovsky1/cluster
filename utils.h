/*
 * The module contains core, general data structures and functions.
 * These are inner implementations used in the project.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "error.h"

/* max_in_array object used to store an array data structure and its maximal value and index */
typedef struct max_in_array{
	unsigned int 	arg_max; /* A as a sparse matrix */
	float 			max_value;
	float 			*array;
} max_in_array;

/* array object used to store an array data structure and its size */
typedef struct array{
	unsigned int 	size;
	unsigned int 	*array;
} array;

/* node object used as the known implementation of linked list sub unit */
typedef struct node{
	unsigned int 	data;
    struct node 	*next;
} node;

/* known linked list data structure of "node"s*/
typedef struct linked_list{
	node 			*head;
	node 			*tail;
} linked_list;

/* node that its data is an array */
typedef struct node_of_array{
	array 				 *data;
    struct node_of_array *next;
} node_of_array;

/* known linked list data structure of "node of array"s*/
typedef struct linked_list_of_arrays{
	node_of_array 		*head;
	node_of_array 		*tail;

} linked_list_of_arrays;

/* known queue data structure */
typedef struct queue{
	unsigned int 			size;
	linked_list_of_arrays*	list;
} queue;

void create_output_graph(queue *O, FILE* output_file);

array* 	pop(struct queue *queue);
void 	push(struct queue *queue, array* item);

/* free functions */
void free_max_in_array(max_in_array *max_in_array);
void free_array(array *array);
void free_linked_list(linked_list *linked_list);
void free_linked_list_of_arrays(linked_list_of_arrays *linked_list_of_arrays);
void free_node_of_array(node_of_array *node_of_array);
void free_queue(queue *queue);

#endif /* UTILS_H_ */

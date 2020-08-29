/*
 * utils.h
 *
 *  Created on: 22 Aug 2020
 *      Author: shira
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

typedef struct max_in_array{
	unsigned int 	arg_max; /* A as a sparse matrix */
	float 			max_value;
	float 			*array;
} max_in_array;

/*typedef struct array_of_arrays{
	unsigned int 	size;
	unsigned int 	**array;
} array_of_arrays;*/

typedef struct array{
	unsigned int 	size;
	unsigned int 	*array;
} array;

typedef struct node{
	unsigned int 	matrix_index;
    struct node 	*next;
} node;

/* linked list object */
typedef struct linked_list{
	node 			*head;
	node 			*tail;

} linked_list;

/* node object for linked list */
typedef struct node_of_array{
	array 				 *data;
    struct node_of_array *next;
} node_of_array;

/* linked list object */
typedef struct linked_list_of_arrays{
	node_of_array 		*head;
	node_of_array 		*tail;

} linked_list_of_arrays;

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

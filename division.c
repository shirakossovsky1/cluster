/*
 * division.c
 *
 *  Created on: 22 Aug 2020
 *      Author: shira
 */

#include "division.h"

division* create_division(float* division_vector, array* original_subgroup) {
	array *g1, *g2;
	division *curr_division;

	/*printf("%s\n","&&&&&&&&& starting inside division file &&&&&&&&&");*/

	curr_division = (division*)malloc(sizeof(division));
	g1 = (array*)malloc(sizeof(array));
	g2 = (array*)malloc(sizeof(array));
	curr_division->g1 = g1;
	curr_division->g2 = g2;
	curr_division->division_vector = division_vector;

	divide(curr_division, original_subgroup);

	/*printf("%s\n","&&&&&&&&& finishing inside division file &&&&&&&&&");*/

	return curr_division;

}

void divide(division *curr_division, array* original_subgroup) {
	unsigned int i, size_of_g1, size_of_g2;
	unsigned int *original_sugroup_ptr, *g1_ptr, *g2_ptr;
	float* division_vector_ptr;

	division_vector_ptr = curr_division->division_vector;
	original_sugroup_ptr = original_subgroup->array;

	size_of_g1 = 0;
	size_of_g2 = 0;

	for (i = 0; i < original_subgroup->size; i++) {
			if (*division_vector_ptr == 1.0) {
				size_of_g1++;
			}
			else if (*division_vector_ptr == -1.0) {
				size_of_g2++;
			}
			division_vector_ptr++;
	}

	division_vector_ptr = curr_division->division_vector;
	curr_division->g1->array = (unsigned int*)calloc(sizeof(unsigned int), size_of_g1);
	curr_division->g2->array = (unsigned int*)calloc(sizeof(unsigned int), size_of_g2);
	curr_division->g1->size = size_of_g1;
	curr_division->g2->size = size_of_g2;

	g1_ptr = curr_division->g1->array;
	g2_ptr = curr_division->g2->array;

	for (i = 0; i < original_subgroup->size; i++) {
		if (*division_vector_ptr == 1.0) {
			*(g1_ptr) = *original_sugroup_ptr;
			g1_ptr++;
		}
		else if (*division_vector_ptr == -1.0) {
			*(g2_ptr) = *original_sugroup_ptr;
			g2_ptr++;
		}
		division_vector_ptr++;
		original_sugroup_ptr++;
	}
}

void free_division(division *division){
	free(division->division_vector);
	free(division);

}




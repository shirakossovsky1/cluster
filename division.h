/*
 * The module represents the division of the given group into 2 sub-groups,
 * based on the optimized division vector.
 */

#ifndef DIVISION_H_
#define DIVISION_H_

#include "utils.h"

/* division object
	division_vector - vector which represents the division indices based on the leading eigenvector (s).
	g1, g2 - the new sub-groups implemented by an array object.
 */
typedef struct division{
	float 			*division_vector;
	array 			*g1;
	array 			*g2;
} division;

division* create_division(float* division_vector, array* original_subgroup);
void divide(division *curr_division, array* original_subgroup);
void free_division(division *division);
void change_to_homogenic_devision(float* division_vector, unsigned int vec_size);

#endif /* DIVISION_H_ */

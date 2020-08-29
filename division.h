/*
 * division.h
 *
 *  Created on: 22 Aug 2020
 *      Author: shira
 */

#ifndef DIVISION_H_
#define DIVISION_H_

#include "utils.h"

typedef struct division{
	float 			*division_vector;
	array 			*g1;
	array 			*g2;
} division;

division* create_division(float* division_vector, array* original_subgroup);
void divide(division *curr_division, array* original_subgroup);
void free_division(division *division);

#endif /* DIVISION_H_ */

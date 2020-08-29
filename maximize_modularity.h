/*
 * maximize_modularity.h
 *
 *  Created on: 18 Aug 2020
 *      Author: shira
 */

#ifndef MAXIMIZE_MODULARITY_H_
#define MAXIMIZE_MODULARITY_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "leading_eigenpair.h"

float calc_modularity_delta(leading_eigenpair* eigenpair);
float improve_modularity(leading_eigenpair* eigenpair);
void find_maximal_score(max_in_array *max_score, unsigned int group_size, unsigned int* unmoved, leading_eigenpair* eigenpair, float q0);
float calc_final_division(unsigned int group_size, max_in_array *max_improvemnt, float *division_vector, unsigned int *indices);


#endif /* MAXIMIZE_MODULARITY_H_ */

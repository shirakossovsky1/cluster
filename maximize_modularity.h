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

double calc_modularity_delta(leading_eigenpair* eigenpair);
double improve_modularity(leading_eigenpair* eigenpair);
void find_maximal_score(max_in_array *max_score, int group_size, int* unmoved, leading_eigenpair* eigenpair, double q0);
double calc_final_division(int group_size, max_in_array *max_improvemnt, double *division_vector, int *indices);


#endif /* MAXIMIZE_MODULARITY_H_ */

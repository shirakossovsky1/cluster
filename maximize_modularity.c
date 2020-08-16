/*
 * maximize_modularity.c
 *
 *  Created on: 16 Aug 2020
 *      Author: shira
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "leading_eigenpair.h"
#include "modularity_matrix.h"

double calc_modularity_delta(modularity_matrix mod_matrix, leading_eigenpair eigenpair) {

	double tmp_vec;
    double b;
    double c;

	tmp_vec = (double*)malloc(sizeof(double) * (eigenpair -> mod_matrix.sub_vertices_group_size));

	tmp_vec = mult_matrix_by_vector(eigenpair -> mod_matrix, eigenpair -> division_vector, tmp_vec, eigenpair -> mod_matrix.sub_vertices_group_size);
	b = dot_product(eigenpair -> division_vector, tmp_vec);

	 free(tmp_vec);

	 return 0.5 * b;
}


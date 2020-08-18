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
#include "modularity_matrix.h"

double calc_modularity_delta(leading_eigenpair* eigenpair);


#endif /* MAXIMIZE_MODULARITY_H_ */

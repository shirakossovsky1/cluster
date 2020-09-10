/*
 * The module performs a method to further optimize a division.
 * the process follows algorithm 4 and changes the division vector inplace accordingly.
 */

#ifndef MAXIMIZE_MODULARITY_H_
#define MAXIMIZE_MODULARITY_H_

#include "leading_eigenpair.h"

/* optimization set object that stores the relevant properties for the optimization process
	unmoved - array indicates wether a vertex was moved or not.
	score - stores the modularity difference gotten by changing the vertex's group.
	improvement - index i contains the maximal score reached in iteration i.
	indices - index i contains the indice of vertex with the maximal score reached in iteration i.
 */
typedef struct optimization_set {
	unsigned int*	unmoved;
	max_in_array* 	score;
	max_in_array*	improvement;
	unsigned int*	indices;

} optimization_set;

optimization_set* create_optimization_set(int group_size);
float improve_modularity(leading_eigenpair* eigenpair);
void calc_score_first_iteration(max_in_array *score, unsigned int group_size, leading_eigenpair* eigenpair);
void calc_score(max_in_array *score, unsigned int group_size, unsigned int* unmoved, leading_eigenpair* eigenpair);
float calc_final_division(unsigned int group_size, max_in_array *max_improvemnt, float *division_vector, unsigned int *indices);
void free_optimization_set(optimization_set *opt);

#endif /* MAXIMIZE_MODULARITY_H_ */

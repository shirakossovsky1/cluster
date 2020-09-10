/*
 * The module holds all the properties that relevant for eigenpair.
 * calculate the leading eigenvector, eigenvalue and the
 * division vector based on the leading eigenvector.
 */

#ifndef LEADING_EIGENPAIR_H_
#define LEADING_EIGENPAIR_H_

#include "power_iteration.h"
#include "modularity_matrix.h"


/* modularity matrix object of vertices sub-group (Bg-hat).
	leading_eigenvector - eigenvector with the maximal eigenvalue of the modularity matrix.
	leading_eigenvalue - maximal eigen value of the modularity matrix.
	mod_matrix - a pointer to a modularity matrix object.
	division_vector - vector which represents the division indices based on the leading eigenvector (s).
 */
typedef struct leading_eigenpair{

	float 			  *leading_eigenvector;
	float 			  leading_eigenvalue;
	modularity_matrix *mod_matrix;
	float 			  *division_vector;

}leading_eigenpair;

leading_eigenpair* create_eigenpair(modularity_matrix* mod_matrix);
float find_leading_eigenvalue(leading_eigenpair* eigenpair);
float* find_leading_eigenvector(modularity_matrix* mod_matrix);
float* calc_division_vector(leading_eigenpair* eigenpair);
void free_leading_eigenpair(leading_eigenpair* eigenpair);

#endif /* LEADING_EIGENPAIR_H_ */

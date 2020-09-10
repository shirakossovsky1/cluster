
#include "division.h"

/* main function which calculates the division of the given group. */
division* create_division(float* division_vector, array* original_subgroup) {

	array *g1, *g2;
	division *curr_division;

	curr_division = (division*)malloc(sizeof(division));
	g1 = (array*)malloc(sizeof(array));
	g2 = (array*)malloc(sizeof(array));
	curr_division->g1 = g1;
	curr_division->g2 = g2;
	curr_division->division_vector = division_vector;

	divide(curr_division, original_subgroup);

	return curr_division;

}

/* divide the origin sub-group into 2 new arrays */
void divide(division *curr_division, array* original_subgroup) {
	unsigned int i, size_of_g1, size_of_g2;
	unsigned int *original_sugroup_ptr, *g1_ptr, *g2_ptr;
	float* division_vector_ptr;

	division_vector_ptr = curr_division->division_vector;
	original_sugroup_ptr = original_subgroup->array;

	size_of_g1 = 0;
	size_of_g2 = 0;

	/* calculate each sub-group size */
	for (i = 0; i < original_subgroup->size; i++) {
		if (*division_vector_ptr == 1.0) {
			size_of_g1++;
		}
		else if (*division_vector_ptr == -1.0) {
			size_of_g2++;
		}
		division_vector_ptr++;
	}

	/* allocate memory */
	division_vector_ptr = curr_division->division_vector;
	curr_division->g1->array = (unsigned int*)calloc(sizeof(unsigned int), size_of_g1);
	curr_division->g2->array = (unsigned int*)calloc(sizeof(unsigned int), size_of_g2);
	curr_division->g1->size = size_of_g1;
	curr_division->g2->size = size_of_g2;

	g1_ptr = curr_division->g1->array;
	g2_ptr = curr_division->g2->array;

	/* fill each sub-group array with the matching vertices */
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

/* free memory of division object */
void free_division(division *division){
	free(division->division_vector);
	free(division);

}




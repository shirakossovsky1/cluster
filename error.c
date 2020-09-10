
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

/* handle a case when the input file doesn't exist or corrupted */
void check_input_file(FILE* input_file) {
	if (input_file == NULL) {
		printf("%s\n", "Corrupted file, exiting program");
		exit(1);
	}
	return;
}

/* handle a case when the input variables do not exist */
void check_variables(int argc) {
	if (argc == 0) {
		printf("%s\n", "Progrem didn't recieve any variables, exiting program");
		exit(1);
	}
	return;
}

/* handle a case when division by zero is done */
void check_float_division_by_zero(float number) {
	if (number == 0.0) {
		printf("%s\n", "Dividing by zero, exiting program");
		exit(1);
	}
	return;
}

/* handle a case when division by zero is done */
void check_int_division_by_zero(unsigned int number) {
	if (number == 0) {
		printf("%s\n", "Dividing by zero, exiting program");
		exit(1);
	}
	return;
}

/* handle a case when reading or writing couldn't be done properly */
void check_reading_writing(int number, int expected, char c) {
	if (number != expected) {
		if (c == 'r') {
			printf("Read %d objects instead of %d objects, exiting program\n", number, expected);
		}
		else if (c == 'w') {
			printf("Wrote %d objects instead of %d objects, exiting program\n", number, expected);
		}
		exit(1);
	}
	return;
}

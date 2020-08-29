/*
 * error.c
 *
 *  Created on: 29 Aug 2020
 *      Author: shira
 */
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void check_float_division_by_zero(float number) {
	if (number == 0.0) {
		printf("%s\n", "Dividing by zero, exiting program");
		EXIT_FAILURE;
	}
	return;
}

void check_int_division_by_zero(unsigned int number) {
	if (number == 0) {
		printf("%s\n", "Dividing by zero, exiting program");
		EXIT_FAILURE;
	}
	return;
}

void check_reading_writing(int number, int expected, char c) {
	if (number != expected) {
		if (c == 'r') {
			printf("Read %d objects instead of %d objects, exiting program\n", number, expected);
		}
		else if (c == 'w') {
			printf("Wrote %d objects instead of %d objects, exiting program\n", number, expected);
		}
		EXIT_FAILURE;
	}
	return;
}

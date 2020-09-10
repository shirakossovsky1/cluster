/*
 * The module handles any logical error that may occur while running the project
 */

#ifndef ERROR_H_
#define ERROR_H_

void check_input_file(FILE* input_file);
void check_variables(int argc);
void check_float_division_by_zero(float number);
void check_int_division_by_zero(unsigned int number);
void check_reading_writing(int number, int expected, char c);

#endif /* ERROR_H_ */

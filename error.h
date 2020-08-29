/*
 * error.h
 *
 *  Created on: 29 Aug 2020
 *      Author: shira
 */

#ifndef ERROR_H_
#define ERROR_H_

void check_float_division_by_zero(float number);
void check_int_division_by_zero(unsigned int number);
void check_reading_writing(int number, int expected, char c);

#endif /* ERROR_H_ */

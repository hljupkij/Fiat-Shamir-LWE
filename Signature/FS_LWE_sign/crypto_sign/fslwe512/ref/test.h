/*
 * test.h
 *
 *  Created on: 10.07.2011
 *      Author: wladimir
 */

#ifndef TEST_H_
#define TEST_H_

#include <NTL/ZZ.h>
#include <stdio.h>
#include "aux.h"

/* Zählt alle Elemente mit einer bestimmten L1-Norm. */
NTL::ZZ test(int power, NTL::ZZ number_loop, NTL::ZZ length, NTL::ZZ modulus);

void test_distribution_Dc(int polynomial_degree, int max_length, int* results, int number_of_tries);

int test_conversion_ZZ_pE_to_char(int number_of_tests);

#endif /* TEST_H_ */

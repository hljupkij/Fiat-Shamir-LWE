/*
 * Distribution.h
 *
 *  Created on: 05.07.2011
 *      Author: wladimir
 */
#include <NTL/ZZ.h>
#include <stdio.h>
#include "aux.h"
#include <ostream>
#include <stdlib.h>

#ifndef DISTRIBUTION_H_
#define DISTRIBUTION_H_

class Distribution {

private:
	int element_length;
	int max_L1_norm;
	NTL::ZZ *abs;
	NTL::ZZ sum;
	double *rel;
	double *distribution;

public:
	/* create a empty distribution */
	Distribution();

	/* create a distribution with  */
	Distribution(int element_length, int max_L1_norm);

	virtual ~Distribution();

	/* return a random value accomplished to distribution. */
	int random();

	/* return number of elements with L1-norm equal to norm. */
	NTL::ZZ get_abs(int norm);

	/* return a number of elements in this distribution */
	NTL::ZZ get_sum();

	/* return a part of elements with L1-norm in percent */
	double get_rel(int norm);

	/* return a maximum L1 norm of this distribution */
	int get_max_L1_norm();

	/* return a length of elements (tupels) in this distribution  */
	int get_element_length();

	double get_distribution(int norm);

	int create_distribution_Dc(int polynomial_degree, int max_length);
};

#endif /* DISTRIBUTION_H_ */

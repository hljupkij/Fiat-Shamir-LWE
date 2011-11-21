/*
 * Distribution.cpp
 *
 *  Created on: 05.07.2011
 *      Author: wladimir
 */


#include "Distribution.h"
#include <stdio.h>
#include "aux.h"
#include <ostream>
#include <stdlib.h>
using namespace NTL;


Distribution::Distribution()
:element_length(0),max_L1_norm(0),sum(to_ZZ(1))
{
	abs = new ZZ[1];
	rel = new double[1];
	distribution = new double[1];
	abs[0] = to_ZZ(1);
	rel[0] = 1;
	distribution[0] = 1;
}

Distribution::Distribution(int element_length, int max_L1_norm)
:element_length(element_length),max_L1_norm(max_L1_norm),sum(ZZ::zero())
{
	abs = new ZZ[max_L1_norm+1];
	rel = new double[max_L1_norm+1];
	distribution = new double[max_L1_norm+1];

	for (int norm_length = 0; norm_length <= max_L1_norm; ++norm_length) {
		abs[norm_length] = count_elements_with_L1(element_length,norm_length);
		sum += abs[norm_length];
	}

	for (int index = 0; index <= max_L1_norm; ++index) {
		rel[index] = to_double(abs[index])/to_double(sum);
		if(index>0){
			distribution[index] = distribution[index-1]+rel[index];
		}else{
			distribution[0] = rel[0];
		}
	}
}

Distribution::~Distribution() {
	// TODO Auto-generated destructor stub
}

int Distribution::random(){
	float rand_ =(float)rand()/(float)RAND_MAX;
//	printf("rand:%f",rand_);
	int index = 1;

	while(index <= max_L1_norm){
		if((rand_ >= 0)&&(rand_ < distribution[0])) return 0;
		if(rand_ >= distribution[index-1] && rand_ <= distribution[index]){
			return index;
		}
		index++;
	}
	printf("Fehler! Random:%f\n",rand_);
	return 0;
}

ZZ Distribution::get_abs(int norm){

	if(norm<=max_L1_norm){
		return abs[norm];
	}else{
		return ZZ::zero();
	}
}

ZZ Distribution::get_sum(){
	return sum;
}

double Distribution::get_rel(int norm){
	if(norm<=max_L1_norm){
		return rel[norm];
	}else{
		return 0;
	}
}

int Distribution::get_max_L1_norm(){
	return max_L1_norm;
}

int Distribution::get_element_length(){
	return element_length;
}

double Distribution::get_distribution(int norm){
	if(norm<=max_L1_norm){
		return distribution[norm];
	}else{
		return 0;
	}
}
/*
int Distribution::create_distribution_Dc(int polynomial_degree, int max_length){
	ZZ sum;					// Gesamtanzahl der Elemente mit norm_1 <= max_length
	ZZ abs[max_length+1];	// Array mit Anzahl der Elemente mit norm_1 = index
	double probability[max_length+1];
	double distribution[max_length+1];
	double rand_;
	int state = -1;

	for (int norm_length = 0; norm_length <= max_length; ++norm_length) {
		abs[norm_length] = count_elements_with_L1(polynomial_degree, norm_length);
		sum +=abs[norm_length];
		cout<<"abs["<<norm_length<<"]:"<<abs[norm_length]<<"\tsum = "<<sum<<"\n";
	}

	probability[0] = 0;
	for (int i = 0; i <= max_length; ++i) {
		probability[i] = to_double(abs[i])/to_double(sum);
		distribution[i+1] = distribution[i]+probability[i];
		cout<<i<< "\tprobability:"<<probability[i]<<"\tdistribution: "<<distribution[i+1]<<"\n";
	}

	rand_ = (float)RandomBits_ulong(log(RAND_MAX)/log(2))/RAND_MAX;
	cout << "rand = "<<rand_<<"\n";

	for (int i = 0; i <= max_length; ++i) {
		if(rand_ < distribution[i+1] && rand_ > distribution[i]){
			state = i;
		}
	}

	cout<<"state: "<<state<<"\n";

	return EXIT_SUCCESS;
}
*/

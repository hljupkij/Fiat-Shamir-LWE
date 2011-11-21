/*
 * aux.cpp
 *
 *  Created on: 16.10.2011
 *      Author: Wladimir Paulsen
 */

#include "aux.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "api.h"

NTL_CLIENT

/* Setup a polynomial ring with (x^degree+1) as irreducible polynomial. */
int initialize_ZZ_pE(ZZ modulus_ZZ_pE, int degree){
	ZZ_p::init(modulus_ZZ_pE);				// define GF(p)

	ZZ_pX P = ZZ_pX(degree,to_ZZ_p(1));		// define P as x^n
	SetCoeff(P,0);							// set x^0 to 1

	ZZ_pE::init(P);							// set P as modulus for polynomials
	P.kill();
	return EXIT_SUCCESS;
}

/* Compute binomial n over k and return the result. */
ZZ ZZ_binomial(ZZ n, ZZ k){
	int i;
	ZZ out,n_,k_;

	if(n < k){
		out = ZZ::zero();
		return out;
	}

	if(n == 0 || k == 0){
		out = to_ZZ(1);
		return out;
	}

	k_ = k;
	n_ = n;

	if(k > n/2){
		k_ = n_ - k;
	}

	for(i = 1, out = 1; i <= k_; n_--, i++){
		out = out*n_/i;
	}

	return out;
}

/* Return the highest coefficient of polynomial. */
ZZ Linf_norm_ZZ_pE(ZZ_pE polynomial){
	ZZ result, modulus, coeff_x_;
	ZZ_p coeff_x;
	ZZ_pX x_;

	x_ = rep(polynomial);

	result = 0;
	modulus = (coeff(x_,0)).modulus();

	for (int x_deg = 0; x_deg < polynomial.degree(); ++x_deg) {
		coeff_x = coeff(x_,x_deg);
		coeff_x_ = rep(coeff_x);
		if(coeff_x_ > modulus/2){
			coeff_x_ = modulus - coeff_x_;
		}
		if(coeff_x_ > result){
			result = coeff_x_;
		}
	}

	return result;
}

// Compare two ZZ_pE-arrays.
int compare_ZZ_pE(ZZ_pE* polynomial_array1, ZZ_pE* polynomial_array2, int number_of_elements){
	ZZ_pX pol_ZZ_pX1, pol_ZZ_pX2;

	if(number_of_elements <= 0){
		return EXIT_FAILURE;
	}

	for (int index = 0; index < number_of_elements; ++index) {
		pol_ZZ_pX1 = rep(polynomial_array1[index]);
		pol_ZZ_pX2 = rep(polynomial_array2[index]);

		for (int coeff_power = 0; coeff_power < polynomial_array1[0].degree() ; ++coeff_power) {
			if(coeff(pol_ZZ_pX1, coeff_power) != coeff(pol_ZZ_pX2, coeff_power)){
				//cout<<"modulus:"<<coeff(pol_ZZ_pX1, coeff_power).modulus()<<"\n";
				//printf("coefficients are not the same!\n");
				//cout<<"element1 z["<<index<<"]["<<coeff_power<<"]="<<coeff(pol_ZZ_pX1, coeff_power)<<"\n";
				//cout<<"element2 z["<<index<<"]["<<coeff_power<<"]="<<coeff(pol_ZZ_pX2, coeff_power)<<"\n";
				return EXIT_FAILURE;
			}
		}
	}
	return EXIT_SUCCESS;
}

// Compare two ZZ_pE-arrays.
int compare_ZZ_pE(vector<ZZ_pE> polynomial_array1, vector<ZZ_pE> polynomial_array2){
	ZZ_pX pol_ZZ_pX1, pol_ZZ_pX2;

	if((polynomial_array1.size() != polynomial_array2.size())||(polynomial_array1.size() <= 0)){
		return EXIT_FAILURE;
	}

	for (int index = 0; index < polynomial_array1.size(); ++index) {
		pol_ZZ_pX1 = rep(polynomial_array1.at(index));
		pol_ZZ_pX2 = rep(polynomial_array2.at(index));

		for (int coeff_power = 0; coeff_power < polynomial_array1.at(index).degree() ; ++coeff_power) {
			if(coeff(pol_ZZ_pX1, coeff_power) != coeff(pol_ZZ_pX2, coeff_power)){
				//cout<<"modulus:"<<coeff(pol_ZZ_pX1, coeff_power).modulus()<<"\n";
				//printf("coefficients are not the same!\n");
				//cout<<"element1 z["<<index<<"]["<<coeff_power<<"]="<<coeff(pol_ZZ_pX1, coeff_power)<<"\n";
				//cout<<"element2 z["<<index<<"]["<<coeff_power<<"]="<<coeff(pol_ZZ_pX2, coeff_power)<<"\n";
				return EXIT_FAILURE;
			}
		}
	}
	return EXIT_SUCCESS;
}

/* Return the sum of absolute value of polynomial coefficients.
 * Polynomial coefficients are in range [-mod/2...mod/2]. */
ZZ L1_norm_ZZ_pE(ZZ_pE x){
	ZZ result, modulus, coeff_x_;
	ZZ_p coeff_x;
	ZZ_pX x_;

	x_ = rep(x);

	result = 0;
	modulus = (coeff(x_,0)).modulus();

	for (int x_deg = 0; x_deg < x.degree(); ++x_deg) {
		coeff_x = coeff(x_,x_deg);
		coeff_x_ = rep(coeff_x);
		if(coeff_x_ > modulus/2){
			coeff_x_ = modulus - coeff_x_;
		}
		result += coeff_x_;
	}
	return result;
}

/* Read in number_chars random bytes from /dev/random and initialize NTL-PRNG with it. */
int init_random(int number_chars){
	FILE *random_source;
	char random_char[number_chars];

	if((random_source = fopen("/dev/urandom", "r")) == NULL ){
		printf("Error: could not open /dev/urandom!\n");
		return EXIT_FAILURE;
	}

	if(!fgets(random_char,number_chars,random_source)){
		printf("Error: could not write random bytes into char array!");
		return EXIT_FAILURE;
	}
	fclose(random_source);

	SetSeed(ZZFromBytes((unsigned char *)random_char,number_chars));

	return EXIT_SUCCESS;
}

/* Return number of elements whose L1-norm is less or equal to max_norm_1. L1|x|<= max_norm_1 */
NTL::ZZ count_elements_up_to_L1(int element_length, int max_norm_1){
	ZZ count = ZZ::zero();

	for (int length = 0; length <= max_norm_1; ++length) {
		count += count_elements_with_L1(element_length,length);
	}
	return count;
}

/* Return number of elements whose L1-norm _exactly_ equal to norm_1. */
ZZ count_elements_with_L1(int element_length, int norm_1){
	ZZ count = ZZ::zero();
	if(element_length <= 0){
		return count;
	}

	if(norm_1 == 0){
		return to_ZZ(1);
	}

	for (int num_coeff = 1; num_coeff <= norm_1; ++num_coeff) {
		count += count_elements_with_L1_and_coeffs(element_length,norm_1,num_coeff);
	}
	return count;
}

ZZ count_elements_with_L1_and_coeffs(int element_length, int norm_1, int num_coeffs){
	return ZZ_binomial(to_ZZ(norm_1 -1),to_ZZ(num_coeffs-1))*ZZ_binomial(to_ZZ(element_length),to_ZZ(num_coeffs))*power(to_ZZ(2),num_coeffs);
}

/* Generate polynomial whose L1-norm is equal to "L1_value". */
ZZ_pE random_element_Dc(int polynomial_degree, ZZ L1_value, ZZ modulus){

	ZZ_pX rand_Dc;
	ZZ_pBak bak;
	ZZ_pE rand_Dc_ZZ_pE;
	long random_long,random_power;
	ZZ L1;
	ZZ_p coeff_old;

	ZZ_p::init(modulus);
	bak.save();								// store current modulus to bak

	rand_Dc = ZZ_pX();						// generate empty ZZ_pX
	rand_Dc_ZZ_pE = to_ZZ_pE(rand_Dc);		// transform it to ZZ_pE
	ZZ_p::init(to_ZZ(polynomial_degree));	// set modulus to n
	L1 = L1_norm_ZZ_pE(rand_Dc_ZZ_pE);

	while(L1 != L1_value) {
		random_power = random()%polynomial_degree;	// generate random coefficient in range(0...n-1)
		random_long = random();

		ZZ_p::init(modulus);						// set modulus to modulus
		coeff_old = coeff(rand_Dc, random_power);	// get old coefficient

		if(random_long > RAND_MAX/2){				// if random number > RAND_MAX/2 then multiply with -1
			SetCoeff(rand_Dc,random_power,coeff_old+(-1));
		}else{
			SetCoeff(rand_Dc,random_power,coeff_old + 1);
		}

		rand_Dc_ZZ_pE = to_ZZ_pE(rand_Dc);			// transform ZZ_pX to ZZ_pE
		L1 = L1_norm_ZZ_pE(rand_Dc_ZZ_pE);
	}
	bak.restore();									// restore modulus

	return rand_Dc_ZZ_pE;
}


int is_element_Gm(ZZ max_length, ZZ_pE element[], int array_length){
	for (int index = 0; index < array_length; ++index) {
		if(Linf_norm_ZZ_pE(element[index])>max_length){
			cout<<"element["<<index<<"] has length "<<Linf_norm_ZZ_pE(element[index])<<" which is greater as max_length "<<max_length<<".\tAbort!\n";
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int is_element_Gm(int PP_m, ZZ max_length, vector<ZZ_pE> array){

	if(array.size() != PP_m){
			return EXIT_FAILURE;
	}

	for (int index = 0; index < array.size(); ++index) {
		if(Linf_norm_ZZ_pE(array.at(index)) > max_length){
			cout<<"element["<<index<<"] has length "<<Linf_norm_ZZ_pE(array.at(index))<<" which is greater as max_length "<<max_length<<".\tAbort!\n";
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

unsigned char* convert_ZZ_pE_array_to_char_array(ZZ_pE *ZZ_pE_array, int number_of_ZZ_pE, unsigned long long* length_char){
	// get length of most length coeff in bits
	ZZ most_length_coeff;
	long bitlength, bytelength;
	ZZ_pX temp_ZZ_pX;
	ZZ_p koeff_x;
	bool sign;
	int degree = ZZ_pE_array[0].degree();

	for (int index = 0; index < number_of_ZZ_pE; ++index) {
		most_length_coeff = Linf_norm_ZZ_pE(ZZ_pE_array[index]);
	}
	bitlength = NumBits(most_length_coeff);

	if(bitlength%8){
		bytelength = (bitlength - bitlength%8)/8 +1;
	}else{
		bytelength = bitlength/8;
	}

	*length_char = bytelength*number_of_ZZ_pE*degree;
	unsigned char* char_array = (unsigned char *)malloc(*length_char*sizeof(unsigned char));

	for (int index_ZZ_pE = 0; index_ZZ_pE < number_of_ZZ_pE; ++index_ZZ_pE) {
		temp_ZZ_pX = rep(ZZ_pE_array[index_ZZ_pE]);
		for (int index_coefficient = 0; index_coefficient < degree; ++index_coefficient) {
			koeff_x = coeff(temp_ZZ_pX,index_coefficient);
			sign = false;
			if(rep(koeff_x)*2 > koeff_x.modulus()){
				koeff_x = to_ZZ_p((koeff_x.modulus()-rep(koeff_x)));
				sign = true;
			}
			BytesFromZZ(&char_array[(index_ZZ_pE*degree+index_coefficient)*bytelength],rep(koeff_x),bytelength);	// return koeff as char-array
			if(sign){
				char_array[(index_ZZ_pE*degree+index_coefficient+1)*bytelength-1] |=(1<<7);
			}
		}
	}
	return char_array;
}

unsigned char* convert_ZZ_pE_array_to_char_array(vector<ZZ_pE> ZZ_pE_array, unsigned long long* length_char){
	// get length of most length coeff in bits
	ZZ most_length_coeff;
	long bitlength, bytelength;
	ZZ_pX temp_ZZ_pX;
	ZZ_p koeff_x;
	bool sign;
	int degree = ZZ_pE_array.at(0).degree();

	for (int index = 0; index < ZZ_pE_array.size(); ++index) {
		most_length_coeff = Linf_norm_ZZ_pE(ZZ_pE_array.at(index));
	}
	bitlength = NumBits(most_length_coeff);

	if(bitlength%8){
		bytelength = (bitlength - bitlength%8)/8 +1;
	}else{
		bytelength = bitlength/8;
	}

	*length_char = bytelength*degree*(ZZ_pE_array.size());
	unsigned char* char_array = (unsigned char *)malloc(*length_char*sizeof(unsigned char));

	for (int index_ZZ_pE = 0; index_ZZ_pE < ZZ_pE_array.size(); ++index_ZZ_pE) {
		temp_ZZ_pX = rep(ZZ_pE_array.at(index_ZZ_pE));
		for (int index_coefficient = 0; index_coefficient < degree; ++index_coefficient) {
			koeff_x = coeff(temp_ZZ_pX,index_coefficient);
			sign = false;
			if(rep(koeff_x)*2 > koeff_x.modulus()){
				koeff_x = to_ZZ_p((koeff_x.modulus()-rep(koeff_x)));
				sign = true;
			}
			BytesFromZZ(&char_array[(index_ZZ_pE*degree+index_coefficient)*bytelength],rep(koeff_x),bytelength);	// return koeff as char-array
			if(sign){
				char_array[(index_ZZ_pE*degree+index_coefficient+1)*bytelength-1] |=(1<<7);
			}
		}
	}
	return char_array;
}

int convert_ZZ_pE_array_to_char_array(NTL::ZZ_pE *ZZ_pE_array, int number_of_ZZ_pE, unsigned long long* length_char, unsigned char* char_array){
	// get length of most length coeff in bits
	ZZ most_length_coeff;
	long bitlength, bytelength;
	ZZ_pX temp_ZZ_pX;
	ZZ_p koeff_x;
	bool sign;
	int degree = ZZ_pE_array[0].degree();

	for (int index = 0; index < number_of_ZZ_pE; ++index) {
		most_length_coeff = Linf_norm_ZZ_pE(ZZ_pE_array[index]);
	}
	bitlength = NumBits(most_length_coeff);

	if(bitlength%8){
		bytelength = (bitlength - bitlength%8)/8 +1;
	}else{
		bytelength = bitlength/8;
	}

	*length_char = bytelength*number_of_ZZ_pE*degree;

	for (int index_ZZ_pE = 0; index_ZZ_pE < number_of_ZZ_pE; ++index_ZZ_pE) {
		temp_ZZ_pX = rep(ZZ_pE_array[index_ZZ_pE]);
		for (int index_coefficient = 0; index_coefficient < degree; ++index_coefficient) {
			koeff_x = coeff(temp_ZZ_pX,index_coefficient);
			sign = false;
			if(rep(koeff_x)*2 > koeff_x.modulus()){
				koeff_x = to_ZZ_p((koeff_x.modulus()-rep(koeff_x)));
				sign = true;
			}
			BytesFromZZ(&char_array[(index_ZZ_pE*degree+index_coefficient)*bytelength],rep(koeff_x),bytelength);	// return koeff as char-array
			if(sign){
				char_array[(index_ZZ_pE*degree+index_coefficient+1)*bytelength-1] |=(1<<7);
			}
		}
	}

	return EXIT_SUCCESS;
}

/* Convert a char-array in a ZZ_pE-array. */
int convert_char_array_to_ZZ_pE_array(NTL::ZZ_pE *ZZ_pE_array,unsigned char* char_array,unsigned long long length_char, int number_of_ZZ_pE, int polynomial_degree){
	ZZ koeff_ZZ;
	ZZ_pX temp_ZZ_pX;

	if(length_char%(number_of_ZZ_pE*polynomial_degree)){
		printf("Char-array does not contain the right number of chars! Abort!\n");
		return EXIT_FAILURE;
	}
	int bytelength = length_char/(number_of_ZZ_pE*polynomial_degree);

	ZZ modulus = coeff(rep((const ZZ_pE)*ZZ_pE_array),0).modulus();
//	initialize_ZZ_pE(modulus,polynomial_degree);
	ZZ_p koeff_p;
	unsigned char temp_string[bytelength];

	for (int index_ZZ_pE_element = 0; index_ZZ_pE_element < number_of_ZZ_pE; ++index_ZZ_pE_element) {
		for (int coeff_power = 0; coeff_power < polynomial_degree; ++coeff_power) {
			if(char_array[(index_ZZ_pE_element*polynomial_degree+coeff_power+1)*bytelength-1]>>7){
				for (int index = 0; index < bytelength-1; ++index) {
					temp_string[index] = char_array[(index_ZZ_pE_element*polynomial_degree+coeff_power)*bytelength+index];
				}
				temp_string[bytelength-1] = char_array[(index_ZZ_pE_element*polynomial_degree+coeff_power+1)*bytelength-1]&0x7F;
				ZZFromBytes(koeff_ZZ, temp_string, bytelength);				// construct a ZZ-number from byte-array
				koeff_p = to_ZZ_p(koeff_ZZ*(-1));
				SetCoeff(temp_ZZ_pX,coeff_power,koeff_p);	// set this ZZ-number as coefficient in ZZ_pX-polynomial
			}else{
				ZZFromBytes(koeff_ZZ, &char_array[(index_ZZ_pE_element*polynomial_degree+coeff_power)*bytelength], bytelength);				// construct a ZZ-number from byte-array
				SetCoeff(temp_ZZ_pX,coeff_power,to_ZZ_p(koeff_ZZ));			// set this ZZ-number as coefficient in ZZ_pX-polynomial
			}
		}
		ZZ_pE_array[index_ZZ_pE_element] = to_ZZ_pE(temp_ZZ_pX);
	}
	return EXIT_SUCCESS;
}

/* Store a char-array in a file. */
//int save_char_array_in_file(unsigned char* char_array, long* length_char, char * filename);

/* Restore a char-array from file. */
//int restore_char_array_from_file(unsigned char* char_array, long* length_char, char * filename);

//int convert_char_array_in_element_Dc();

/* Return a random element of polynomial ring R, whose Linf-norm is equal to parameter length_Linf. */
int random_element_R_with_Linf(NTL::ZZ length_Linf, NTL::ZZ_pE* element){
	ZZ_pX temp;
	ZZ_pBak bak;
	bak.save();					// store current modulus of ZZ_pE to bak

	ZZ_p::init(length_Linf);	// set modulus to sigma+1

	temp = rep(random_ZZ_pE());	//

	bak.restore();				// set previous modulus back

	int random_int;

	for (unsigned int coeff_index = 0; coeff_index < element->degree()/(sizeof(int)*8); ++coeff_index) {
		random_int = random();
		for (unsigned char bit_index = 0; bit_index < 32; ++bit_index) {
			if((random_int>>bit_index)&1){
				SetCoeff(temp, coeff_index*32+bit_index, coeff(temp,coeff_index*32+bit_index)*(-1));
			}
		}
	}
	*element = to_ZZ_pE(temp);

	return EXIT_SUCCESS;
}

/* Return the number of polynomial coefficients which are not 0. */
int num_of_coeff_not_0(NTL::ZZ_pE polynomial){
	int result = 0;
	ZZ_pX temp_ZZ_pX = rep(polynomial);

	for (long int index_coeff = 0; index_coeff < polynomial.degree(); ++index_coeff) {
		if(coeff(temp_ZZ_pX,index_coeff) != ZZ_pE::zero())	result++;
	}
	return result;
}


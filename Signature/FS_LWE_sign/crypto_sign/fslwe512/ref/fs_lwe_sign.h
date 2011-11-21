/*
 * fs_lwe_sign.h
 *
 *  Created on: 10.10.2011
 *      Author: Wladimir Paulsen
 */

#ifndef FS_LWE_SIGN_H_
#define FS_LWE_SIGN_H_

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pX.h>
#include "sizes.h"

//NTL_CLIENT
struct SecretKey {
	NTL::ZZ_pE *SK;
};

struct PublicParameter{
	int n;				// Define the degree of polynomials
	int m;				// define the number of polynomials
	int sigma;			//
	int k;				//
};

struct Signatur{
	int length;
	NTL::ZZ_pE hash_RO_ZZ_pE;
	NTL::ZZ_pE* array_ZZ_pE;
};

struct Hash_Function{
	int length;
	NTL::ZZ_pE* array_ZZ_pE;
};

/* Generate secret key for FS_LWE-Schema.
 * sk€Ds^m && Linf(sk) <= sigma. */
int generate_sk(PublicParameter PP, NTL::ZZ_pE *secret_key);

/* Generate random hash function for FS_LWE-Schema. */
int generate_random_hash_function(PublicParameter PP, NTL::ZZ_pE *random_hash_function, int number_of_ZZ_pE);

/* Test input parameter and return a prime p with length (2*sigma +1)^m*(2^(-128/2)). */
NTL::ZZ test_input_parameter(PublicParameter PP);

/* Berechnet den Hash-Wert indem es message*(random hash function) berechnet */
NTL::ZZ_pE LWE_hash(PublicParameter PP, NTL::ZZ_pE *hash_function, NTL::ZZ_pE *message, int number_of_ZZ_pE);

/* Generate m random polynomials from Dy, which Linf-norm is <= n*m*k*sigma */
int random_element_Dy(PublicParameter PP, NTL::ZZ_pE* randomDy);

/* Take as input a random hash function and a message and put an element from Dc out.
 * This function should act as random oracle. */
NTL::ZZ_pE RandomOracle_Dc(PublicParameter PP, NTL::ZZ_pE LWE_hash_rand, unsigned char* message, unsigned long long message_length);

#endif /* FS_LWE_SIGN_H_ */

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
#include "aux.h"
#include "sizes.h"
#include <cstdlib>
#include <vector>

//NTL_CLIENT
struct PublicParameter{
	int n;				// Define the degree of polynomials
	int m;				// define the number of polynomials
	int sigma;			//
	int k;				//
};

/**
 *	Convert deterministically a char-array into an element of Dc.
 *	Split char array in sections with size of BLOCKSIZE, to get optimal use of chars.
 *	Then get random power and raise coefficient, up to settled number of coefficients.
 *	Finally fill up settled powers to L1_value.
 * @param num_coeff			Number of settled coefficients in generated polynomial.
 * @param L1_value			L1-norm of generated polynomial.
 * @param md_value			Array with chars from message digest.
 * @param charArraySize		Number of chars in array.
 * @return	Generated element of ring Dc.
 */
NTL::ZZ_pE generate_element_maxL1(NTL::ZZ num_coeff, NTL::ZZ L1_value, unsigned char* md_value, unsigned int charArraySize);

/**
 * Generate secret key for FS_LWE-Schema.
 * @param PP 			Public security parameter.
 * @param secret_key	Pointer to array of polynomials, where to store generated secret key.
 * @return 				0 if successful, else -1.
 */
int generate_sk(PublicParameter PP, NTL::ZZ_pE *secret_key);

/**
 *	Generate random hash function for FS_LWE-Schema.
 * @param random_hash_function	Pointer to array of polynomials, where to store generated random hash polynomials.
 * @param number_of_ZZ_pE		Number of polynomials to generate.
 * @return	0 if successful, else -1.
 */
int generate_random_hash_function(NTL::ZZ_pE *random_hash_function, int number_of_ZZ_pE);

/**
 * Test input parameter and return a prime p with length (2*sigma +1)^m*(2^(-128/2)).
 * @param PublicParam 	Public security parameter (n, m, sigma, k)
 * @return	A (probably) prime number with bitlength m*(ceil(log2(sigma))+1).
 */
NTL::ZZ test_input_parameter(PublicParameter PP);

/**
 * Compute LWE-Hash by multiplying message with hash polynomials and add to results.
 * @param PP				Public security parameter-
 * @param hash_function		Pointer to array of hash polynomials.
 * @param message			Pointer to array of polynomials to hash.
 * @param number_of_ZZ_pE	Number of polynomials in array.
 * @return					Hash-value as polynomial.
 */
NTL::ZZ_pE LWE_hash(PublicParameter PP, NTL::ZZ_pE *hash_function, NTL::ZZ_pE *message, int number_of_ZZ_pE);

/**
 * Generate (m) random polynomials from Dy, whose Linf-norm is <= n*m*k*sigma.
 * @param PP			Public security parameter.
 * @param randomDy		Pointer to array, where to store generated polynomials.
 * @return	0 if successful, else -1.
 */
int random_element_Dy(PublicParameter PP, NTL::ZZ_pE* randomDy);

/**
 *	Take as input a random hash function and a message and put an element from Dc out.
 *	This function should act as random oracle.
 * @param PP				Public security parameter.
 * @param LWE_hash_rand		LWE-hash value of random element.
 * @param message			Pointer to message array of unsigned char.
 * @param message_length	Length of message array.
 * @return					Random element of ring Dc.
 */
NTL::ZZ_pE RandomOracle_Dc(PublicParameter PP, NTL::ZZ_pE LWE_hash_rand, unsigned char* message, unsigned long long message_length);

/**
 * Verify signature of message, given the message, signature and public key.
 * @param PublicParameter		Public security parameter.
 * @param message				Pointer to message array of unsigned chars.
 * @param SignaturePart_z		Pointer to array of polynomials, part z of signature.
 * @param SignaturePart_e		Pointer to array of polynomials, part e of signature.
 * @param HashPolynomials		Pointer to array of polynomials of LWE-hash function, part of public key.
 * @param PublicKey				LWE-hash of secret key, public key.
 * @param length_Sign_z			Length of polynomial array.
 * @param message_length		Length of message array.
 * @return						0 if successful, -1 else.
 */
int verify_signatur(
		PublicParameter PublicParameter,
		unsigned char* 	message,
		NTL::ZZ_pE* 			SignaturePart_z,
		NTL::ZZ_pE* 			SignaturePart_e,
		NTL::ZZ_pE* 			HashPolynomials,
		NTL::ZZ_pE 			PublicKey,
		int 			length_Sign_z,
		unsigned long long message_length
	);
/**
 *	Generate a Fiat-Shamir signature of message with given secret key.
 * @param PublicParam		Public security parameter.
 * @param message			Pointer to message array of unsigned chars.
 * @param message_length	Length of message array.
 * @param SecretKey			Pointer to array of polynomials of secret key.
 * @param HashPolynomials	Pointer to array of polynomials of LWE hash function.
 * @param SignaturePart_e	Pointer to signature part e.
 * @param SignaturePart_z	Pointer to signature part z.
 * @return					0 if successful, else -1.
 */
int sign_message(
		PublicParameter PublicParam,
		unsigned char* 	message,
		unsigned long long message_length,
		NTL::ZZ_pE* 			SecretKey,
		NTL::ZZ_pE* 			HashPolynomials,
		NTL::ZZ_pE* 			SignaturePart_e,
		NTL::ZZ_pE* 			SignaturePart_z
);

/**
 * Generate a pair of secret and public key.
 * @param pk	Char array to store public key and polynomials of LWE hash function.
 * @param sk	Char array to store secret key and polynomials of LWE hash function.
 * @return	0 if successful, -1 else.
 */
int FSLWE25_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

/**
 * Generating a signed message sm[0],...,sm[*smlen-1] from original message m[0],...,m[mlen-1] under secret key sk[0],sk[1].
 *
 * @param sm	Char array to store signed message (message + signature).
 * @param smlen	Length of signed message.
 * @param m		Char array with message to sign.
 * @param mlen	Length of message.
 * @param sk	Pointer to secret key.
 * @return		0 if successful, -1 else.
 */
int FSLWE25_crypto_sign(
   unsigned char *sm,unsigned long long *smlen,
   const unsigned char *m,unsigned long long mlen,
   const unsigned char *sk
);

/**
 * verifying a signed message sm[0],...,sm[smlen-1] under public key pk and producing original message m[0],m[1],...,m[*mlen-1].
 * @param m		Char array with message.
 * @param mlen	Length of message.
 * @param sm	Char array with signed message.
 * @param smlen	Length of signed message.
 * @param pk	Pointer to public key.
 * @return		0 if successful, -1 else.
 */
int FSLWE25_crypto_sign_open(
  unsigned char *m,unsigned long long *mlen,
  const unsigned char *sm,unsigned long long smlen,
  const unsigned char *pk
);


// how many bits in higher byte needed to get a random degree, if degree of reduction polynomial > 256
const unsigned char  BITS_HIGH_BYTE = ((int)ceil(log2(PUBLIC_PARAMETER_N)))%8;
// what is optimal size for splitting char array, so that there is no need for bit shift over chars
const unsigned char BLOCKSIZE = (8 - 8%(BITS_HIGH_BYTE+1))/(BITS_HIGH_BYTE+1);
// how many bytes are needed to get a random number in range of Dc
const unsigned char const_BYTES_NUMBER = ceil(ceil(log2(NTL::to_double(elementsL1Max(PUBLIC_PARAMETER_N,PUBLIC_PARAMETER_K))))/8);
// how many bytes are needed to get a random number in range of Dc
const unsigned char const_BITS_NUMBER = (unsigned char)(ceil(log2(NTL::to_double(elementsL1Max(PUBLIC_PARAMETER_N,PUBLIC_PARAMETER_K))))) % 8;

#endif /* FS_LWE_SIGN_H_ */

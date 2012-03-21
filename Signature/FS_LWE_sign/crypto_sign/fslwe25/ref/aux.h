
#ifndef AUX_H_
#define AUX_H_

#include <NTL/ZZ.h>
#include <NTL/ZZ_pE.h>
#include <vector>

/** Setup a polynomial ring with (x^degree+1) as irreducible polynomial.
 * @param 	modulus_ZZ_pE 	Modulus of underlying integer field.
 * @param 	degree 			Degree of reduction polynomial.
 * @return 	Return 0 if successful else 1.
 * */
int initialize_ZZ_pE(NTL::ZZ modulus_ZZ_pE, int degree);

/** Compute binomial n over k and return the result.
 * @param  	n	NTL::ZZ Integer
 * @param	k	NTL::ZZ Integer
 * @return 	Return 0 if (n < k) or (n < 0) or (k < 0).
 * @return 	Return 1 if (n==0) or (k==0).
 * @return 	Else return (n over k) = n! / (k!*(n-k)!).
 * */
NTL::ZZ ZZ_binomial(NTL::ZZ n, NTL::ZZ k);

/**
 * Compare two ZZ_pE-arrays.
 *
 * @param polynomial_array1	First polynomial array to compare.
 * @param polynomial_array2	Second polynomial array to compare.
 * @param number_of_elements Number of polynomials in array.
 * @return Return 0 if arrays are the same, else return 1.
 */
int compare_ZZ_pE(NTL::ZZ_pE* polynomial_array1, NTL::ZZ_pE* polynomial_array2, int number_of_elements);

/**
 * Initialize NTL-Pseudo-random-number-generator with random bytes from /dev/random.
 * @param number_chars How many bytes from /dev/random will be used to init.
 * @return
 */
int init_random(int number_chars);

/**
 * Determine the highest coefficient of polynomial.
 * @param 	polynomial Polynomial to analyze.
 * @return the highest coefficient of polynomial.
 **/
NTL::ZZ LinfElement(NTL::ZZ_pX polynomial);

/**
 * Return the sum of absolute value of polynomial coefficients.
 * Polynomial coefficients are in range [-mod/2,...,mod/2].
 * @param polynomial Polynomial to analyze.
 * @return The sum of polynomial coefficients.
 */
NTL::ZZ L1Element(NTL::ZZ_pX polynomial);
/**
 * Return number of elements whose have L1-norm equal to norm_1.
 * L1|x| = norm_1
 * Assume that modulus of polynomial coefficients is greater as norm_1
 * @param polynomial_degree	Greatest degree of polynomials to analyze.
 * @param L1			L1-norm to determine.
 * @return Number of polynomials with L1-norm = norm_1
 */
NTL::ZZ elementsL1(int polynomial_degree, int L1);

/**
 * Return number of elements whose have L1-norm less or equal to norm_1.
 * L1|x|<= norm_1
 * @param polynomial_degree		Greatest degree of polynomials to analyze.
 * @param max_norm_1			L1-norm to determine.
 * @return	Number of polynomials with L1-norm <= norm_1
 */
NTL::ZZ elementsL1Max(int polynomial_degree, int max_norm_1);

/**
 * Determine if each element of polynomial array is an element with max. L1-norm <= max_length.
 * @param max_Linf 		Maximal Linf-norm.
 * @param element		Array of polynomials.
 * @param array_length	Number of polynomials in array.
 * @return	Return true if each element of array has max Linf <= max_Linf , else false.
 */
bool isElementOfRingWithLinfMax(NTL::ZZ LinfMax, NTL::ZZ_pE polynomialsArray[], int arrayLength);

/**
 *	Return the number of polynomial coefficients unequal 0.
 * @param polynomial 	Polynomial to analyze.
 * @return The number of polynomial coefficients unequal 0.
 */
int coeffsNot0(NTL::ZZ_pX polynomial);

/**
 * Return the number of polynomials with certain L1-norm and certain number of coefficients.
 * @param maxPolynomialDegree
 * @param L1norm
 * @param num_coeffs
 * @return
 */
NTL::ZZ elementsL1Coeffs(int polynomialDegree, int L1, int numberCoefficients);


/**
 * Generate a random polynomial whose L1-norm is equal to parameter L1.
 * @param polynomialDegree	Maximum Degree of polynomial
 * @param L1				L1-length to match.
 * @param modulus			Set modulus of integer ring.
 * @return random polynomial.
 */
NTL::ZZ_pE randomElementOfRingWithL1(int polynomialDegree, NTL::ZZ L1, NTL::ZZ modulus);

/**
 * Return a random element of polynomial ring R with certain Linf-norm.
 * @param length_Linf	Maximum Linf-length of sampled polynomials.
 * @return	random polynomial.
 */
NTL::ZZ_pE randomElementOfRingWithLinfMax(NTL::ZZ LinfMax);

/**
 * Convert a ZZ_pE-Array into an char-array.
 * @param polynomialArray			Polynomial array to convert.
 * @param lengthArray				Number of polynomials to convert.
 * @param lengthConvertedChars		Store here the number of converted chars.
 * @return	Array with converted chars.
 */
unsigned char* ZZpEArrayToCharArray(NTL::ZZ_pE *polynomialArray, int lengthPolynomialsArray, unsigned long long* lengthConvertedChars);

/**
 *
 * @param polynomialArray			Polynomial array to convert.
 * @param lengthPolynomialsArray	Number of polynomials to convert.
 * @param length_char				Store here the number of converted chars.
 * @param char_array				Pointer to array, where to store converted chars.
 * @return
 */
int convert_ZZ_pE_array_to_char_array(NTL::ZZ_pE *polynomialArray, int lengthPolynomialsArray, unsigned long long* lengthConvertedChars, unsigned char* convertedCharsArray);

/**
 * Convert polynomials stored in char-array back to ZZ_pE-array.
 *
 * @param polynomialArray			Pointer to array, where to store converted polynomials.
 * @param charsArray				Pointer to char array with converted polynomials.
 * @param lengthCharsArray			Length of char array.
 * @param numberOfPolynomials		How many polynomials are stored in char array.
 * @param polynomialDegree			Degree of polynomials, stored in char array.
 * @return	0 if successful, -1 else.
 */
int charArrayToZZpEArray(NTL::ZZ_pE *polynomialArray, unsigned char* charsArray, unsigned long long lengthCharsArray, int numberOfPolynomials, int polynomialDegree);

#endif /* AUX_H_ */

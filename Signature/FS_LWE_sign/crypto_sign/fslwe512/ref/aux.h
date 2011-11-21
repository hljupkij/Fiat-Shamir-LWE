
#ifndef AUX_H_
#define AUX_H_

#include <NTL/ZZ.h>
#include <NTL/ZZ_pE.h>

/* Setup a polynomial ring with (x^degree+1) as irreducible polynomial. */
int initialize_ZZ_pE(NTL::ZZ modulus_ZZ_pE, int degree);

/* Return the binomial coefficient (n over k). */
NTL::ZZ ZZ_binomial(NTL::ZZ n, NTL::ZZ k);

/* Compare two arrays with [number_of_elements] ZZ_pE-elements.
 * Return true if all elements are equal, else false.	*/
int compare_ZZ_pE(NTL::ZZ_pE* element_1, NTL::ZZ_pE* element_2, int number_of_elements);

/* Read in number_chars random bytes from /dev/random and initialize NTL-PRNG with it. */
int init_random(int number_chars);

/* Return the L-inf norm of polynomial.
 * (maximum value of polynomial coefficient) */
NTL::ZZ Linf_norm_ZZ_pE(NTL::ZZ_pE x);

/* Return the sum of absolute value of polynomial coefficients.
 * Polynomial coefficients are in range [-mod/2,...,mod/2]. */
NTL::ZZ L1_norm_ZZ_pE(NTL::ZZ_pE x);

/* Return number of elements whose have L1-norm equal to norm_1. L1|x| = norm_1 */
NTL::ZZ count_elements_with_L1(int element_length, int norm_1);

/* Return number of elements whose have L1-norm less or equal to norm_1. L1|x|<= norm_1 */
NTL::ZZ count_elements_up_to_L1(int element_length, int norm_1);

/* Return true if element is element of G^m. */
int is_element_Gm(NTL::ZZ max_length, NTL::ZZ_pE element[], int array_length);

/* Return the number of polynomial coefficients unequal 0. */
int num_of_coeff_not_0(NTL::ZZ_pE polynomial);

/* Return the number of polynomials with certain L1-norm and certain number of coefficients. */
NTL::ZZ count_elements_with_L1_and_coeffs(int element_length, int norm_1, int num_coeffs);

/* Generate a random polynomial whose L1-norm is equal to length. Random distribution. */
NTL::ZZ_pE random_element_Dc(int polynomial_degree, NTL::ZZ L1_value, NTL::ZZ modulus);

/* Convert a ZZ_pE-Array into an char-array.
 * Return a pointer to converted char-array. */
unsigned char* convert_ZZ_pE_array_to_char_array(NTL::ZZ_pE *ZZ_pE_array, int number_of_ZZ_pE, unsigned long long* length_char);

/* Convert a ZZ_pE-Array into an char-array.
 * Place converted char-array in memory pointed by parameter. */
int convert_ZZ_pE_array_to_char_array(NTL::ZZ_pE *ZZ_pE_array, int number_of_ZZ_pE, unsigned long long* length_char, unsigned char* char_array);

/* Convert a char-array in a ZZ_pE-array. */
int convert_char_array_to_ZZ_pE_array(NTL::ZZ_pE *ZZ_pE_array, unsigned char* char_array, unsigned long long length_char, int number_of_ZZ_pE, int polynomial_degree);

/* Store a char-array in a file. */
int save_char_array_in_file(unsigned char* char_array, long* length_char, char * filename);

/* Restore a char-array from file. */
int restore_char_array_from_file(unsigned char* char_array, long* length_char, char * filename);

/* Return a random element of polynomial ring R with certain Linf-norm. */
int random_element_R_with_Linf(NTL::ZZ length_Linf, NTL::ZZ_pE* element);

#endif /* AUX_H_ */

/*
 * fs_lwe_sign.cpp
 *
 *  Created on: 16.10.2011
 *      Author: Wladimir Paulsen
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pX.h>

#include "aux.h"
#include "api.h"
#include "fs_lwe_sign.h"
#include "crypto_sign.h"


NTL_CLIENT

 /* Generate secret key for FS_LWE-Schema.
  * sk€Ds^m && Linf(sk) <= sigma. */
int generate_sk(PublicParameter PP, ZZ_pE *secret_key){
	for(int i = 0; i < PP.m; i++){
		random_element_R_with_Linf(to_ZZ(PP.sigma+1),&secret_key[i]);
	}
	return EXIT_SUCCESS;
}

/* Generate secret key for FS_LWE-Schema.
 * sk€Ds^m && Linf(sk) <= sigma. */
vector<ZZ_pE> generate_sk(PublicParameter PP){
	vector<ZZ_pE> secret_key(PP.m);

	for(int index = 0; index < PP.m; index++){
		random_element_R_with_Linf(to_ZZ(PP.sigma+1),&secret_key[index]);
	}
	return secret_key;
}

/* Generate random hash function for FS_LWE-Schema. */
int generate_random_hash_function(PublicParameter PP, ZZ_pE *random_hash_function, int number_of_ZZ_pE){

	for(int i = 0; i < number_of_ZZ_pE; i++){
		random_hash_function[i] = random_ZZ_pE();
	}
	return EXIT_SUCCESS;
}

/* Generate random hash function for FS_LWE-Schema. */
vector<ZZ_pE> generate_random_hash_function(PublicParameter PP){
	vector<ZZ_pE> random_hash_function(PP.m);

	for(int index = 0; index < random_hash_function.size(); index++){
		random_hash_function.at(index) = random_ZZ_pE();
	}
	return random_hash_function;
}


// Convert deterministically a 512-Bit String/char-array into an element of Dc.
ZZ_pE generate_element_Dc(int byte_offset, ZZ num_coeff, ZZ L1_value, vector<char> md_value){

	int power_, bit_length;
	bool high_byte, element_in_vector;
	unsigned char koef=0,index_koeff=0, index_number=0, random_power;
	ZZ_pX temp_res = ZZ_pX();
	ZZ_pX result = ZZ_pX();
	std::vector<int> setted_coefficients;

	while((L1_norm_ZZ_pE(to_ZZ_pE(result)) != L1_value)&&((byte_offset+5*index_koeff+index_number) < md_value.size())){
		while((num_of_coeff_not_0(to_ZZ_pE(result)) < num_coeff)&&((byte_offset+5*index_koeff+index_number) < md_value.size())){
			if(index_number%4 == 0){
				koef = md_value[index_koeff*5+byte_offset];
				index_koeff++;
				index_number = 0;
			}
			high_byte = (koef>>(2*index_number))&0x1;
			power_ = md_value[index_koeff*5+byte_offset+index_number]+high_byte*(1<<8);

			element_in_vector = false;
			for (unsigned int vector_index = 0; vector_index < setted_coefficients.size(); ++vector_index) {
				if(setted_coefficients.at(vector_index) == power_){
					element_in_vector = true;
				}
			}
			if(!element_in_vector){	// power_ isn't in set
				setted_coefficients.push_back(power_);
				if(koef>>(2*index_number+1)){
					SetCoeff(temp_res,power_, 1);
				}else{
					SetCoeff(temp_res,power_,(-1));
				}
			}else{
			}
			result = ZZ_pX(temp_res);
			index_number++;
		}

		if((L1_value > num_coeff)&&(L1_norm_ZZ_pE(to_ZZ_pE(temp_res)) < L1_value)){
			// raise one coefficient
			bit_length = (int)log2(setted_coefficients.size())+1;
			random_power = md_value[index_koeff*5+index_number+byte_offset]&((1<<bit_length) - 1);

			if(random_power < setted_coefficients.size()){
				ZZ_p coeff_old = coeff(temp_res,setted_coefficients.at(random_power));

				if(2*rep(coeff_old) > coeff_old.modulus()){		// get sign of old coefficient
					SetCoeff(temp_res,setted_coefficients.at(random_power),coeff_old-1);
				}else{
					SetCoeff(temp_res,setted_coefficients.at(random_power),coeff_old+1);
				}
			}
		}

		result = ZZ_pX(temp_res);
		index_number++;
	}

	return to_ZZ_pE(result);
}

/* Test input parameter and return a prime p with length (2*sigma +1)^m*(2^(-128/2)). */
ZZ test_input_parameter(PublicParameter PP){
	ZZ binomial, p, tmp;

	long pow;

	if(PP.n <= 0 || PP.m <= 0 || PP.sigma <= 0 || PP.m <= 0){
		printf("Some of input parameters is smaller as 0!\n");
		return to_ZZ(0);
	}

	// test if n is power of 2
	if((PP.n&(PP.n-1))!= 0){
		printf("Error, n isn't power of 2!");
		return ZZ::zero();
	}

	binomial =  ZZ_binomial(to_ZZ(PP.n),to_ZZ(PP.k));

	/* take care that (k + log2((n over k))) >= 160 Bit */
	if((PP.k +(log(binomial)/log(2)))< 160){
		printf("Error: Value k is to small!\n");
		return to_ZZ(0);
	}

	tmp = 2*power(to_ZZ(2*PP.sigma+1), PP.m);
	pow = log(tmp)/log(2);
	p = GenPrime_ZZ(pow,80);

	return p;
}

/* compute LWE-Hash through multiplying message with hash polynomials and add results. */
ZZ_pE LWE_hash(PublicParameter PP, ZZ_pE *hash_function, ZZ_pE *message, int number_of_ZZ_pE){
	ZZ_pE result = ZZ_pE::zero();

    // multiply message blocks with hash polynomials and add to result
    for (int i = 0; i < number_of_ZZ_pE; ++i) {
    	result += message[i]*hash_function[i];
	}
    return result;
}

/* compute LWE-Hash through multiplying message with hash polynomials and add results. */
ZZ_pE LWE_hash(vector<ZZ_pE> hash_function, vector<ZZ_pE> message){
	ZZ_pE result = ZZ_pE::zero();

    // verify that message and hash matrix has same length
    if(hash_function.size() != message.size()){
        return result;
    }

    // multiply message blocks with hash polynomials and add to result
    for (int index = 0; index < message.size(); ++index) {
    	result += message.at(index)*hash_function.at(index);
	}
    return result;
}

/* Generate m random polynomials from Dy, whose Linf-norm is <= n*m*k*sigma */
int random_element_Dy(PublicParameter PP, ZZ_pE* randomDy){

	for(int i = 0; i < PP.m; i++){
		random_element_R_with_Linf(to_ZZ(PP.k*PP.m*PP.n*PP.sigma), &randomDy[i]);
	}
	return EXIT_SUCCESS;
}

/* Take as input a random hash function and a message and put an element from Dc out.
 * This function should act as random oracle. */
ZZ_pE RandomOracle_Dc(PublicParameter PP, ZZ_pE LWE_hash_rand, unsigned char* message, unsigned long long message_length){

	unsigned char md_value[EVP_MAX_MD_SIZE];	// message digest
	unsigned char* char_arr;					// char-array for converted lwe-hash-value
	unsigned long long length_char_arr;

	// convert LWE-Hash into char-array for hash-function
	char_arr =  convert_ZZ_pE_array_to_char_array(&LWE_hash_rand, 1, &length_char_arr);

	// copy message into a new array
	unsigned char* message_with_lwe = (unsigned char*)malloc(sizeof(char)*(message_length + length_char_arr));
	if(NULL == message_with_lwe){
		printf("Couldn't reserve memory, abort!\n");
		return ZZ_pE::zero();
	}

	strncpy((char *)message_with_lwe,(const char*)message,message_length);
	for (unsigned int index = 0; index < length_char_arr; ++index) {
		message_with_lwe[message_length+index] = char_arr[index];
	}

	SHA512(message_with_lwe, message_length+length_char_arr, md_value);

	md_value[BYTES_NUMBER-1] &= 1;		// remove first 7 bits to get a number in range 0...(2^161)-1

	ZZ number = ZZFromBytes(md_value, BYTES_NUMBER);	// 8*21=168-Bit with
	int L1_norm=0, num_coeff=0;

	for(int i = PP.k; i>= 1;i--){
		if((count_elements_up_to_L1(PP.n,i-1) < number) && (count_elements_up_to_L1(PP.n,i) > number)){
			L1_norm = i;
		}
	}

	ZZ temp = number - count_elements_up_to_L1(PP.n, L1_norm-1);
	ZZ sum_elements = count_elements_with_L1(PP.n, L1_norm);

	for (int index_num_coeff = L1_norm; index_num_coeff >= 1; index_num_coeff--){
		if((sum_elements - count_elements_with_L1_and_coeffs(PP.n, L1_norm, index_num_coeff) <= temp) && (sum_elements > temp)){
			num_coeff = index_num_coeff;
		}
		sum_elements -= count_elements_with_L1_and_coeffs(PP.n, L1_norm, index_num_coeff);
	}

	vector<char> message_digest(EVP_MAX_MD_SIZE);
	for(int i =0; i< sizeof(md_value)/sizeof(char);i++){
		message_digest.push_back(md_value[i]);
	}

	// Convert 512-Bit String (md_value) into an element of Dc
	ZZ_pE result =  generate_element_Dc(BYTES_NUMBER, to_ZZ(num_coeff), to_ZZ(L1_norm), message_digest);

	// delete temporary arrays and return a result
	free(char_arr);
	free(message_with_lwe);

	return result;
}


int verify_signatur(PublicParameter PP,unsigned char* message, ZZ_pE *Sign_z, ZZ_pE *Sign_e, ZZ_pE* random_hash_function, ZZ_pE PublicKey, int length_Sign_z, unsigned long long message_length){
//	printf("Start verification!\n");

	// is sign_z element of G^m ?
	ZZ max_length = to_ZZ(PP.sigma*PP.k*(PP.m*PP.n-1));
	if(is_element_Gm(max_length, Sign_z, length_Sign_z)){
//		printf("Verification failed, response is not an element of G^m! Abort!\n");
		return -1;
	}

	ZZ_pE hash_Sign_z = LWE_hash(PP,random_hash_function, Sign_z, length_Sign_z);
	ZZ_pE temp = hash_Sign_z - (*Sign_e)*PublicKey;
	ZZ_pE temp_e = RandomOracle_Dc(PP,temp, message, message_length);

	if(!compare_ZZ_pE(Sign_e,&temp_e,1)){
		return EXIT_SUCCESS;
	}
	return -1;
}

/* Verification function for FS-LWE-ID-Schema. */
int verify_response(PublicParameter PP, ZZ_pE* response, int length_response, ZZ_pE* hash_function, ZZ_pE challenge, ZZ_pE commitment, ZZ_pE PublicKey){
//	printf("Start verification!\n");
	ZZ max_length = to_ZZ(PP.m*PP.n*(PP.sigma*PP.k -1));
	if(is_element_Gm(max_length,response,length_response)){
//		printf("Verification failed, response is not an element of G^m! Abort!\n");
		return -1;
	}

	ZZ_pE hash_y = LWE_hash(PP,hash_function,response,length_response);
	if(hash_y != PublicKey*challenge+commitment){
//		printf("Verification failed, wrong response! Abort!\n");
		return -1;
	}
	return EXIT_SUCCESS;
}

int sign_message(PublicParameter PP,unsigned char* message, unsigned long long message_length, ZZ_pE* SecretKey, ZZ_pE* hash_function, ZZ_pE *e, ZZ_pE *z){
//	printf("Start signing message!\tMessage length:%lli\n",message_length);
	ZZ_pE rand_Dy_hash;
	ZZ_pE random_Dy[PP.m];
	bool element_ok = false;
	int count = 1;

	// up to 30 tries
	while(!element_ok && count < 30){
		element_ok = true;
		random_element_Dy(PP,random_Dy);
		rand_Dy_hash = LWE_hash(PP,hash_function,random_Dy,PP.m);

		e[0] = RandomOracle_Dc(PP,rand_Dy_hash,message, message_length);

		for (int index = 0; index < PP.m; ++index) {
			z[index] = SecretKey[index]*e[0] + random_Dy[index];

			if(Linf_norm_ZZ_pE(z[index])>(PP.sigma*PP.k*(PP.n*PP.m-1))){
				element_ok = false;
				break;
			}

		}
		count++;
	}
	return EXIT_SUCCESS;
}

/* Generate a keypair of secret and public key. */
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk){

	 bool result;
	 struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};

	 unsigned long long length;

	NTL::ZZ prime_number = test_input_parameter(PP);
	initialize_ZZ_pE(prime_number, PP.n);						// initialize ZZ_pE and ZZ_p
	NTL::BytesFromZZ(pk, prime_number, BYTES_PRIMENUMBER);		// convert p to char
	NTL::BytesFromZZ(sk, prime_number, BYTES_PRIMENUMBER);		// convert p to char

	NTL::ZZ_pE sk_ZZ[PP.m];
	NTL::ZZ_pE rhf[PP.m];

	result = generate_sk(PP,sk_ZZ);								// generate random secret key

	generate_random_hash_function(PP, rhf, PP.m);				// generate random hash-function

	ZZ_pE pk_ZZ_pE = LWE_hash(PP, rhf, sk_ZZ, PP.m);			// compute public key

	convert_ZZ_pE_array_to_char_array(rhf, PP.m, &length, &pk[BYTES_PRIMENUMBER]);
	convert_ZZ_pE_array_to_char_array(rhf, PP.m, &length, &sk[BYTES_PRIMENUMBER]);

	convert_ZZ_pE_array_to_char_array(&pk_ZZ_pE, 1, &length, &pk[BYTES_PRIMENUMBER+BYTES_HASHPOLYNOMIAL]);
	convert_ZZ_pE_array_to_char_array(sk_ZZ, PP.m, &length, &sk[BYTES_PRIMENUMBER+BYTES_HASHPOLYNOMIAL]);

	 return 0;
 }
/* generating a signed message sm[0],...,sm[*smlen-1] from original message m[0],...,m[mlen-1] under secret key sk[0],sk[1]. */
int crypto_sign(unsigned char *sm,unsigned long long *smlen, const unsigned char *m,unsigned long long mlen, const unsigned char *sk){

	struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};
	unsigned long long length;
	NTL::ZZ prime_number;
	prime_number = NTL::ZZFromBytes(sk, BYTES_PRIMENUMBER);
	initialize_ZZ_pE(prime_number, PUBLIC_PARAMETER_N);				// initialize ZZ_pE and ZZ_p

	NTL::ZZ_pE hash_polynomial[PUBLIC_PARAMETER_M];
	NTL::ZZ_pE sk_ZZ_pE[PUBLIC_PARAMETER_M];
	convert_char_array_to_ZZ_pE_array(hash_polynomial, (unsigned char*)&sk[BYTES_PRIMENUMBER], BYTES_HASHPOLYNOMIAL, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(sk_ZZ_pE, (unsigned char*)&sk[BYTES_PRIMENUMBER+BYTES_HASHPOLYNOMIAL],BYTES_SECRETKEY,PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_N);

	NTL::ZZ_pE sign_e;
	NTL::ZZ_pE sign_z[PUBLIC_PARAMETER_M];
	sign_message(PP, (unsigned char*) m, mlen, sk_ZZ_pE, hash_polynomial, &sign_e, sign_z);

	/* copy message and signature */
	*smlen = mlen + BYTES_SIGNATUR_E + BYTES_SIGNATUR_Z;
	strncpy((char *)sm, (const char*)m, mlen);

	convert_ZZ_pE_array_to_char_array(sign_z,PP.m, &length, sm+mlen);
	convert_ZZ_pE_array_to_char_array(&sign_e, 1, &length, sm+mlen+BYTES_SIGNATUR_Z);

	return 0;
}
/* verifying a signed message sm[0],...,sm[smlen-1] under public key pk and producing original message m[0],m[1],...,m[*mlen-1]. */
 int crypto_sign_open(unsigned char *m,unsigned long long *mlen, const unsigned char *sm,unsigned long long smlen, const unsigned char *pk){

	*mlen = smlen - BYTES_SIGNATUR_E - BYTES_SIGNATUR_Z;
	strncpy((char *)m,(const char*)sm,*mlen);	// copy chars from signed message
	struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};

	NTL::ZZ prime_number = NTL::ZZFromBytes(pk, BYTES_PRIMENUMBER);
	initialize_ZZ_pE(prime_number, PUBLIC_PARAMETER_N);				// initialize ZZ_pE and ZZ_p

	NTL::ZZ_pE hash_polynomial[PUBLIC_PARAMETER_M];
	NTL::ZZ_pE sign_z[PUBLIC_PARAMETER_M];
	NTL::ZZ_pE pk_ZZ_pE, sign_e;


	convert_char_array_to_ZZ_pE_array(hash_polynomial,(unsigned char*)&pk[BYTES_PRIMENUMBER], BYTES_HASHPOLYNOMIAL,PUBLIC_PARAMETER_M, PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(&pk_ZZ_pE,(unsigned char*)&pk[BYTES_PRIMENUMBER+BYTES_HASHPOLYNOMIAL],BYTES_PUBLICKEY,1,PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(sign_z,(unsigned char *) (sm+*mlen),BYTES_SIGNATUR_Z, PUBLIC_PARAMETER_M, PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(&sign_e,(unsigned char *) (sm+*mlen+BYTES_SIGNATUR_Z), BYTES_SIGNATUR_E, 1, PUBLIC_PARAMETER_N);

	return verify_signatur(PP, m, sign_z, &sign_e, hash_polynomial, pk_ZZ_pE, PUBLIC_PARAMETER_M, *mlen);

	 return 0;
 }

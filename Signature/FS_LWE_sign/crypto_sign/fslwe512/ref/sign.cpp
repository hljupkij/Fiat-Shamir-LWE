/*
 * sign.c
 *
 *  Created on: 11.10.2011
 *      Author: Wladimir Paulsen
 */

#include "crypto_sign.h"
#include "fs_lwe_sign.h"
#include "api.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pX.h>

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk){
/*       ... the code for your Square2048 implementation goes here,
	 ... generating public key pk[0],pk[1],...
	 ... and secret key sk[0],sk[1],...
	 */
	 bool result;
	 struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};

	 unsigned long long length;

	NTL::ZZ prime_number = test_input_parameter(PP);
	initialize_ZZ_pE(prime_number, PP.n);						// initialize ZZ_pE and ZZ_p
	NTL::BytesFromZZ(pk, prime_number, BYTES_PRIMENUMBER);		// convert p to char
	NTL::BytesFromZZ(sk, prime_number, BYTES_PRIMENUMBER);		// convert p to char

	NTL::ZZ_pE sk_ZZ[PP.m];
	NTL::ZZ_pE rhf[PP.m];

	// TODO: generation of sk und hash-polynomial are separate -> threads?

	result = generate_sk(PP,sk_ZZ);			// generate secret key

	generate_random_hash_function(PP, rhf, PP.m);	// generate polynomial for hash-function

	NTL::ZZ_pE pk_ZZ_pE = LWE_hash(PP, rhf, sk_ZZ, PP.m);

	convert_ZZ_pE_array_to_char_array(rhf, PP.m, &length, &pk[BYTES_PRIMENUMBER]);
	convert_ZZ_pE_array_to_char_array(rhf, PP.m, &length, &sk[BYTES_PRIMENUMBER]);

	convert_ZZ_pE_array_to_char_array(&pk_ZZ_pE, 1, &length, &pk[BYTES_PRIMENUMBER+BYTES_HASHFUNCTION]);
	convert_ZZ_pE_array_to_char_array(sk_ZZ, PP.m, &length, &sk[BYTES_PRIMENUMBER+BYTES_HASHFUNCTION]);

	 return 0;
 }

int crypto_sign(unsigned char *sm,unsigned long long *smlen, const unsigned char *m,unsigned long long mlen, const unsigned char *sk){
 /*		 ... the code for your Square2048 implementation goes here,
         ... generating a signed message sm[0],sm[1],...,sm[*smlen-1]
         ... from original message m[0],m[1],...,m[mlen-1]
         ... under secret key sk[0],sk[1],...
*/
	struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};
	unsigned long long length;
	NTL::ZZ prime_number;
	prime_number = NTL::ZZFromBytes(sk, BYTES_PRIMENUMBER);
	initialize_ZZ_pE(prime_number, PUBLIC_PARAMETER_N);				// initialize ZZ_pE and ZZ_p

	NTL::ZZ_pE hash_polynomial[PUBLIC_PARAMETER_M];
	NTL::ZZ_pE sk_ZZ_pE[PUBLIC_PARAMETER_M];
	convert_char_array_to_ZZ_pE_array(hash_polynomial, (unsigned char*)&sk[BYTES_PRIMENUMBER], BYTES_HASHFUNCTION, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(sk_ZZ_pE, (unsigned char*)&sk[BYTES_PRIMENUMBER+BYTES_HASHFUNCTION],BYTES_SECRETKEY,PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_N);

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

 int crypto_sign_open(unsigned char *m,unsigned long long *mlen, const unsigned char *sm,unsigned long long smlen, const unsigned char *pk){
   /*
	 ... the code for your Square2048 implementation goes here,
	 ... verifying a signed message sm[0],sm[1],...,sm[smlen-1]
	 ... under public key pk[0],pk[1],...
	 ... and producing original message m[0],m[1],...,m[*mlen-1]
   */

	*mlen = smlen - BYTES_SIGNATUR_E - BYTES_SIGNATUR_Z;
	strncpy((char *)m,(const char*)sm,*mlen);	// copy chars from signed message
	struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};

	NTL::ZZ prime_number = NTL::ZZFromBytes(pk, BYTES_PRIMENUMBER);
	initialize_ZZ_pE(prime_number, PUBLIC_PARAMETER_N);				// initialize ZZ_pE and ZZ_p

	NTL::ZZ_pE hash_polynomial[PUBLIC_PARAMETER_M];
	NTL::ZZ_pE sign_z[PUBLIC_PARAMETER_M];
	NTL::ZZ_pE pk_ZZ_pE, sign_e;


	convert_char_array_to_ZZ_pE_array(hash_polynomial,(unsigned char*)&pk[BYTES_PRIMENUMBER], BYTES_HASHFUNCTION,PUBLIC_PARAMETER_M, PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(&pk_ZZ_pE,(unsigned char*)&pk[BYTES_PRIMENUMBER+BYTES_HASHFUNCTION],BYTES_PUBLICKEY,1,PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(sign_z,(unsigned char *) (sm+*mlen),BYTES_SIGNATUR_Z, PUBLIC_PARAMETER_M, PUBLIC_PARAMETER_N);
	convert_char_array_to_ZZ_pE_array(&sign_e,(unsigned char *) (sm+*mlen+BYTES_SIGNATUR_Z), BYTES_SIGNATUR_E, 1, PUBLIC_PARAMETER_N);

	return verify_signatur(PP, m, sign_z, &sign_e, hash_polynomial, pk_ZZ_pE, PUBLIC_PARAMETER_M, *mlen);

	 return 0;
 }

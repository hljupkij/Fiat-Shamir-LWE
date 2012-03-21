/*
 * TestSuite.cpp
 *
 * Test-Suite for testing all module functions.
 *
 *  Created on: 10.11.2011
 *      Author: Wladimir Paulsen
 */

#include <cpptest.h>
#include "fs_lwe_sign.h"
#include "aux.h"
#include <cstdlib>
#include "api.h"
#include "openssl/sha.h"
#include <cstring>

class TestSuite : public Test::Suite
{
	public:
		TestSuite()
		{
			TEST_ADD(TestSuite::test_ZZ_pE)
			TEST_ADD(TestSuite::test_ZZ_binomial)
			TEST_ADD(TestSuite::test_initialize_ZZ_pE)
			TEST_ADD(TestSuite::test_compare_ZZ_pE)
			TEST_ADD(TestSuite::test_count_elements_up_to_L1)
			TEST_ADD(TestSuite::test_Linf_norm_ZZ_pE)
			TEST_ADD(TestSuite::test_L1_norm_ZZ_pE)
			TEST_ADD(TestSuite::test_L1_norm_ZZ_pX)
			TEST_ADD(TestSuite::test_count_elements_with_L1)
			TEST_ADD(TestSuite::test_count_elements_with_L1_and_coeffs)
			TEST_ADD(TestSuite::test_is_element_Gm)
			TEST_ADD(TestSuite::test_num_of_coeff_not_0)
			TEST_ADD(TestSuite::test_random_element_R_with_Linf)
			TEST_ADD(TestSuite::test_ZZ_pE_convert)
			TEST_ADD(TestSuite::test_crypto_sign_keypair)
			TEST_ADD(TestSuite::test_LWE_hash)
			TEST_ADD(TestSuite::test_SSL_SHA512)
			TEST_ADD(TestSuite::test_Sign_Verify)
		}

	private:
		void test_ZZ_pE(){
			initialize_ZZ_pE(NTL::to_ZZ(7),7);
			NTL::ZZ_pX t = NTL::ZZ_pX(7,2);
			NTL::ZZ_pE test = NTL::to_ZZ_pE(t);

			std::cout<<test<<"\n";

			TEST_ASSERT(test.degree() == 7);
		}


		/* test ZZ_binomial*/
		void test_ZZ_binomial(){
			// if ok = 0
			TEST_ASSERT(ZZ_binomial(NTL::to_ZZ(2), NTL::ZZ::zero()) == NTL::to_ZZ(1))
			// if n < k
			TEST_ASSERT(ZZ_binomial(NTL::to_ZZ(1), NTL::to_ZZ(2)) 	== NTL::ZZ::zero())
			// normal operation
			TEST_ASSERT(ZZ_binomial(NTL::to_ZZ(6), NTL::to_ZZ(3)) 	== NTL::to_ZZ(20))
			// test if k < 0
			TEST_ASSERT(ZZ_binomial(NTL::to_ZZ(3), NTL::to_ZZ(-2))	== NTL::ZZ::zero())
		}

		void test_initialize_ZZ_pE(){
			NTL::ZZ modul = NTL::to_ZZ(3);

			// try to set modulus to 0
			TEST_ASSERT(initialize_ZZ_pE(NTL::ZZ::zero(),2) == 	EXIT_FAILURE)
			// try to set modulus to -2
			TEST_ASSERT(initialize_ZZ_pE(NTL::to_ZZ(-2),2) == 	EXIT_FAILURE)
			// try to set modulus to 1
			TEST_ASSERT(initialize_ZZ_pE(NTL::to_ZZ(1),2) 	== 	EXIT_FAILURE)
			// try to set the degree to 0
			TEST_ASSERT(initialize_ZZ_pE(modul,0) 			== 	EXIT_FAILURE)
			// try to set the degree to -2
			TEST_ASSERT(initialize_ZZ_pE(modul,-2) 			== 	EXIT_FAILURE)
			// try to set x³-1
			TEST_ASSERT(initialize_ZZ_pE(modul,3)			==	EXIT_SUCCESS)
		}

		void test_compare_ZZ_pE(){

			initialize_ZZ_pE(NTL::to_ZZ(13),5);
			NTL::ZZ_pE const1 = NTL::random_ZZ_pE();

			NTL::ZZ_pE test1_ZZ_pE[3];
				test1_ZZ_pE[0] = const1;
				test1_ZZ_pE[1] = const1;
				test1_ZZ_pE[2] = NTL::ZZ_pE::zero();

			NTL::ZZ_pE test2_ZZ_pE[4];
				test2_ZZ_pE[0] = const1;
				test2_ZZ_pE[1] = const1;
				test2_ZZ_pE[2] = const1;
				test2_ZZ_pE[3] = const1;

			// test if length is not equal if test1 > test2
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 5) == -1)
			// test if length is not equal if test1 < test2
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test2_ZZ_pE, 4) == -1)
			// test if length = 0
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 0) == -1)
			// test if length > len(test1) or len(test2)
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test2_ZZ_pE, 5) == -1)
			// test for correct
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 3) == 0)
		}

		void test_count_elements_up_to_L1(){
			TEST_ASSERT(elementsL1Max(0,2) == NTL::ZZ::zero())
			TEST_ASSERT(elementsL1Max(3,0) == NTL::to_ZZ(1))
			TEST_ASSERT(elementsL1Max(3,3) == NTL::to_ZZ(63))
		}

		void test_count_elements_with_L1(){

			TEST_ASSERT(elementsL1(0,0) == 0)
			TEST_ASSERT(elementsL1(1,0) == 1)
			TEST_ASSERT(elementsL1(0,1) == 0)
			TEST_ASSERT(elementsL1(1,1) == 2)
		}

		void test_count_elements_with_L1_and_coeffs(){

			TEST_ASSERT(elementsL1Coeffs(2,0,0)	==	NTL::to_ZZ(1))
			TEST_ASSERT(elementsL1Coeffs(0,1,1)	==  NTL::ZZ::zero())
			TEST_ASSERT(elementsL1Coeffs(0,0,1)	==	NTL::ZZ::zero())

			TEST_ASSERT(elementsL1Coeffs(0,0,0)	==	NTL::ZZ::zero())
			TEST_ASSERT(elementsL1Coeffs(2,1,0)	==	NTL::ZZ::zero())
			// if coeff > L1
			TEST_ASSERT(elementsL1Coeffs(2,1,2)	==	NTL::ZZ::zero())

			TEST_ASSERT(elementsL1Coeffs(-2,1,2)	==	NTL::ZZ::zero())
			TEST_ASSERT(elementsL1Coeffs(2,-1,2)	==	NTL::ZZ::zero())
			TEST_ASSERT(elementsL1Coeffs(2,2,-1)	==	NTL::ZZ::zero())
		}

		void test_Linf_norm_ZZ_pE(){
			struct PublicParameter PP = {512, 4, 127, 24};
			NTL::ZZ p = test_input_parameter(PP);
			initialize_ZZ_pE(p,512);

			NTL::ZZ_pX two = NTL::to_ZZ_pX((p+1)/2);
			NTL::ZZ_pX three = NTL::to_ZZ_pX((p-1)/2);
			NTL::ZZ_pX one = NTL::to_ZZ_pX(-1);

			TEST_ASSERT(LinfElement(two) 				== 	NTL::to_ZZ((p-1)/2))
			TEST_ASSERT(LinfElement(three) 				== 	NTL::to_ZZ((p-1)/2))
			TEST_ASSERT(LinfElement(one) 				== 	NTL::to_ZZ(1))
			TEST_ASSERT(LinfElement(NTL::ZZ_pX::zero())	==	NTL::ZZ::zero())
		}

		void test_L1_norm_ZZ_pX(){
			NTL::ZZ_pX one = NTL::to_ZZ_pX(-1);

			TEST_ASSERT(L1Element(one) 						== NTL::to_ZZ(1))
			TEST_ASSERT(L1Element(NTL::ZZ_pX::zero())		== NTL::ZZ::zero())
		}

		void test_is_element_Gm(){
			initialize_ZZ_pE(NTL::to_ZZ(13),5);
			NTL::ZZ_pE const1 = NTL::random_ZZ_pE();

			NTL::ZZ_pE test1_ZZ_pE[3];
				test1_ZZ_pE[0] = const1;
				test1_ZZ_pE[1] = const1;
				test1_ZZ_pE[2] = NTL::ZZ_pE::zero();

			NTL::ZZ_pE test2_ZZ_pE[4];
				test2_ZZ_pE[0] = const1;
				test2_ZZ_pE[1] = const1;
				test2_ZZ_pE[2] = const1;
				test2_ZZ_pE[3] = const1;

			TEST_ASSERT(isElementOfRingWithLinfMax(NTL::to_ZZ(-1),test1_ZZ_pE,3) ==	false)
			TEST_ASSERT(isElementOfRingWithLinfMax(NTL::to_ZZ(-1),test1_ZZ_pE,0) ==	false)
		}

		void test_num_of_coeff_not_0(){
			initialize_ZZ_pE(NTL::to_ZZ(13),5);
			NTL::ZZ_pE const1 = NTL::random_ZZ_pE();

			NTL::ZZ_pE test1_ZZ_pE[3];
				test1_ZZ_pE[0] = const1;
				test1_ZZ_pE[1] = const1;
				test1_ZZ_pE[2] = NTL::ZZ_pE::zero();

			NTL::ZZ_pE test2_ZZ_pE[4];
				test2_ZZ_pE[0] = const1;
				test2_ZZ_pE[1] = const1;
				test2_ZZ_pE[2] = const1;
				test2_ZZ_pE[3] = const1;

			TEST_ASSERT(coeffsNot0(NTL::ZZ_pX::zero())	== 0)
			TEST_ASSERT(coeffsNot0(NTL::ZZ_pX::zero())	== 0)
		}

		void test_random_element_R_with_Linf(){

				NTL::ZZ max_length = NTL::to_ZZ(random());
				TEST_ASSERT(LinfElement(rep(randomElementOfRingWithLinfMax(max_length))) <= max_length)
		}

		void test_generate_element_Dc(){
			std::vector<unsigned char> empty_char_vector(40);
			unsigned int index = 0;
			while(index<empty_char_vector.size()){
				empty_char_vector.push_back(0);
			}

		//	TEST_ASSERT(generate_element_Dc(NTL::to_ZZ(1),NTL::to_ZZ(1), empty_char_vector) == NTL::ZZ_pE::zero())
		}

		void test_ZZ_pE_convert(){
			struct PublicParameter PP = {512, 4, 127, 24};
			unsigned long long length;
			NTL::ZZ p = test_input_parameter(PP);
			initialize_ZZ_pE(p,512);
			NTL::ZZ_pE test[4];
			NTL::ZZ_pE test_rest[4];
			unsigned char* char_array = (unsigned char*)malloc(sizeof(char)*20000);

			generate_random_hash_function(test,4);
			convert_ZZ_pE_array_to_char_array(test,4,&length, char_array);
			charArrayToZZpEArray(test_rest,char_array,length,4,512);

			TEST_ASSERT(compare_ZZ_pE(test,test_rest,4) == 0);
			free(char_array);
		}

		void test_crypto_sign_keypair(){
			unsigned char sk[CRYPTO_SECRETKEYBYTES];
			unsigned  char pk[CRYPTO_PUBLICKEYBYTES];

			FSLWE25_crypto_sign_keypair(pk, sk);
			NTL::ZZ_pE hash_polynomial_sk[PUBLIC_PARAMETER_M];
			NTL::ZZ_pE hash_polynomial_pk[PUBLIC_PARAMETER_M];
			charArrayToZZpEArray(hash_polynomial_sk, (unsigned char*)&sk[BYTES_PRIMENUMBER], BYTES_HASHFUNCTION, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_N);
			charArrayToZZpEArray(hash_polynomial_pk,(unsigned char*)&pk[BYTES_PRIMENUMBER], BYTES_HASHFUNCTION,PUBLIC_PARAMETER_M, PUBLIC_PARAMETER_N);

			TEST_ASSERT(compare_ZZ_pE(hash_polynomial_pk, hash_polynomial_sk, PUBLIC_PARAMETER_M) == 0);
		}

		void test_LWE_hash(){
			struct PublicParameter PP = {512, 4, 127, 24};
			NTL::ZZ p = test_input_parameter(PP);

			initialize_ZZ_pE(p,512);
			NTL::ZZ_pE hash_polynomials[4];
			NTL::ZZ_pE sk[4];
			NTL::ZZ_pE y[4];
			NTL::ZZ_pE z[4];


				generate_random_hash_function(hash_polynomials,4);
				generate_sk(PP,sk);
				NTL::ZZ_pE pk =	LWE_hash(PP,hash_polynomials,sk,4);

				NTL::ZZ_pE random_E = randomElementOfRingWithL1(512,NTL::to_ZZ(24),p);
				NTL::ZZ_pE random_E1 = randomElementOfRingWithL1(512,NTL::to_ZZ(24),p);
				random_element_Dy(PP,y);

				NTL::ZZ_pE Hy = LWE_hash(PP,hash_polynomials,y,4);

				for (int var = 0; var < 4; ++var) {
					z[var] = sk[var]*random_E + y[var];
				}
				NTL::ZZ_pE Hz = LWE_hash(PP, hash_polynomials, z, 4);
				NTL::ZZ_pE Hz_ = pk*random_E + Hy;

				NTL::ZZ_pE Hy_ = Hz - pk*random_E;
				NTL::ZZ_pE Hy_2 = Hz - pk*random_E1;

				TEST_ASSERT(compare_ZZ_pE(&Hy,&Hy_2,1) == 1);
				TEST_ASSERT(compare_ZZ_pE(&Hy,&Hy_,1) == 0);
				TEST_ASSERT(compare_ZZ_pE(&Hz,&Hz_,1) == 0);

		}

		void test_SSL_SHA512(){
			unsigned char md_value1[64];	// message digest
			unsigned char md_value2[64];	// message digest
			char test[] = {"test"};


			SHA512((unsigned char *)test,4,md_value1);
			SHA512((unsigned char *)test,4,md_value2);
			TEST_ASSERT(strcmp((char *)md_value1, (char *)md_value2) == 0);
		}

		void test_Sign_Verify(){
			struct PublicParameter PP = {512, 4, 127, 24};
			char message[] = {"TEST STRING"};
			NTL::ZZ p = test_input_parameter(PP);

			initialize_ZZ_pE(p, PP.n);
			NTL::ZZ_pE hash_polynomials[PP.m];
			NTL::ZZ_pE sk[PP.m];
			NTL::ZZ_pE y[PP.m];
			NTL::ZZ_pE z[PP.m];
			NTL::ZZ_pE Hy;
			NTL::ZZ_pE e;

			NTL::ZZ_pE hash_polynomials_[PP.m];
			NTL::ZZ_pE sk_[PP.m];
			NTL::ZZ_pE z_[PP.m];
			NTL::ZZ_pE pk_[1];
			NTL::ZZ_pE e_[1];
			NTL::ZZ max_length = NTL::to_ZZ(PP.sigma*PP.k*(PP.n*PP.m-1));
			int result;

			unsigned char char_array[PP.n*PP.m*(size_t)(log2(NTL::to_double(p))+1)];
			unsigned long long int length;

			for(int i = 0; i< 1;i++){
				// generate hash polynomial array, transform to char, then back to polynomial and compare both
				generate_random_hash_function(hash_polynomials, PP.m);

				convert_ZZ_pE_array_to_char_array(hash_polynomials, PP.m, &length, char_array);
				charArrayToZZpEArray(hash_polynomials_, char_array, length, PP.m, PP.n);
				TEST_ASSERT(compare_ZZ_pE(hash_polynomials,hash_polynomials_, PP.m) == 0);

				generate_sk(PP,sk);

				convert_ZZ_pE_array_to_char_array(sk, PP.m, &length, char_array);
				charArrayToZZpEArray(sk_,char_array, length, PP.m, PP.n);
				TEST_ASSERT(compare_ZZ_pE(sk,sk_, PP.m) == 0);

				NTL::ZZ_pE pk =	LWE_hash(PP,hash_polynomials,sk,PP.m);

				convert_ZZ_pE_array_to_char_array(&pk, 1, &length, char_array);
				charArrayToZZpEArray(pk_,char_array, length, 1, PP.n);
				TEST_ASSERT(compare_ZZ_pE(&pk,pk_, 1) == 0);

//				sign_message(PP,(unsigned char *)message,(unsigned long long int)sizeof(message)/sizeof(char), sk, hash_polynomials, &e, z);

				do{
					random_element_Dy(PP,y);

					Hy = LWE_hash(PP,hash_polynomials,y,PP.m);

	//				NTL::ZZ_pE e = random_element_Dc(PP.n, NTL::to_ZZ(PP.k), p);
					e = RandomOracle_Dc(PP, Hy,(unsigned char *) message, sizeof(message)/sizeof(char));
					convert_ZZ_pE_array_to_char_array(&e, 1, &length, char_array);
					charArrayToZZpEArray(e_,char_array, length, 1, PP.n);
					TEST_ASSERT(compare_ZZ_pE(&e, e_, 1) == 0);

					for (int var = 0; var < PP.m; ++var) {
						z[var] = sk[var]*e + y[var];
					}
				}while(!isElementOfRingWithLinfMax(max_length, z, PP.m));

				convert_ZZ_pE_array_to_char_array(z, PP.m, &length, char_array);
				charArrayToZZpEArray(z_,char_array, length, PP.m, PP.n);
				TEST_ASSERT(compare_ZZ_pE(z,z_, PP.m) == 0);

				NTL::ZZ_pE Hz = LWE_hash(PP, hash_polynomials, z, PP.m);
				NTL::ZZ_pE Hz_ = pk*e + Hy;

				NTL::ZZ_pE Hy_ = Hz - pk*e;

				TEST_ASSERT(compare_ZZ_pE(&Hy,&Hy_,1) == 0);
				TEST_ASSERT(compare_ZZ_pE(&Hz,&Hz_,1) == 0);
				if(compare_ZZ_pE(&Hy,&Hy_,1) != 0){
					std::cout<<"Hy != Hy_\n";
					return;
				}
				if(compare_ZZ_pE(&Hz,&Hz_,1) != 0){
					std::cout<<"Hz != Hz_!\n";
					return;
				}
/*
result = verify_signatur(PP,(unsigned char *)message, z, &e, hash_polynomials, pk, PP.m, (unsigned long long int)sizeof(message)/sizeof(char));
				if(result != 0){
					return;
				}
				TEST_ASSERT(result == 0);
*/
			}
		}

};

bool run_test(){

	TestSuite test1;

	Test::TextOutput outca(Test::TextOutput::Verbose);

	return test1.run(outca, true);
}

int main(){
	run_test();
}

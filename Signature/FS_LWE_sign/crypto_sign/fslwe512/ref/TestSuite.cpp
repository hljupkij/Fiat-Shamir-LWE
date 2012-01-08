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

class TestSuite : public Test::Suite
{
	public:
		TestSuite()
		{
			TEST_ADD(TestSuite::test_ZZ_binomial)
			TEST_ADD(TestSuite::test_initialize_ZZ_pE)
			TEST_ADD(TestSuite::test_compare_ZZ_pE)
			TEST_ADD(TestSuite::test_count_elements_up_to_L1)
			TEST_ADD(TestSuite::test_Linf_norm_ZZ_pE)
			TEST_ADD(TestSuite::test_L1_norm_ZZ_pE)
			TEST_ADD(TestSuite::test_count_elements_with_L1)
			TEST_ADD(TestSuite::test_count_elements_with_L1_and_coeffs)
			TEST_ADD(TestSuite::test_is_element_Gm)
			TEST_ADD(TestSuite::test_num_of_coeff_not_0)
			TEST_ADD(TestSuite::test_random_element_R_with_Linf)
		//	TEST_ADD(TestSuite::test_generate_element_Dc)
		}

	private:
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
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 5) == EXIT_FAILURE)
			// test if length is not equal if test1 < test2
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test2_ZZ_pE, 4) == EXIT_FAILURE)
			// test if length = 0
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 0) == EXIT_FAILURE)
			// test if length > len(test1) or len(test2)
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test2_ZZ_pE, 5) == EXIT_FAILURE)
			// test for correct
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 3) == EXIT_SUCCESS)
		}

		void test_count_elements_up_to_L1(){
			TEST_ASSERT(count_elements_up_to_L1(0,2) == NTL::ZZ::zero())
			TEST_ASSERT(count_elements_up_to_L1(3,0) == NTL::to_ZZ(1))
			TEST_ASSERT(count_elements_up_to_L1(3,3) == NTL::to_ZZ(63))
		}

		void test_count_elements_with_L1(){

			TEST_ASSERT(count_elements_with_L1(0,0) == 0)
			TEST_ASSERT(count_elements_with_L1(1,0) == 1)
			TEST_ASSERT(count_elements_with_L1(0,1) == 0)
			TEST_ASSERT(count_elements_with_L1(1,1) == 2)
		}

		void test_count_elements_with_L1_and_coeffs(){

			TEST_ASSERT(count_elements_with_L1_and_coeffs(2,0,0)	==	NTL::to_ZZ(1))
			TEST_ASSERT(count_elements_with_L1_and_coeffs(0,1,1)	==  NTL::ZZ::zero())
			TEST_ASSERT(count_elements_with_L1_and_coeffs(0,0,1)	==	NTL::ZZ::zero())

			TEST_ASSERT(count_elements_with_L1_and_coeffs(0,0,0)	==	NTL::ZZ::zero())
			TEST_ASSERT(count_elements_with_L1_and_coeffs(2,1,0)	==	NTL::ZZ::zero())
			// if coeff > L1
			TEST_ASSERT(count_elements_with_L1_and_coeffs(2,1,2)	==	NTL::ZZ::zero())

			TEST_ASSERT(count_elements_with_L1_and_coeffs(-2,1,2)	==	NTL::ZZ::zero())
			TEST_ASSERT(count_elements_with_L1_and_coeffs(2,-1,2)	==	NTL::ZZ::zero())
			TEST_ASSERT(count_elements_with_L1_and_coeffs(2,2,-1)	==	NTL::ZZ::zero())
		}

		void test_Linf_norm_ZZ_pE(){
			NTL::ZZ_pE one = NTL::to_ZZ_pE(-1);

			TEST_ASSERT(Linf_norm_ZZ_pE(one) 				== 	NTL::to_ZZ(1))
			TEST_ASSERT(Linf_norm_ZZ_pE(NTL::ZZ_pE::zero())	==	NTL::ZZ::zero())
		}

		void test_L1_norm_ZZ_pE(){
			NTL::ZZ_pE one = NTL::to_ZZ_pE(-1);

			TEST_ASSERT(L1_norm_ZZ_pE(one) 						== NTL::to_ZZ(1))
			TEST_ASSERT(L1_norm_ZZ_pE(NTL::ZZ_pE::zero())		== NTL::ZZ::zero())
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

			TEST_ASSERT(is_element_with_max_Linf(NTL::to_ZZ(-1),test1_ZZ_pE,3) ==	false)
			TEST_ASSERT(is_element_with_max_Linf(NTL::to_ZZ(-1),test1_ZZ_pE,0) ==	false)
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

			TEST_ASSERT(num_of_coeff_not_0(NTL::ZZ_pE::zero())	== 0)
			TEST_ASSERT(num_of_coeff_not_0(NTL::ZZ_pE::zero())	== 0)
		}

		void test_random_element_R_with_Linf(){

				NTL::ZZ max_length = NTL::to_ZZ(random());
				TEST_ASSERT(Linf_norm_ZZ_pE(random_element_R_with_Linf(max_length)) <= max_length)
		}

		void test_generate_element_Dc(){
			std::vector<unsigned char> empty_char_vector(40);
			unsigned int index = 0;
			while(index<empty_char_vector.size()){
				empty_char_vector.push_back(0);
			}

		//	TEST_ASSERT(generate_element_Dc(NTL::to_ZZ(1),NTL::to_ZZ(1), empty_char_vector) == NTL::ZZ_pE::zero())
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

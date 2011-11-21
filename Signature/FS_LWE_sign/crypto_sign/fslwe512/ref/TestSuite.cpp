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
			TEST_ADD(TestSuite::test_ZZ_bin)
			TEST_ADD(TestSuite::test_initialize_ZZ_pE)
			TEST_ADD(TestSuite::test_compare_ZZ_pE)
			TEST_ADD(TestSuite::test_count_elements_up_to_L1)
		}

	private:
		void test_ZZ_bin(){
			// if one coefficient is 0
			TEST_ASSERT(ZZ_binomial( NTL::to_ZZ(2), NTL::ZZ::zero()) == NTL::to_ZZ(1))
			// if n is smaller as k
			TEST_ASSERT(ZZ_binomial(NTL::to_ZZ(1), NTL::to_ZZ(2)) == NTL::ZZ::zero())
			// normal operation
			TEST_ASSERT(ZZ_binomial(NTL::to_ZZ(6), NTL::to_ZZ(3)) == NTL::to_ZZ(20))
		}

		void test_initialize_ZZ_pE(){

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
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 5) == EXIT_FAILURE)
			// test for correct
			TEST_ASSERT(compare_ZZ_pE(test1_ZZ_pE, test1_ZZ_pE, 3) == EXIT_SUCCESS)
		}

		void test_count_elements_up_to_L1(){
			TEST_ASSERT(count_elements_up_to_L1(0,2) == NTL::ZZ::zero())
			TEST_ASSERT(count_elements_up_to_L1(3,0) == NTL::to_ZZ(1))
			TEST_ASSERT(count_elements_up_to_L1(3,3) == NTL::to_ZZ(63))
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

/*
 * test.cpp
 *
 *  Created on: 10.07.2011
 *      Author: wladimir
 */

#include "crypto_sign.h"
#include "api.h"
#include "fs_lwe_sign.h"
#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace NTL;
using namespace std;

int main()
{
	ZZ n, m, sigma, k, p, norm, random_seed;
	ZZ_pX P, y, z;

	struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};

	/* Set seed (8 Bytes) for PRNG */
	init_random(8);

	cout << "n:" << PP.n << "\tm:" << PP.m << "\tsigma:" << PP.sigma << "\tk:" << PP.k << "\n";

	timespec time_start, time_end;
	float time_key_generation = 0, time_signature = 0, time_verification= 0;
	ifstream file;
	char* memblock;
	ifstream::pos_type size;
	char* filename = (char*)malloc(50*sizeof(char));

	printf("Dateiname der zu signierenden Datei eingeben:\n");
	cin>>filename;

	if(filename != NULL){
		file.open(filename, ifstream::in | ifstream::binary| ifstream::ate);
		if(!file.fail() && file.is_open()){
			size = file.tellg();
			memblock = (char*)malloc((unsigned long)size*sizeof(char));
			file.seekg (0, ios::beg);
			file.read (memblock, (int)size);
			file.close();
		}
	}

	unsigned char test_message[] = "TEST STRING";
	if(memblock == NULL){
		memblock = (char*)test_message;
		size = sizeof(test_message)/sizeof(char);
	}

	int number_tests = 1;
	printf("Anzahl der Testdurchläufe eingeben:");
	scanf("%d",&number_tests);



	printf("\n\n------------------------------------------eBATS-Wrapper----------------------------\n\n");
	unsigned char sk_[CRYPTO_SECRETKEYBYTES];
	unsigned char pk_[CRYPTO_PUBLICKEYBYTES];

	unsigned long long len, len_m;
	unsigned char* signed_message = (unsigned char*) malloc(sizeof(char)*((unsigned long)size+CRYPTO_BYTES));
	unsigned char* message = (unsigned char*)malloc(sizeof(char)*(unsigned long)size);

	for (int var = 0; var < number_tests; ++var) {
	//	printf("Start test Nr.%i\n",var);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
		crypto_sign_keypair(pk_,sk_);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_end);
	//	printf("Generating sk and pk takes about %.5f-seconds.\n",(float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000));
		time_key_generation += (float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_start);
		crypto_sign(signed_message, &len, (const unsigned char*)memblock,(unsigned long long) size, (const unsigned char*)sk_);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_end);
	//	printf("Computing signature takes about %.5f-seconds.\n",(float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000));
		time_signature += (float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000);


		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_start);
		if(crypto_sign_open(message, &len_m, signed_message, len, pk_)){
	//		printf("Verification not successful!\n");
			break;
		}else{
	//		printf("Verification successful!\n");
		}
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_end);
	//	printf("Verification takes about %.5f-seconds.\n",(float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000));
		time_verification += (float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000);
	}

	free(filename);
	free(message);
	free(signed_message);

	if((unsigned char*)memblock != test_message){
		free(memblock);
	}
	printf("Mean time by %i-tests for\n",number_tests);
	printf("Key generation: %.5f s\n",(time_key_generation/number_tests));
	printf("Signing: %.5f s\n",(time_signature/number_tests));
	printf("Verification: %.5f s\n",(time_verification/number_tests));

	printf("\n\n------------------------------------------TEST-FUNCTIONS----------------------------\n\n");
/*
	if(test_conversion_ZZ_pE_to_char(10)){
		printf("Test conversion not successful!\n");
	}else{
		printf("Test conversion successful!\n");
	}
*/
}

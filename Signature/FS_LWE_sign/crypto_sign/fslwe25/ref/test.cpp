/*
 * test.cpp
 *
 * Application to test function of Fiat-Shamir LWE signature algorithm.
 *
 *  Created on: 10.07.2011
 *      Author: wladimir
 */

#include "sizes.h"
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
#include <time.h>


using namespace NTL;
using namespace std;

void show_parameter(){
	cout << "n:" << PUBLIC_PARAMETER_N << "\tm:" << PUBLIC_PARAMETER_M << "\tsigma:" << PUBLIC_PARAMETER_SIGMA << "\tk:" << PUBLIC_PARAMETER_K << "\n";
	cout<<"M*N*SIGMA*K="<<to_ZZ(PUBLIC_PARAMETER_M*PUBLIC_PARAMETER_N*PUBLIC_PARAMETER_K*PUBLIC_PARAMETER_SIGMA)<<"\n";

	cout<<"BYTES_PRIMENUMBER:"<<BYTES_PRIMENUMBER<<"\n";
	cout<<"BYTES_Dy:"<<BYTES_Dy<<"\n";
	cout<<"BYTES_SECRETKEY:"<<BYTES_SECRETKEY<<"\n";
	cout<<"BYTES_HASHFUNCTION:"<<BYTES_HASHFUNCTION<<"\n";
	cout<<"BYTES_PUBLICKEY:"<<BYTES_PUBLICKEY<<"\n";
	cout<<"BYTES_SIGNATUR_Z:"<<BYTES_SIGNATUR_Z<<"\n";
	cout<<"BYTES_SIGNATUR_E:"<<BYTES_SIGNATUR_E<<"\n";

	cout<<"BITS_HIGH_BYTE:"<<(int)BITS_HIGH_BYTE<<"\n";
	cout<<"BLOCKSIZE:"<<(int)BLOCKSIZE<<"\n";
	cout<<"const_BYTES_NUMBER:"<<(int)const_BYTES_NUMBER<<"\n";
	cout<<"const_BITS_NUMBER:"<<(int)const_BITS_NUMBER<<"\n";


	cout<<"Signaturlänge CRYPTO_BYTES:"<<CRYPTO_BYTES<<"\n";
	cout<<"Länge SK:"<<CRYPTO_SECRETKEYBYTES<<"\n";
	cout<<"Länge PK:"<<CRYPTO_PUBLICKEYBYTES<<"\n";
}

class FSLWE {
	NTL::ZZ _L1_elements[];
public:
	FSLWE(PublicParameter PP){
	//	_L1_elements = NTL::ZZ[PP.sigma];

	}

private:

};

int main()
{
	ZZ n, m, sigma, k, p, norm, random_seed;
	ZZ_pX P, y, z;

	struct PublicParameter PP = {PUBLIC_PARAMETER_N, PUBLIC_PARAMETER_M,PUBLIC_PARAMETER_SIGMA,PUBLIC_PARAMETER_K};
	show_parameter();
	/* Set seed (8 Bytes) for PRNG */
	init_random(8);

	timespec time_start, time_end;
	float time_key_generation = 0, time_signature = 0, time_verification= 0;
	ifstream file;
	ofstream file_out;
	unsigned char *memblock;
	ifstream::pos_type size;
	char* filename = (char*)malloc(50*sizeof(char));

	printf("Dateiname der zu signierenden Datei eingeben:\n");
	cin>>filename;

	if(filename != NULL){
		file.open(filename, ifstream::in | ifstream::binary| ifstream::ate);
		if(!file.fail() && file.is_open()){
			printf("Try to open file:%s\n",filename);
			size = file.tellg();
			memblock = (unsigned char*)malloc((unsigned long)size*sizeof(unsigned char));
			file.seekg (0, ios::beg);
			file.read ((char*)memblock, (int)size);
			file.close();
		}
	}

	unsigned char test_message[] = "TEST STRING";

	if(memblock == NULL||size == 0){
		memblock = test_message;
		size = sizeof(test_message)/sizeof(char);
	}

	int number_tests = 1;
	printf("Anzahl der Testdurchläufe eingeben:");
	if(scanf("%d",&number_tests) == EOF){
		number_tests = 1;
	}

	int L1_stat[PP.k+1];
	int Koeff_stat[PP.k+1];

	for (int var = 0; var <= PP.k; ++var) {
		L1_stat[var] = 0;
		Koeff_stat[var] = 0;
	}



	printf("\n\n------------------------------------------eBATS-Wrapper----------------------------\n\n");
	unsigned char sk_[CRYPTO_SECRETKEYBYTES];
	unsigned char pk_[CRYPTO_PUBLICKEYBYTES];

	unsigned long long length_signed_message, length_message;
	unsigned char* signed_message = (unsigned char*) malloc(sizeof(char)*((unsigned long)size+CRYPTO_BYTES));
	unsigned char* message = (unsigned char*)malloc(sizeof(char)*(unsigned long)size);

	for (int var = 0; var < number_tests; ++var) {
//		printf("Start test Nr.%i\n",var);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
		crypto_sign_keypair(pk_,sk_);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_end);
//		printf("Generating sk and pk takes about %.5f-seconds.\n",(float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000));
		time_key_generation += (float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000);

/*		file_out.open("public_key",ios::binary);
		if(file_out.is_open()){
			file_out.write((const char *)pk_, CRYPTO_PUBLICKEYBYTES);
		}
		file_out.close();

		file_out.open("secret_key",ios::binary);
		if(file_out.is_open()){
			file_out.write((const char *)sk_, CRYPTO_SECRETKEYBYTES);
		}
		file_out.close();*/

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_start);
		crypto_sign(signed_message, &length_signed_message, (const unsigned char*)memblock,(unsigned long long) size, (const unsigned char*)sk_);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_end);
//		printf("Computing signature takes about %.5f-seconds.\n",(float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000));
		time_signature += (float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000);

/*		file_out.open("signature",ios::binary);
		if(file_out.is_open()){
			file_out.write((const char *)signed_message, length_signed_message);
		}
		file_out.close();*/

		ZZ_pE _e;

		convert_char_array_to_ZZ_pE_array(&_e, &signed_message[size + BYTES_SIGNATUR_Z], BYTES_SIGNATUR_E, 1, PP.n);
		ZZ L1_e =  L1_norm_ZZ_pE(_e);
		int num_e = num_of_coeff_not_0(_e);
		L1_stat[to_int(L1_e)]++;
		Koeff_stat[num_e]++;


		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_start);
		if(crypto_sign_open(message, &length_message, signed_message, length_signed_message, pk_)){
			printf("[%i]Verification not successful!\n",var);
			break;
		}else{
//			printf("Verification successful!\n");
		}
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time_end);
//		printf("Verification takes about %.5f-seconds.\n",(float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000));
		time_verification += (float)((time_end.tv_sec- time_start.tv_sec)+((float)(time_end.tv_nsec-time_start.tv_nsec))/1000000000);
	}

	free(filename);
	free(message);
	free(signed_message);

	if(memblock != test_message){
		free(memblock);
	}

	printf("Mean time for %i-tests and message of size:%i\n",number_tests,(int)size);
	printf("Key generation: %.5f s\n",(time_key_generation/number_tests));
	printf("Signing: %.5f s\n",(time_signature/number_tests));
	printf("Verification: %.5f s\n",(time_verification/number_tests));

	printf("\n\n------------------------------------------TEST-FUNCTIONS----------------------------\n\n");
	// TODO: hier muss noch ein Test zur Verifikation von gespeicherten Signaturen rein.
	// Signatur und die Nachricht lesen und verifizieren!
	// Genauso geheimen Schlüssel lesen und signieren, ohne erst den Schlüssel zu erzeugen
/*
	if(test_conversion_ZZ_pE_to_char(10)){
		printf("Test conversion not successful!\n");
	}else{
		printf("Test conversion successful!\n");
	}
*/
	for(int i = 0; i <= PP.k; i++){
		printf("Elemente mit L1[%i]:=%i\t#coeff = %i\n",i,L1_stat[i],Koeff_stat[i]);
	}
}

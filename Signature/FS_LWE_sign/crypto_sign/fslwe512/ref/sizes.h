/*
 * sizes.h
 *
 *  Created on: 20.11.2011
 *      Author: Wladimir Paulsen
 */

#ifndef SIZES_H_
#define SIZES_H_

#define PUBLIC_PARAMETER_N 			512					// [log2(N)] = [9]  = 	10
#define PUBLIC_PARAMETER_M 			4					// [log2(M)] = [2,...] = 3
#define PUBLIC_PARAMETER_K 			24					// [log2(K)] = [4,..] = 5
#define PUBLIC_PARAMETER_SIGMA 		127					// [log2(SIGMA)] = [6,9..] = 11

//#define LOG2_P 						(ceil(log2(PUBLIC_PARAMETER_SIGMA))+1)*PUBLIC_PARAMETER_M

// [m*([log2(SIGMA)]+1)/8]
#define BYTES_PRIMENUMBER 			(unsigned int)ceil(PUBLIC_PARAMETER_M*(ceil(log2(PUBLIC_PARAMETER_SIGMA))+1)/8)
// [(log2(M)]+[(log2(N)]+[(log2(SIGMA)]+[(log2(K)]+1)/8
#define BYTES_Dy 					(unsigned int)ceil((ceil(log2(PUBLIC_PARAMETER_K*PUBLIC_PARAMETER_M*PUBLIC_PARAMETER_N*PUBLIC_PARAMETER_SIGMA))+1)/8)

#define BYTES_SECRETKEY 			PUBLIC_PARAMETER_N*PUBLIC_PARAMETER_M*(unsigned int)ceil((ceil(log2(PUBLIC_PARAMETER_SIGMA))+1)/8)
#define BYTES_HASHFUNCTION 			PUBLIC_PARAMETER_N*PUBLIC_PARAMETER_M*BYTES_PRIMENUMBER
#define BYTES_PUBLICKEY 			PUBLIC_PARAMETER_N*BYTES_PRIMENUMBER
#define BYTES_SIGNATUR_Z 			PUBLIC_PARAMETER_N*PUBLIC_PARAMETER_M*BYTES_Dy
#define BYTES_SIGNATUR_E 			PUBLIC_PARAMETER_N

#endif /* SIZES_H_ */

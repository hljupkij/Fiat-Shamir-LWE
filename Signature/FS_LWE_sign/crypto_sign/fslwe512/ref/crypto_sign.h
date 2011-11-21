/*
 * crypto_sign.h
 *
 *  Created on: 20.11.2011
 *      Author: wladimir
 */

#ifndef CRYPTO_SIGN_H_
#define CRYPTO_SIGN_H_


int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int crypto_sign(
       unsigned char *sm,unsigned long long *smlen,
       const unsigned char *m,unsigned long long mlen,
       const unsigned char *sk
     );

int crypto_sign_open(
  unsigned char *m,unsigned long long *mlen,
  const unsigned char *sm,unsigned long long smlen,
  const unsigned char *pk
);

#endif /* CRYPTO_SIGN_H_ */

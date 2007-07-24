/*
 * Copyright (c) 2007 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden).
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* $Id$ */

#ifndef HEIM_CAMELLIA_H
#define HEIM_CAMELLIA_H 1

#include "camellia-ntt.h"

/* symbol renaming */
#define CAMELLIA_set_key hc_CAMELLIA_set_encrypt_key
#define CAMELLIA_encrypt hc_CAMELLIA_encrypt
#define CAMELLIA_decrypt hc_CAMELLIA_decrypt
#define CAMELLIA_cbc_encrypt hc_CAMELLIA_cbc_encrypt

/*
 *
 */

#define CAMELLIA_BLOCK_SIZE 16
#define CAMELLIA_MAXNR 14

#define CAMELLIA_ENCRYPT 1
#define CAMELLIA_DECRYPT 0

typedef struct camellia_key {
    unsigned int bits;
    KEY_TABLE_TYPE key;
} CAMELLIA_KEY;

int CAMELLIA_set_key(const unsigned char *, const int, CAMELLIA_KEY *);

void CAMELLIA_encrypt(const unsigned char *, unsigned char *,
		      const CAMELLIA_KEY *);
void CAMELLIA_decrypt(const unsigned char *, unsigned char *,
		      const CAMELLIA_KEY *);

void CAMELLIA_cbc_encrypt(const unsigned char *, unsigned char *,
			  const unsigned long, const CAMELLIA_KEY *,
			  unsigned char *, int);

#endif /* HEIM_CAMELLIA_H */

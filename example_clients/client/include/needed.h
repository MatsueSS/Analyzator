#ifndef _NEEDED_H_
#define _NEEDED_H_

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SALT_SIZE 16
#define IV_SIZE 16
#define KEY_SIZE 32

//need free
char* base64_encode(const unsigned char* input, int length);

//need free
unsigned char *base64_decode(const char *base64data, size_t *out_len);

#endif //_NEEDED_H_
#include "needed.h"

//need free
char* base64_encode(const unsigned char* input, int length) {
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // без переносов строк
    BIO_write(b64, input, length);
    BIO_flush(b64);

    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);

    char* buff = (char*)malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = '\0';

    BIO_free_all(b64);
    return buff;
}

//need free
unsigned char *base64_decode(const char *base64data, size_t *out_len) {
    BIO *bio, *b64;
    size_t decodeLen = strlen(base64data);
    unsigned char *buffer = (unsigned char *)malloc(decodeLen);
    if (!buffer) return NULL;

    bio = BIO_new_mem_buf(base64data, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    *out_len = BIO_read(bio, buffer, decodeLen);
    BIO_free_all(bio);

    return buffer;
}
#include "cipher.h"

char* cipher(const char* theme, const char* psw)
{
    unsigned char salt[SALT_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char key[KEY_SIZE];

    RAND_bytes(salt, sizeof(salt));
    RAND_bytes(iv, sizeof(iv));

    if (!PKCS5_PBKDF2_HMAC(theme, strlen(theme), salt, sizeof(salt),
                           10000, EVP_sha256(), sizeof(key), key)) {
        fprintf(stderr, "Error generating key\n");
        exit(1);
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int outlen, finallen;
    unsigned char ciphertext[1024];

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &outlen, (unsigned char*)psw, strlen(psw));
    EVP_EncryptFinal_ex(ctx, ciphertext + outlen, &finallen);
    int ciphertext_len = outlen + finallen;
    EVP_CIPHER_CTX_free(ctx);

    int total_len = SALT_SIZE + IV_SIZE + ciphertext_len;
    unsigned char *combined = malloc(total_len);
    memcpy(combined, salt, SALT_SIZE);
    memcpy(combined + SALT_SIZE, iv, IV_SIZE);
    memcpy(combined + SALT_SIZE + IV_SIZE, ciphertext, ciphertext_len);

    char* encoded = base64_encode(combined, total_len);
    free(combined);

    return encoded;
}
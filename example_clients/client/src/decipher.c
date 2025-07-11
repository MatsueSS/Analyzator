#include "decipher.h"

//need free
unsigned char* decipher(const char* theme, const char* encrypted)
{
    size_t decoded_len;
    unsigned char *decoded_data = base64_decode(encrypted, &decoded_len);

    if (decoded_len < SALT_SIZE + IV_SIZE) {
        fprintf(stderr, "Данные слишком короткие\n");
        free(decoded_data);
        exit(1);
    }

    unsigned char salt[SALT_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char key[KEY_SIZE];

    memcpy(salt, decoded_data, SALT_SIZE);
    memcpy(iv, decoded_data + SALT_SIZE, IV_SIZE);

    int ciphertext_len = decoded_len - SALT_SIZE - IV_SIZE;
    unsigned char *ciphertext = decoded_data + SALT_SIZE + IV_SIZE;

    // Генерация ключа из пароля и соли
    if (!PKCS5_PBKDF2_HMAC(theme, strlen(theme), salt, SALT_SIZE, 10000, EVP_sha256(), KEY_SIZE, key)) {
        fprintf(stderr, "Ошибка генерации ключа\n");
        free(decoded_data);
        exit(1);
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Ошибка создания контекста EVP\n");
        free(decoded_data);
        exit(1);
    }

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        fprintf(stderr, "Ошибка инициализации расшифровки\n");
        EVP_CIPHER_CTX_free(ctx);
        free(decoded_data);
        exit(1);
    }

    unsigned char* plaintext = (char *)malloc(1024*sizeof(char));
    int len, plaintext_len;

    if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        fprintf(stderr, "Ошибка во время расшифровки\n");
        EVP_CIPHER_CTX_free(ctx);
        free(decoded_data);
        exit(1);
    }
    plaintext_len = len;

    if (!EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        fprintf(stderr, "Ошибка при финализации расшифровки\n");
        EVP_CIPHER_CTX_free(ctx);
        free(decoded_data);
        exit(1);
    }
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    plaintext[plaintext_len] = '\0';
    free(decoded_data);
    return plaintext;
}
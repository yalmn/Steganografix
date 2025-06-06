#ifndef ENCRYPTION_AES_H
#define ENCRYPTION_AES_H

#include <stddef.h>

int aes_encrypt(const unsigned char *in, size_t in_len, unsigned char **out,
                size_t *out_len, const char *key);
int aes_decrypt(const unsigned char *in, size_t in_len, unsigned char **out,
                size_t *out_len, const char *key);

#endif

#include "../include/encryption_aes.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdlib.h>
#include <string.h>

// 16 Bytes = AES-128
#define AES_KEYLEN 16
#define AES_BLOCKLEN 16

int aes_encrypt(const unsigned char *in, size_t in_len, unsigned char **out,
                size_t *out_len, const char *key) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
    return 1;

  unsigned char iv[AES_BLOCKLEN];
  if (!RAND_bytes(iv, sizeof(iv)))
    return 2;

  *out_len = in_len + AES_BLOCKLEN + AES_BLOCKLEN; // Puffer + IV + Padding
  *out = malloc(*out_len);
  if (!*out)
    return 3;

  // Kopiere IV an den Anfang
  memcpy(*out, iv, AES_BLOCKLEN);

  int len, total = 0;

  EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char *)key, iv);
  EVP_EncryptUpdate(ctx, *out + AES_BLOCKLEN, &len, in, in_len);
  total += len;
  EVP_EncryptFinal_ex(ctx, *out + AES_BLOCKLEN + total, &len);
  total += len;

  *out_len = AES_BLOCKLEN + total; // inkl. IV
  EVP_CIPHER_CTX_free(ctx);
  return 0;
}

int aes_decrypt(const unsigned char *in, size_t in_len, unsigned char **out,
                size_t *out_len, const char *key) {
  if (in_len < AES_BLOCKLEN)
    return 1; // IV fehlt

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
    return 2;

  const unsigned char *iv = in;
  const unsigned char *cipher = in + AES_BLOCKLEN;
  size_t cipher_len = in_len - AES_BLOCKLEN;

  *out = malloc(cipher_len);
  if (!*out)
    return 3;

  int len, total = 0;

  EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char *)key, iv);
  EVP_DecryptUpdate(ctx, *out, &len, cipher, cipher_len);
  total += len;
  EVP_DecryptFinal_ex(ctx, *out + total, &len);
  total += len;

  *out_len = total;
  EVP_CIPHER_CTX_free(ctx);
  return 0;
}

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

void xor_encrypt(unsigned char *data, size_t len, const char *key);

#endif

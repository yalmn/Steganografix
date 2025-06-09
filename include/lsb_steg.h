#ifndef LSB_STEG_H
#define LSB_STEG_H

#include <stdbool.h>

int embed_payload_lsb(const char *input_image, const char *payload_file,
                      const char *output_image, const char *key, bool use_aes);

int extract_payload_lsb(const char *input_image, const char *output_payload,
                        const char *key, bool use_aes);

#endif

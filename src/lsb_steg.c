#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "../include/stb_image_write.h"

#include "../include/lsb_steg.h"
#include "../include/encryption.h"
#include "../include/encryption_aes.h"
#include "../include/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static unsigned int fnv1a(const char *key) {
    unsigned int hash = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)key; *p; p++) {
        hash ^= *p;
        hash *= 16777619u;
    }
    return hash;
}

static void shuffle_indices(size_t *idx, size_t n, unsigned int seed) {
    srand(seed);
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        size_t tmp = idx[i];
        idx[i] = idx[j];
        idx[j] = tmp;
    }
}

int embed_payload_lsb(const char *input_image, const char *payload_file,
                      const char *output_image, const char *key, bool use_aes) {
    int width, height, channels;
    unsigned char *img = stbi_load(input_image, &width, &height, &channels, 0);
    if (!img) {
        log_error("Image loading failed.");
        return 1;
    }

    FILE *pfile = fopen(payload_file, "rb");
    if (!pfile) {
        log_error("Payload file open failed.");
        stbi_image_free(img);
        return 1;
    }
    fseek(pfile, 0, SEEK_END);
    long payload_len = ftell(pfile);
    rewind(pfile);
    unsigned char *payload_buf = malloc(payload_len);
    if (!payload_buf) {
        fclose(pfile);
        stbi_image_free(img);
        log_error("Memory allocation failed.");
        return 1;
    }
    fread(payload_buf, 1, payload_len, pfile);
    fclose(pfile);

    unsigned char *enc_buf = NULL;
    size_t enc_len = 0;
    if (use_aes) {
        if (aes_encrypt(payload_buf, payload_len, &enc_buf, &enc_len, key) != 0) {
            free(payload_buf);
            stbi_image_free(img);
            log_error("AES encryption failed.");
            return 1;
        }
        free(payload_buf);
    } else {
        xor_encrypt(payload_buf, payload_len, key);
        enc_buf = payload_buf;
        enc_len = payload_len;
    }

    size_t capacity_bits = (size_t)width * height * channels;
    size_t needed_bits = (enc_len + 4) * 8; // 4 bytes length header
    if (needed_bits > capacity_bits) {
        stbi_image_free(img);
        if (use_aes && enc_buf)
            free(enc_buf);
        log_error("Image too small for payload.");
        return 1;
    }

    size_t total_bytes = width * height * channels;
    size_t *indices = malloc(total_bytes * sizeof(size_t));
    if (!indices) {
        stbi_image_free(img);
        if (use_aes && enc_buf)
            free(enc_buf);
        log_error("Memory allocation failed.");
        return 1;
    }
    for (size_t i = 0; i < total_bytes; i++)
        indices[i] = i;
    shuffle_indices(indices, total_bytes, fnv1a(key));

    unsigned char *data = enc_buf;
    unsigned char header[4];
    header[0] = (enc_len >> 24) & 0xFF;
    header[1] = (enc_len >> 16) & 0xFF;
    header[2] = (enc_len >> 8) & 0xFF;
    header[3] = enc_len & 0xFF;

    size_t bit_pos = 0;
    for (int i = 0; i < 4; i++) {
        for (int b = 7; b >= 0; b--) {
            size_t idx = indices[bit_pos++];
            img[idx] = (img[idx] & ~1) | ((header[i] >> b) & 1);
        }
    }
    for (size_t i = 0; i < enc_len; i++) {
        for (int b = 7; b >= 0; b--) {
            size_t idx = indices[bit_pos++];
            img[idx] = (img[idx] & ~1) | ((data[i] >> b) & 1);
        }
    }

    int success = 0;
    const char *ext = strrchr(output_image, '.');
    if (ext && (strcmp(ext, ".png") == 0 || strcmp(ext, ".PNG") == 0)) {
        if (!stbi_write_png(output_image, width, height, channels, img, width * channels))
            success = 1;
    } else if (ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".JPG") == 0 || strcmp(ext, ".JPEG") == 0)) {
        if (!stbi_write_jpg(output_image, width, height, channels, img, 100))
            success = 1;
    } else {
        log_error("Output format must be .png or .jpg");
        success = 1;
    }

    stbi_image_free(img);
    free(indices);
    if (use_aes && enc_buf)
        free(enc_buf);

    return success;
}

int extract_payload_lsb(const char *input_image, const char *output_payload,
                        const char *key, bool use_aes) {
    int width, height, channels;
    unsigned char *img = stbi_load(input_image, &width, &height, &channels, 0);
    if (!img) {
        log_error("Image loading failed.");
        return 1;
    }

    size_t total_bytes = width * height * channels;
    size_t *indices = malloc(total_bytes * sizeof(size_t));
    if (!indices) {
        stbi_image_free(img);
        log_error("Memory allocation failed.");
        return 1;
    }
    for (size_t i = 0; i < total_bytes; i++)
        indices[i] = i;
    shuffle_indices(indices, total_bytes, fnv1a(key));

    size_t bit_pos = 0;
    unsigned int enc_len = 0;
    for (int i = 0; i < 4; i++) {
        for (int b = 7; b >= 0; b--) {
            size_t idx = indices[bit_pos++];
            enc_len |= (img[idx] & 1) << (31 - (i * 8 + (7 - b)));
        }
    }

    size_t capacity_bits = (size_t)width * height * channels;
    if ((enc_len + 4) * 8 > capacity_bits) {
        stbi_image_free(img);
        free(indices);
        log_error("Invalid payload length.");
        return 1;
    }

    unsigned char *enc_buf = malloc(enc_len);
    if (!enc_buf) {
        stbi_image_free(img);
        free(indices);
        log_error("Memory allocation failed.");
        return 1;
    }
    for (size_t i = 0; i < enc_len; i++) {
        unsigned char byte = 0;
        for (int b = 7; b >= 0; b--) {
            size_t idx = indices[bit_pos++];
            byte |= (img[idx] & 1) << b;
        }
        enc_buf[i] = byte;
    }

    stbi_image_free(img);
    free(indices);

    unsigned char *plain = NULL;
    size_t plain_len = 0;
    if (use_aes) {
        if (aes_decrypt(enc_buf, enc_len, &plain, &plain_len, key) != 0) {
            free(enc_buf);
            log_error("AES decryption failed.");
            return 1;
        }
        free(enc_buf);
    } else {
        xor_encrypt(enc_buf, enc_len, key);
        plain = enc_buf;
        plain_len = enc_len;
    }

    FILE *out = fopen(output_payload, "wb");
    if (!out) {
        log_error("Output file open failed.");
        if (use_aes && plain)
            free(plain);
        return 1;
    }
    fwrite(plain, 1, plain_len, out);
    fclose(out);
    if (use_aes && plain)
        free(plain);

    return 0;
}


#include "../include/steg_extract.h"
#include "../include/encryption.h"
#include "../include/encryption_aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MARKER_START "STEGO_START"
#define MARKER_END   "STEGO_END"
#define MARKER_START_LEN 10
#define MARKER_END_LEN   8

// Kompatibel zu macOS: eigener memmem-Ersatz
void *memsearch(const void *haystack, size_t haystack_len, const void *needle, size_t needle_len) {
    if (!needle || needle_len == 0 || haystack_len < needle_len) return NULL;
    const unsigned char *h = haystack;
    for (size_t i = 0; i <= haystack_len - needle_len; ++i) {
        if (memcmp(h + i, needle, needle_len) == 0) {
            return (void *)(h + i);
        }
    }
    return NULL;
}

int extract_payload(const char *input_file, const char *output_payload_file, const char *key, bool use_aes) {
    FILE *in_fp = fopen(input_file, "rb");
    FILE *out_fp = fopen(output_payload_file, "wb");

    if (!in_fp || !out_fp) {
        fprintf(stderr, "[!] Failed to open input or output file.\n");
        if (in_fp) fclose(in_fp);
        if (out_fp) fclose(out_fp);
        return 1;
    }

    // Ganze Datei lesen
    fseek(in_fp, 0, SEEK_END);
    long filesize = ftell(in_fp);
    rewind(in_fp);

    unsigned char *buffer = malloc(filesize);
    if (!buffer) {
        fprintf(stderr, "[!] Memory allocation failed.\n");
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    fread(buffer, 1, filesize, in_fp);
    fclose(in_fp);

    // Marker suchen
    unsigned char *start = memsearch(buffer, filesize, MARKER_START, MARKER_START_LEN);
    if (!start) {
        fprintf(stderr, "[!] Start marker not found.\n");
        free(buffer); fclose(out_fp);
        return 1;
    }

    unsigned char *end = memsearch(start + MARKER_START_LEN,
                                   filesize - (start - buffer) - MARKER_START_LEN,
                                   MARKER_END, MARKER_END_LEN);
    if (!end) {
        fprintf(stderr, "[!] End marker not found.\n");
        free(buffer); fclose(out_fp);
        return 1;
    }

    size_t enc_len = end - (start + MARKER_START_LEN);
    unsigned char *enc_data = malloc(enc_len);
    if (!enc_data) {
        fprintf(stderr, "[!] Memory allocation failed.\n");
        free(buffer); fclose(out_fp);
        return 1;
    }

    memcpy(enc_data, start + MARKER_START_LEN, enc_len);
    free(buffer);  // Hauptpuffer kann jetzt weg

    unsigned char *plain = NULL;
    size_t plain_len = 0;

    if (use_aes) {
        if (aes_decrypt(enc_data, enc_len, &plain, &plain_len, key) != 0) {
            fprintf(stderr, "[!] AES decryption failed.\n");
            free(enc_data); fclose(out_fp);
            return 1;
        }
        free(enc_data);
    } else {
        xor_encrypt(enc_data, enc_len, key);  // XOR = symmetrisch
        plain = enc_data;
        plain_len = enc_len;
    }

    fwrite(plain, 1, plain_len, out_fp);
    if (use_aes && plain) free(plain);
    fclose(out_fp);

    return 0;
}


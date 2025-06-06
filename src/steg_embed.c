#include "../include/steg_embed.h"
#include "../include/encryption.h"
#include "../include/encryption_aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MARKER_START "STEGO_START"
#define MARKER_END "STEGO_END"

int embed_payload(const char *input_file, const char *payload_file,
                  const char *output_file, const char *key, bool use_aes) {
  FILE *in_fp = fopen(input_file, "rb");
  FILE *payload_fp = fopen(payload_file, "rb");
  FILE *out_fp = fopen(output_file, "wb");

  if (!in_fp || !payload_fp || !out_fp) {
    fprintf(stderr, "[!] Failed to open one or more files.\n");
    if (in_fp)
      fclose(in_fp);
    if (payload_fp)
      fclose(payload_fp);
    if (out_fp)
      fclose(out_fp);
    return 1;
  }

  // Eingabedatei vollständig in Ausgabedatei schreiben
  int ch;
  while ((ch = fgetc(in_fp)) != EOF) {
    fputc(ch, out_fp);
  }

  // Payload lesen
  fseek(payload_fp, 0, SEEK_END);
  long payload_len = ftell(payload_fp);
  rewind(payload_fp);

  unsigned char *payload_buf = malloc(payload_len);
  if (!payload_buf) {
    fprintf(stderr, "[!] Memory allocation failed.\n");
    fclose(in_fp);
    fclose(payload_fp);
    fclose(out_fp);
    return 1;
  }

  fread(payload_buf, 1, payload_len, payload_fp);
  fclose(payload_fp);

  unsigned char *enc_buf = NULL;
  size_t enc_len = 0;

  if (use_aes) {
    // AES-Verschlüsselung
    if (aes_encrypt(payload_buf, payload_len, &enc_buf, &enc_len, key) != 0) {
      fprintf(stderr, "[!] AES encryption failed.\n");
      free(payload_buf);
      fclose(in_fp);
      fclose(out_fp);
      return 1;
    }
    free(payload_buf);
  } else {
    // XOR-Verschlüsselung in-place
    xor_encrypt(payload_buf, payload_len, key);
    enc_buf = payload_buf;
    enc_len = payload_len;
  }

  // Marker + verschlüsselten Payload schreiben
  fwrite(MARKER_START, 1, strlen(MARKER_START), out_fp);
  fwrite(enc_buf, 1, enc_len, out_fp);
  fwrite(MARKER_END, 1, strlen(MARKER_END), out_fp);

  // Aufräumen
  if (enc_buf)
    free(enc_buf);
  fclose(in_fp);
  fclose(out_fp);

  return 0;
}

#ifndef STEG_EMBED_H
#define STEG_EMBED_H

#include <stdbool.h>

/**
 * Betten eine Payload-Datei in eine Binärdatei ein, optional mit
 * Verschlüsselung.
 *
 * @param input_file        Eingabedatei (z. B. Binärdatei, Bild, Audio)
 * @param payload_file      Datei mit geheimer Information
 * @param output_file       Ergebnisdatei mit eingebetteter Payload
 * @param key               Schlüssel für XOR/AES-Verschlüsselung
 * @param use_aes           true = AES, false = XOR
 * @return                  0 bei Erfolg, 1 bei Fehler
 */
int embed_payload(const char *input_file, const char *payload_file,
                  const char *output_file, const char *key, bool use_aes);

#endif

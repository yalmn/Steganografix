#ifndef STEG_EXTRACT_H
#define STEG_EXTRACT_H

#include <stdbool.h>

/**
 * Extrahiert eine eingebettete Payload aus einer Datei und entschlüsselt sie.
 *
 * @param input_file             Datei mit eingebetteter Information
 * @param output_payload_file    Zielpfad für extrahierte, entschlüsselte
 * Payload
 * @param key                    Schlüssel für XOR- oder AES-Entschlüsselung
 * @param use_aes                true = AES, false = XOR
 * @return                       0 bei Erfolg, 1 bei Fehler
 */
int extract_payload(const char *input_file, const char *output_payload_file,
                    const char *key, bool use_aes);

#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/steg_embed.h"
#include "../include/steg_extract.h"
#include "../include/lsb_steg.h"
#include "../include/log.h"

void print_usage(const char *progname) {
    printf("\n\033[1;36mSteganografix - CLI-Tool für Steganografie mit optionaler Verschlüsselung\033[0m\n");
    printf("Version: 1.0  |  Author: yalmn\n\n");

    printf("\033[1mVerwendung:\033[0m\n");
    printf("  %s embed <input_file> <payload_file> <output_file> [--key=yourkey] [--aes|--xor]\n", progname);
    printf("  %s extract <input_file> <output_payload_file> [--key=yourkey] [--aes|--xor]\n", progname);
    printf("  %s embed-lsb <image> <payload_file> <output_image> [--key=yourkey] [--aes|--xor]\n", progname);
    printf("  %s extract-lsb <image> <output_payload_file> [--key=yourkey] [--aes|--xor]\n", progname);
    printf("  %s --help   (zeigt diese Hilfe)\n\n", progname);

    printf("\033[1mOptionen:\033[0m\n");
    printf("  --key=...    Verschlüsselungsschlüssel (Pflicht für embed & extract)\n");
    printf("  --aes        AES-128-Verschlüsselung (CBC)\n");
    printf("  --xor        einfache XOR-Verschlüsselung (Default)\n\n");

    printf("\033[1mBeispiele:\033[0m\n");
    printf("  %s embed image.png secret.txt out.png --key=geheim123 --aes\n", progname);
    printf("  %s extract out.png recovered.txt --key=geheim123 --aes\n", progname);
    printf("  %s embed-lsb photo.jpg secret.txt stego.jpg --key=geheim123 --aes\n", progname);
    printf("  %s extract-lsb stego.jpg recovered.txt --key=geheim123 --aes\n", progname);
    printf("  %s embed logo.wav code.c out.wav --key=abc --xor\n\n", progname);

    printf("Weitere Infos: https://github.com/yalmn/steganografix.git\n\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        log_error("No command provided.");
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "embed") == 0) {
        if (argc < 5) {
            log_error("Invalid arguments for 'embed'.");
            print_usage(argv[0]);
            return 1;
        }

        const char *input_file = argv[2];
        const char *payload_file = argv[3];
        const char *output_file = argv[4];
        const char *key = "MySecretKey123";
        bool use_aes = false;

        for (int i = 5; i < argc; i++) {
            if (strncmp(argv[i], "--key=", 6) == 0) {
                key = argv[i] + 6;
            } else if (strcmp(argv[i], "--aes") == 0) {
                use_aes = true;
            } else if (strcmp(argv[i], "--xor") == 0) {
                use_aes = false;
            }
        }

        log_info("Embedding payload...");
        if (embed_payload(input_file, payload_file, output_file, key, use_aes) != 0) {
            log_error("Embedding failed.");
            return 1;
        }

        log_success("Payload embedded successfully.");
        return 0;

    } else if (strcmp(argv[1], "embed-lsb") == 0) {
        if (argc < 5) {
            log_error("Invalid arguments for 'embed-lsb'.");
            print_usage(argv[0]);
            return 1;
        }

        const char *input_file = argv[2];
        const char *payload_file = argv[3];
        const char *output_file = argv[4];
        const char *key = "MySecretKey123";
        bool use_aes = false;

        for (int i = 5; i < argc; i++) {
            if (strncmp(argv[i], "--key=", 6) == 0) {
                key = argv[i] + 6;
            } else if (strcmp(argv[i], "--aes") == 0) {
                use_aes = true;
            } else if (strcmp(argv[i], "--xor") == 0) {
                use_aes = false;
            }
        }

        log_info("Embedding payload with LSB...");
        if (embed_payload_lsb(input_file, payload_file, output_file, key, use_aes) != 0) {
            log_error("Embedding failed.");
            return 1;
        }

        log_success("Payload embedded successfully.");
        return 0;

    } else if (strcmp(argv[1], "extract-lsb") == 0) {
        if (argc < 4) {
            log_error("Invalid arguments for 'extract-lsb'.");
            print_usage(argv[0]);
            return 1;
        }

        const char *input_file = argv[2];
        const char *output_payload_file = argv[3];
        const char *key = "MySecretKey123";
        bool use_aes = false;

        for (int i = 4; i < argc; i++) {
            if (strncmp(argv[i], "--key=", 6) == 0) {
                key = argv[i] + 6;
            } else if (strcmp(argv[i], "--aes") == 0) {
                use_aes = true;
            } else if (strcmp(argv[i], "--xor") == 0) {
                use_aes = false;
            }
        }

        log_info("Extracting payload with LSB...");
        if (extract_payload_lsb(input_file, output_payload_file, key, use_aes) != 0) {
            log_error("Extraction failed.");
            return 1;
        }

        log_success("Payload extracted successfully.");
        return 0;

    } else if (strcmp(argv[1], "extract") == 0) {
        if (argc < 4) {
            log_error("Invalid arguments for 'extract'.");
            print_usage(argv[0]);
            return 1;
        }

        const char *input_file = argv[2];
        const char *output_payload_file = argv[3];
        const char *key = "MySecretKey123";
        bool use_aes = false;

        for (int i = 4; i < argc; i++) {
            if (strncmp(argv[i], "--key=", 6) == 0) {
                key = argv[i] + 6;
            } else if (strcmp(argv[i], "--aes") == 0) {
                use_aes = true;
            } else if (strcmp(argv[i], "--xor") == 0) {
                use_aes = false;
            }
        }

        log_info("Extracting payload...");
        if (extract_payload(input_file, output_payload_file, key, use_aes) != 0) {
            log_error("Extraction failed.");
            return 1;
        }

        log_success("Payload extracted successfully.");
        return 0;

    } else {
        char msg[128];
        snprintf(msg, sizeof(msg), "Unknown command: '%s'", argv[1]);
        log_error(msg);
        print_usage(argv[0]);
        return 1;
    }
}


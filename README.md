<p align="center">
  <img src="docs/Steg_Logo_Marked.png" alt="Steganografix Logo Logo" height="300"/>
</p>

# 🕵️‍♂️ Steganografix

**Steganografix** ist ein leichtgewichtiges, modulares C-Tool zur **steganografischen Einbettung** und **Extraktion** von Daten in Binärdateien (z. B. `.bin`, `.bmp`, `.wav`, `.exe`), mit optionaler **Verschlüsselung** (XOR oder AES-128-CBC).

---

##  Features

-  **Einbettung** von Daten in beliebige Binärdateien (Anhängen mit Marker)
-  **Extraktion** inklusive Entschlüsselung der eingebetteten Payload
-  **Verschlüsselung:**  
  - XOR (leichtgewichtig, symmetrisch)  
  - AES-128-CBC (sicherer, über OpenSSL)
-  **Modularer Aufbau:** klare Trennung von Logik und CLI
-  **CLI mit Argumenten:** `--key`, `--aes`, `--xor`, `--help`
-  **Farbausgabe** für Logs (Erfolg, Info, Fehler)
-  **LSB-Steganografie für Bilder** (`.png`, `.jpg`, zufällige Pixelpositionen)

---

##  Build-Anleitung

###  Voraussetzungen

- GCC-kompatibler C-Compiler
- OpenSSL (`libcrypto`) installiert  
  macOS (Intel):
  ```bash
  brew install openssl
  ```

###  Kompilieren

```bash
make clean
make
```

>  Falls du auf ARM/M1 bist, passe im Makefile die Pfade zu OpenSSL an.

---

##  Verwendung

###  Einbetten

```bash
./steganografix embed <input_file> <payload_file> <output_file> [--key=yourkey] [--aes|--xor]
./steganografix embed-lsb <image.png|image.jpg> <payload_file> <output_image> [--key=yourkey] [--aes|--xor]
```

###  Extrahieren

```bash
./steganografix extract <input_file> <output_payload_file> [--key=yourkey] [--aes|--xor]
./steganografix extract-lsb <image.png|image.jpg> <output_payload_file> [--key=yourkey] [--aes|--xor]
```

###  Hilfe anzeigen

```bash
./steganografix --help
```

---

##  Optionen

| Option        | Beschreibung                                           |
|---------------|--------------------------------------------------------|
| `--key=...`   | Schlüssel für Verschlüsselung/Entschlüsselung (Pflicht) |
| `--aes`       | AES-128-Verschlüsselung (OpenSSL, sicherer)           |
| `--xor`       | einfache XOR-Verschlüsselung (Default)                |

---

##  Beispiele

###  XOR-Modus

```bash
./steganografix embed input.bin secret.txt output.bin --key=abc123 --xor
./steganografix extract output.bin extracted.txt --key=abc123 --xor
```

###  AES-Modus

```bash
./steganografix embed audio.wav config.json out.wav --key=SuperSecure --aes
./steganografix extract out.wav recovered.json --key=SuperSecure --aes
```

###  LSB-Bilder

```bash
./steganografix embed-lsb pic.png secret.txt stego.png --key=secret --aes
./steganografix extract-lsb stego.png recovered.txt --key=secret --aes
```

---

##  Use Cases

| Szenario                         | Beschreibung                                                             |
|----------------------------------|---------------------------------------------------------------------------|
|  **Versteckter Transport**     | Sensible Konfigurationsdaten unauffällig in Audio-/Binärdateien einbetten |
|  **Datenübertragung im Feld** | Integrierte Payloads in Firmware-/Logdateien versenden                    |
|  **Sicherheitsforschung**      | Steganografie- und Kryptografie-Analysen testen                           |
|  **Forensik & OSINT**         | Versteckte Daten extrahieren und entschlüsseln                            |

---

##  Projektstruktur

```bash
Steganografix/
├── src/                   # C-Quellcode (main, embed, extract, crypto, log)
├── include/               # Header-Dateien
├── build/                 # Objektdateien (automatisch)
├── testfiles/             # Beispieltestdaten
├── Makefile               # Build-System
└── README.md              # Dieses Dokument
```

---

##  Status & Erweiterungen

| Feature                  | Status     |
|--------------------------|------------|
| Einbettung mit Marker    |  Fertig  |
| XOR- & AES-Verschlüsselung|  Fertig  |
| CLI mit `--key`, `--aes` |  Fertig  |
| `--help` Funktion        |  Fertig  |
| Fortschrittsanzeige      |  Optional |
| LSB-Steganografie        |  Fertig  |
| GUI (Qt, GTK)            |  Möglich  |

---

##  Mitmachen

Pull Requests, Issues, Forks – alles willkommen!

> Bei Interesse an AES-256, Passwort-Derivation, LSB-Embedding oder forensischen Analysen: [Melde dich!](mailto:halil@yalman.io)

---

##  Inspiration

> "Steganografie ist die Kunst, dort zu verstecken, wo niemand hinschaut."  
> — (angelehnt an Bruce Schneier)


# Compiler & Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I/usr/local/opt/openssl/include
LDFLAGS = -L/usr/local/opt/openssl/lib -lcrypto

# Verzeichnisse
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# Quelldateien & Objektdateien
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Zielname
TARGET = steganografix

# Default-Target
all: $(TARGET)

# Erstelle build-Verzeichnis falls nötig
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Baue Objektdateien
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Linken zum finalen Binärprogramm
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Aufräumen
clean:
	rm -rf $(OBJ_DIR) $(TARGET)



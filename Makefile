# Compiler und Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = 

# Verzeichnisse
SRC_DIR = src
INC_DIR = include
OBJ_DIR = build

# Quell- und Objektdateien
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Zielprogramm
TARGET = steganografix

# Default-Target
all: $(TARGET)

# Erzeuge Build-Verzeichnis falls nötig
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Kompilieren der Objektdateien
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Linken
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Clean-Target
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

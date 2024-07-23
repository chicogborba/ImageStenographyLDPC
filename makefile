# Nome do executável
TARGET = bin/program

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INC_DIR = include

# Arquivos fonte
SRC = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto (converte .c para .o)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Arquivos de cabeçalho
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Regra principal para compilar o executável
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Regra para compilar os arquivos .c em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.o

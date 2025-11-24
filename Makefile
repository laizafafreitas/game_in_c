# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/lib

# project name
PROJ_NAME = Logic_fighters

# Target directories
BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
SRC_DIR     = src
INCLUDE_DIR = include

# Source files (na raiz de src/)
SRC_FILES = $(notdir $(wildcard $(SRC_DIR)/*.c))
# Source files dentro de src/lib/
LIB_SRC_FILES = $(notdir $(wildcard $(SRC_DIR)/lib/*.c))

# Object files (todos vão pra build/obj/)
OBJ_FILES = \
	$(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o)) \
	$(addprefix $(OBJ_DIR)/, $(LIB_SRC_FILES:.c=.o))

# ======================
# Alvo principal
# ======================
all: $(BUILD_DIR)/$(PROJ_NAME)

$(BUILD_DIR)/$(PROJ_NAME): $(OBJ_DIR) $(OBJ_FILES)
	@echo "Linkando $(BUILD_DIR)/$(PROJ_NAME)..."
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(PROJ_NAME) $(OBJ_FILES)

# Garante que as pastas existem
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# ======================
# Regras de compilação
# ======================

# Arquivos .c em src/ -> .o em build/obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compilando $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Arquivos .c em src/lib/ -> .o em build/obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/lib/%.c | $(OBJ_DIR)
	@echo "Compilando $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# ======================
# Auxiliares
# ======================

clean:
	rm -rf $(BUILD_DIR)

# Você pediu "make clear"
clear: clean

run: all
	./$(BUILD_DIR)/$(PROJ_NAME)

.PHONY: all clean clear run

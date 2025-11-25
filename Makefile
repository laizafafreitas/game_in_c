# Compiler and flags
CC = gcc
INCLUDE_DIR = include
CFLAGS = -Wall -Werror -g \
         -I$(INCLUDE_DIR) \
         -I$(INCLUDE_DIR)/lib \
         -I$(INCLUDE_DIR)/screens

# project name
PROJ_NAME = Logic_fighters

# Target directories
BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
SRC_DIR     = src
LIB_SRC_DIR = $(SRC_DIR)/lib
SCREEN_DIR  = $(SRC_DIR)/screens

# Source files
SRC_FILES        = $(wildcard $(SRC_DIR)/*.c)
LIB_SRC_FILES    = $(wildcard $(LIB_SRC_DIR)/*.c)
SCREEN_SRC_FILES = $(wildcard $(SCREEN_DIR)/*.c)

# Object files (todos vão pra build/obj/)
OBJ_FILES = \
	$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
	$(patsubst $(LIB_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIB_SRC_FILES)) \
	$(patsubst $(SCREEN_DIR)/%.c,$(OBJ_DIR)/%.o,$(SCREEN_SRC_FILES))

# Onde procurar .c quando a regra genérica pedir %.c
vpath %.c $(SRC_DIR) $(LIB_SRC_DIR) $(SCREEN_DIR)

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

# Regra genérica para compilar QUALQUER %.c encontrado via vpath em %.o
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
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

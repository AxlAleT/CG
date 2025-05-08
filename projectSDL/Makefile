# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR) -lm `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lm

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
RES_DIR = resources

# Files
EXEC = $(BIN_DIR)/main
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Rules
all: directories $(EXEC)

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(RES_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories

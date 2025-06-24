CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lSDL2 -lm

# Debug flags
DEBUG_CFLAGS = -Wall -Wextra -g -std=c99 -DDEBUG

# Source files
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_DIR = obj
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Header files
INCLUDE_DIR = include
INCLUDES = -I$(INCLUDE_DIR)

# Output executable
TARGET = raytracer

# Refactored version (no SDL dependency)
REFACTOR_SRC = $(SRC_DIR)/utils.c $(SRC_DIR)/quadrics.c $(SRC_DIR)/main.c
REFACTOR_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(REFACTOR_SRC))
REFACTOR_TARGET = raytracer_refactored
REFACTOR_LDFLAGS = -lm

# Default target
all: $(TARGET) $(REFACTOR_TARGET)

# Debug target
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(TARGET)

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Refactored version (no SDL)
$(REFACTOR_TARGET): $(REFACTOR_OBJ)
	$(CC) $^ -o $@ $(REFACTOR_LDFLAGS)

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(REFACTOR_TARGET) raytracer_output.ppm

# Phony targets
.PHONY: all clean debug refactored run_refactored

# Run target for easier execution with SDL debug info
run: $(TARGET)
	SDL_DEBUG=1 ./$(TARGET)

# Build only the refactored version
refactored: $(REFACTOR_TARGET)

# Run the refactored version
run_refactored: $(REFACTOR_TARGET)
	./$(REFACTOR_TARGET)

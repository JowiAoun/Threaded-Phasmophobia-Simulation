# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -g -pthread

# Directories
SRC_DIR := src
OBJ_DIR := out

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable name
TARGET := final.exe

# Main target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to create the objects directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony target to avoid conflicts with files of the same name
.PHONY: all clean

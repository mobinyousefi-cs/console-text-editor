# Project: Console-Based Text Editor
# Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
# License: MIT License (see LICENSE file for details)

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -pedantic -Iinclude
LDFLAGS :=

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
TEST_DIR := tests

SOURCES := $(SRC_DIR)/main.c \
           $(SRC_DIR)/editor.c \
           $(SRC_DIR)/buffer.c \
           $(SRC_DIR)/fileio.c \
           $(SRC_DIR)/util.c

OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET  := $(BIN_DIR)/text_editor
TEST_BIN := $(BIN_DIR)/test_buffer

.PHONY: all clean test dirs

all: dirs $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): dirs $(TEST_DIR)/test_buffer.c $(SRC_DIR)/buffer.c
	$(CC) $(CFLAGS) -o $@ $(TEST_DIR)/test_buffer.c $(SRC_DIR)/buffer.c

 test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

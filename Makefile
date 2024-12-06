# The day you want to build. Defaults to "day_1".
DAY ?= day_1

# Directories
SRC_DIR := src
BIN_DIR := bin

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -O2 -g

# Source and target definitions
SRC_FILE := $(SRC_DIR)/$(DAY)/main.c
BIN_FILE := $(BIN_DIR)/$(DAY)

# The default target
all: $(BIN_FILE)

$(BIN_FILE): $(SRC_FILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/

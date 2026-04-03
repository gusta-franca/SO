SRC_DIR := src
BUILD_DIR := build

CC := gcc
CFLAGS := -Wall -Wextra -g

SOURCES := $(wildcard $(SRC_DIR)/*.c)

BINARIES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))

all: $(BUILD_DIR) $(BINARIES)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean

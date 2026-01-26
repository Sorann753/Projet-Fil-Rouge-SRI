# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -lm
INCLUDES = -Iinclude

# Sources
SRC_TEST = $(shell find source -name '*.c' ! -name 'main.c')  # tout sauf main.c

# Binaire
BIN_DIR = build/linux/x86_64/release
BIN_TEST = $(BIN_DIR)/PFR-test

# Compile
.PHONY: PFR-test
PFR-test:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_TEST) $(SRC_TEST) -lm

# Run
.PHONY: run-test
run-test: PFR-test
	cd $(BIN_DIR) && ./PFR-test

# Clean
.PHONY: clean
clean:
	rm -rf build/linux

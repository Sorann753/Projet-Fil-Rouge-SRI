# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -lm
INCLUDES = -Iinclude

# Sources
SRC_TEST = $(shell find source -name '*.c' ! -name 'main.c' ! -name 'unitTest.c')  # tout sauf main.c
SRC_UNIT = $(shell find source -name '*.c' ! -name 'main.c' ! -name 'test.c')  # tout sauf main.c

# Binaire
BIN_DIR = build/linux/x86_64/release
BIN_TEST = $(BIN_DIR)/PFR-test
BIN_UNIT = $(BIN_DIR)/PFR-unit

# Compile
.PHONY: PFR-test
PFR-test:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_TEST) $(SRC_TEST) -lm

# Run
.PHONY: run-test
run-test: PFR-test
	cd $(BIN_DIR) && ./PFR-test

.PHONY: PFR-unit
PFR-unit:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BIN_DIR)/export
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_UNIT) $(SRC_UNIT) -lm

# Run  with vision module
.PHONY: run-unit
run-unit: PFR-unit
	cd $(BIN_DIR) && ./PFR-unit

# Clean
.PHONY: clean
clean:
	rm -rf build/linux


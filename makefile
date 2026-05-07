# --- CONFIGURATION ---
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -D_DEFAULT_SOURCE
INCLUDES = -Iinclude
LIBS = -lm

# Dossiers
BIN_DIR = build/linux/x86_64/release
# On s'assure que le chemin est absolu pour éviter les surprises
ABS_BIN_DIR = $(shell pwd)/$(BIN_DIR)

# --- SOURCES ---
# Robot principal (exclut les fichiers de test)
SRC_ROBOT = $(shell find source -name '*.c' ! -name 'test.c' ! -name 'unitTest.c' ! -name 'arduinoTest.c')

# Tests Arduino (inclut arduinoTest.c mais exclut main.c)
SRC_TEST = $(shell find source -name '*.c' ! -name 'main.c' ! -name 'unitTest.c')

# Vision / Unit Tests
SRC_VISION = $(shell find source -name '*.c' ! -name 'main.c' ! -name 'test.c')

# Binaires
BIN_ROBOT = $(BIN_DIR)/PFR-robot
BIN_TEST  = $(BIN_DIR)/PFR-test
BIN_UNIT  = $(BIN_DIR)/PFR-unit

# --- CIBLES ---

.PHONY: all clean run run-test run-unit

all: PFR-robot

# Compilation du Robot
PFR-robot:
	@mkdir -p "$(BIN_DIR)"
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_ROBOT) $(SRC_ROBOT) $(LIBS)

# Compilation des Tests Arduino
PFR-test:
	@mkdir -p "$(BIN_DIR)"
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_TEST) $(SRC_TEST) $(LIBS)

# Compilation Vision
PFR-unit:
	@mkdir -p "$(BIN_DIR)"
	@mkdir -p "$(BIN_DIR)/export"
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_UNIT) $(SRC_VISION) $(LIBS)

# --- EXECUTION ---
run: PFR-robot
	cd $(BIN_DIR) && ./PFR-robot

run-test: PFR-test
	cd $(BIN_DIR) && ./PFR-test

run-unit: PFR-unit
	cd $(BIN_DIR) && ./PFR-unit

# Nettoyage
clean:
	rm -rf build/linux
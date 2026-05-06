# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -D_DEFAULT_SOURCE
INCLUDES = -Iinclude
LIBS = -lm

# Dossier binaire
BIN_DIR = build/linux/x86_64/release

# --- FILTRAGE DES SOURCES ---

# Pour le robot (Pilotage Manuel + Main)
SRC_ROBOT = $(shell find source -name '*.c' ! -name 'test.c' ! -name 'unitTest.c' ! -name 'arduinoTest.c')

# Pour les tests Arduino (ton ancien test)
SRC_TEST = $(shell find source -name '*.c' ! -name 'main.c' ! -name 'unitTest.c')

# Pour le module Vision (Unit Test)
SRC_VISION = $(shell find source -name '*.c' ! -name 'main.c' ! -name 'test.c')

# Binaires
BIN_ROBOT = $(BIN_DIR)/PFR-robot
BIN_TEST  = $(BIN_DIR)/PFR-test
BIN_UNIT  = $(BIN_DIR)/PFR-unit

# --- CIBLES ---

.PHONY: all clean

all: PFR-robot

# Compilation et Run du programme principal (Pilotage manuel)
.PHONY: PFR-robot run
PFR-robot:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_ROBOT) $(SRC_ROBOT) $(LIBS)

run: PFR-robot
	./$(BIN_ROBOT)

# Compilation et Run des tests Arduino
.PHONY: PFR-test run-test
PFR-test:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_TEST) $(SRC_TEST) $(LIBS)

run-test: PFR-test
	./$(BIN_TEST)

# Compilation et Run du module Vision (Unit Test)
.PHONY: PFR-unit run-unit
PFR-unit:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BIN_DIR)/export
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_UNIT) $(SRC_VISION) $(LIBS)

run-unit: PFR-unit
	cd $(BIN_DIR) && ./PFR-unit

# Nettoyage
clean:
	rm -rf build/linux
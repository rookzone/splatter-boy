# === Project Settings ===
PROJECT_NAME = splatter-boy
SRC_DIR = src
BUILD_DIR = build

# === Compiler Settings ===
# Automatically detect GBDK compiler location
ifeq ($(OS),Windows_NT)
    CC = bin/lcc.exe
    MKDIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /s /q
else
    CC = lcc
    MKDIR = mkdir -p $(1)
    RM = rm -rf
endif

# === Source Files ===

SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/tiles/*.c) \
      $(wildcard $(SRC_DIR)/maps/*.c) \
      $(wildcard $(SRC_DIR)/scenes/*.c)

# This now includes the root src/ folder headers explicitly
HEADERS = $(wildcard $(SRC_DIR)/*.h) \
          $(wildcard $(SRC_DIR)/tiles/*.h) \
          $(wildcard $(SRC_DIR)/maps/*.h) \
          $(wildcard $(SRC_DIR)/scenes/*.h) \
          $(wildcard $(SRC_DIR)/platform/gb/*.h)

# === Output ===
OUT = $(BUILD_DIR)/$(PROJECT_NAME).gb

# === Compiler Flags ===
# Note: Removed '-Iinclude' and added '-Isrc' so the compiler 
# can find headers referenced as #include "ball.h"
CFLAGS = -Wa-l -Wl-m -Wl-j

# === Build Rule ===
# Now the .gb file depends on both .c and .h files
$(OUT): $(SRC) $(HEADERS)
	@$(call MKDIR,$(BUILD_DIR))
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# === Clean Rule ===
clean:
	@echo Cleaning build files...
	@$(RM) $(BUILD_DIR)
	@echo Done.

# === Rebuild Rule ===
rebuild: clean all

.PHONY: all clean rebuild
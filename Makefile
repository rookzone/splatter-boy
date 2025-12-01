# === Project Settings ===
PROJECT_NAME = splatter-boy
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# === Compiler Settings ===
CC = ./bin/lcc.exe

# === Source Files ===
SRC = $(wildcard $(SRC_DIR)/*.c) \
	$(wildcard $(SRC_DIR)/tiles/*.c)

# === Output ===
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
OUT = $(BUILD_DIR)/$(PROJECT_NAME).gb

# === Compiler Flags ===
CFLAGS = -Wa-l -Wl-m -Wl-j -Iinclude

# === Default Rule ===
all: $(OUT)
	@echo "Build complete: $(OUT)"

# NEW LINK RULE:
# This .PHONY rule forces the linker to run every time, fixing the header dependency problem.
.PHONY: $(OUT)
$(OUT): $(SRC) $(wildCARD include/*.h) # Added dependency check to be safer, though .PHONY is the main fix
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# === Clean rule ===
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Done."

# === Rebuild rule ===
rebuild: clean all
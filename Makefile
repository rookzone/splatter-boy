# === Project Settings ===
PROJECT_NAME := splatter-boy
SRC_DIR := src
BUILD_DIR := build
OUT := $(BUILD_DIR)/$(PROJECT_NAME).gb

# === Source Files ===
SRC := $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/*/*.c)

HEADERS := $(wildcard $(SRC_DIR)/*.h) \
           $(wildcard $(SRC_DIR)/*/*.h)

# === Tool Overrides ===
CFLAGS ?= -std-c11 -Wf--fverbose-asm
RUN_ARGS ?=
GBDK_HOME ?=
EMU_HOME ?=

ifeq ($(OS),Windows_NT)
    GBDK_LCC_FROM_HOME := $(if $(GBDK_HOME),$(firstword $(wildcard $(GBDK_HOME)/bin/lcc.exe)))
    EMULATOR_FROM_HOME := $(if $(EMU_HOME),$(firstword $(wildcard $(EMU_HOME)/mGBA.exe) $(wildcard $(EMU_HOME)/mgba-sdl.exe)))
    LOCAL_GBDK_LCC := $(firstword $(wildcard .gbdk/bin/lcc.exe) $(wildcard gbdk/bin/lcc.exe) $(wildcard bin/lcc.exe) $(GBDK_LCC_FROM_HOME))
    LOCAL_EMULATOR := $(firstword $(wildcard emu/mgba/mGBA.exe) $(wildcard emu/mgba/mgba-sdl.exe) $(EMULATOR_FROM_HOME) $(wildcard emu/BGB/bgb.exe))
    PATH_GBDK_LCC := $(strip $(firstword $(shell where lcc 2>NUL)))
    PATH_EMULATOR := $(strip $(firstword $(shell where mGBA.exe 2>NUL) $(shell where mgba-sdl.exe 2>NUL)))
    MKDIR_BUILD = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM_BUILD = if exist "$(subst /,\,$(1))" rmdir /s /q "$(subst /,\,$(1))"
else
    GBDK_LCC_FROM_HOME := $(if $(GBDK_HOME),$(firstword $(wildcard $(GBDK_HOME)/bin/lcc)))
    EMULATOR_FROM_HOME := $(if $(EMU_HOME),$(firstword $(wildcard $(EMU_HOME)/mGBA.AppImage) $(wildcard $(EMU_HOME)/*.AppImage) $(wildcard $(EMU_HOME)/mgba-qt) $(wildcard $(EMU_HOME)/mgba-sdl) $(wildcard $(EMU_HOME)/mgba)))
    LOCAL_GBDK_LCC := $(firstword $(wildcard .gbdk/bin/lcc) $(wildcard gbdk/bin/lcc) $(wildcard bin/lcc) $(GBDK_LCC_FROM_HOME) $(wildcard /opt/gbdk/bin/lcc))
    LOCAL_EMULATOR := $(firstword $(wildcard emu/mgba/mgba) $(wildcard emu/mgba/mgba-qt) $(wildcard emu/mgba/mgba-sdl) $(wildcard emu/mgba/mGBA.AppImage) $(wildcard emu/mgba/*.AppImage) $(wildcard emu/mgba/*.appimage) $(EMULATOR_FROM_HOME))
    PATH_GBDK_LCC := $(strip $(shell command -v lcc 2>/dev/null))
    PATH_EMULATOR := $(strip $(shell command -v mgba-qt 2>/dev/null || command -v mgba-sdl 2>/dev/null || command -v mgba 2>/dev/null))
    MKDIR_BUILD = mkdir -p $(1)
    RM_BUILD = rm -rf $(1)
endif

AUTO_GBDK_LCC := $(or $(LOCAL_GBDK_LCC),$(PATH_GBDK_LCC))
AUTO_EMULATOR := $(or $(LOCAL_EMULATOR),$(PATH_EMULATOR))

GBDK_LCC ?= $(or $(AUTO_GBDK_LCC),lcc)
GB_EMULATOR ?= $(AUTO_EMULATOR)

define require-compiler
$(if $(strip $(AUTO_GBDK_LCC)$(filter-out lcc,$(GBDK_LCC))),,$(error Could not find the GBDK compiler. Install GBDK and add 'lcc' to PATH, place it in ./.gbdk or ./bin, run 'make GBDK_HOME=/path/to/gbdk' / 'make GBDK_LCC=/path/to/lcc', or on Linux run './scripts/setup-linux.sh'))
endef

define require-emulator
$(if $(strip $(AUTO_EMULATOR)$(GB_EMULATOR)),,$(error Could not find a Game Boy emulator. Install mGBA and add it to PATH, place it in ./emu/mgba, run 'make run GB_EMULATOR=/path/to/emulator', or on Linux run './scripts/setup-linux.sh'))
endef

# === Targets ===
all: $(OUT)

$(OUT): $(SRC) $(HEADERS)
	@$(call require-compiler)
	@$(call MKDIR_BUILD,$(BUILD_DIR))
	"$(GBDK_LCC)" $(CFLAGS) -o "$(OUT)" $(SRC)

run: $(OUT)
	@$(call require-emulator)
	"$(GB_EMULATOR)" $(RUN_ARGS) "$(OUT)"

doctor:
	@echo Project: $(PROJECT_NAME)
	@echo ROM output: $(OUT)
	@echo GBDK compiler: $(if $(strip $(AUTO_GBDK_LCC)$(filter-out lcc,$(GBDK_LCC))),$(GBDK_LCC),not found)
	@echo Emulator: $(if $(strip $(AUTO_EMULATOR)$(GB_EMULATOR)),$(GB_EMULATOR),not found)

clean:
	@echo Cleaning build files...
	@$(call RM_BUILD,$(BUILD_DIR))
	@echo Done.

rebuild: clean all

.PHONY: all run doctor clean rebuild

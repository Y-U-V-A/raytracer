#    ███    ███  █████  ██   ██ ███████ ███████ ██ ██      ███████ 
#    ████  ████ ██   ██ ██  ██  ██      ██      ██ ██      ██      
#    ██ ████ ██ ███████ █████   █████   █████   ██ ██      █████   
#    ██  ██  ██ ██   ██ ██  ██  ██      ██      ██ ██      ██      
#    ██      ██ ██   ██ ██   ██ ███████ ██      ██ ███████ ███████ 
#                                                                  
# 

CONFIG?=debug

ifeq ($(OS),Windows_NT)
	PLATFORM=windows
else ifeq ($(shell uname -s),Linux)
	PLATFORM=linux
else
	$(error platform not supported)
endif

ifeq ($(PLATFORM),windows)
	SHELL=cmd.exe
	EXT=.exe
	MKDIR=if not exist "$(1)" mkdir "$(1)"
	RMDIR=if exist "$(1)" rmdir /Q /S "$(1)"
	RMFILE=if exist "$(1)" del /Q "$(1)"
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		ARCH=-m64
	else ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		ARCH=-m64
	else ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		ARCH=-m32
	else
		$(error machine arch not supported)
	endif
endif

ifeq ($(PLATFORM),linux)
	EXT=
	MKDIR=mkdir -p "$(1)"
	RMDIR=rm -rf "$(1)"
	RMFILE=rm -f "$(1)"
	MACHINE_ARCH=$(shell uname -m)
	ifeq ($(MACHINE_ARCH),x86_64)
			ARCH=-m64
	else ifeq ($(MACHINE_ARCH),i386)
		ARCH=-m32
	else
		$(error machine arch not supported)
	endif
endif

GET_FILES=$(sort \
				$(foreach DIR,$(1),\
					$(foreach ITEM,$(wildcard $(DIR)/*),\
						$(if $(wildcard $(ITEM)/*),\
							$(call GET_FILES,$(ITEM),$(2)),\
							$(if $(filter $(2),$(ITEM)),$(ITEM))\
						)\
					)\
				)\
			)

BIN=build/bin
BIN_INT=build/bin_int/$(PLATFORM)/$(CONFIG)

SRC?=main
SOURCE=source/core source/$(SRC)

CFILES=$(call GET_FILES,$(SOURCE),%.c)
OFILES=$(patsubst %.c,$(BIN_INT)/%.o,$(CFILES))
DFILES=$(patsubst %.c,$(BIN_INT)/%.d,$(CFILES))
HFILES=$(call GET_FILES,$(SOURCE),%.h)

TARGET=$(BIN)/raytracer$(EXT)

ifeq ($(CONFIG),debug)
	CONFIG_FLAGS=-O0 -g -DDEBUG -fsanitize=address
else ifeq ($(CONFIG),release)
	CONFIG_FLAGS=-O3 -march=native -ffast-math -DNDEBUG
else
	$(error unknown config : $(CONFIG))
endif

CC=clang
CVERSION=-std=c99
CFLAGS=-c -Wall -Wextra -Werror -MMD -MP -Wno-unused-parameter $(sort $(foreach FILE,$(HFILES),-I$(dir $(FILE)))) $(ARCH) $(CVERSION) $(CONFIG_FLAGS)
LFLAGS=$(ARCH) $(CVERSION) $(CONFIG_FLAGS)

.PHONY:info run clean build_dirs

run:build_dirs $(TARGET)
	@echo "running..."
	@$(TARGET)

build_dirs:
	@$(call MKDIR,$(BIN))
	@$(call MKDIR,$(BIN_INT))

$(TARGET):$(OFILES)
	@echo "linking..."
	@$(CC) $(LFLAGS) $(OFILES) -o $(TARGET)

$(BIN_INT)/%.o:%.c
	@$(call MKDIR,$(dir $@))
	@$(CC) $(CFLAGS) $< -o $@
	@echo "compiled $(basename $(notdir $<))"

-include $(DFILES)

clean:
	@$(call RMDIR,build)
	@$(call RMDIR,.cache)
	@$(call RMDIR,.vscode)
	@$(call RMFILE,compile_commands.json)

info:
	@echo "SOURCE=$(SOURCE)"
	@echo "CFILES=$(CFILES)"
	@echo "OFILES=$(OFILES)"
	@echo "DFILES=$(DFILES)"
	@echo "HFILES=$(HFILES)"
	@echo "PLATFORM=$(PLATFORM)"
	@echo "CONFIG=$(CONFIG)"
	@echo "BIN=$(BIN)"
	@echo "BIN_INT=$(BIN_INT)"
	@echo "CFLAGS=$(CFLAGS)"
	@echo "LFLAGS=$(LFLAGS)"
	@echo "TARGET=$(TARGET)"
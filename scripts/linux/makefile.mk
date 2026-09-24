# =====================================================================
#  Cross-platform build for img(.exe)
#  Auto-detects Windows vs Linux and switches flags/libs accordingly.
# =====================================================================

CXX := g++

# --- Detect platform ---
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
else
    PLATFORM := Linux
endif

# --- Directories (same on both platforms) ---
SRC_DIR    := src
SHADER_DIR := shaders
OBJ_DIR    := obj
BIN_DIR    := bin
ENGINE_OBJ := Basic-OpenGL/obj

# --- Flags common to both platforms ---
CXXFLAGS := -std=c++17 -Wall -Wextra -I. -IBasic-OpenGL -IBasic-OpenGL/src -DSDL_MAIN_HANDLED

ifeq ($(PLATFORM),Windows)
# ------------------------------------------------------------------
# Windows / MinGW-w64 - unchanged from the original Win32 Makefile
# ------------------------------------------------------------------
TARGET   := $(BIN_DIR)/img.exe

CXXFLAGS += -IBasic-OpenGL/ThirdParty/freetype-2.14.3/include -D_WIN32_WINNT=0x0601

LDFLAGS  := -LBasic-OpenGL/ThirdParty/SDL2/lib              \
            -LBasic-OpenGL/ThirdParty/freetype-2.14.3/objs  \
            -lfreetype                                      \
            -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

else
# ------------------------------------------------------------------
# Linux - use system dev packages via pkg-config instead of the
# vendored Windows-only SDL2/freetype binaries under ThirdParty/
# ------------------------------------------------------------------
TARGET   := $(BIN_DIR)/img

CXXFLAGS += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf freetype2)

# -ldl is for GLAD's runtime loader (dlopen/dlsym); harmless if unused.
LDFLAGS  := $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf freetype2) -ldl

endif

# --- Source and Object Files (identical logic on both platforms) ---
SRCS := $(wildcard *.cpp) $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SHADER_DIR)/*.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

ALL_ENGINE_OBJS := $(wildcard $(ENGINE_OBJ)/src/*.o) \
$(wildcard $(ENGINE_OBJ)/shaders/*.o) \
$(wildcard $(ENGINE_OBJ)/ThirdParty/GLAD/src/*.o)

ENGINE_OBJS := $(filter-out $(ENGINE_OBJ)/src/main.o, $(ALL_ENGINE_OBJS))

# --- Build Rules ---
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) $(ENGINE_OBJS) -o $(TARGET) $(LDFLAGS)
ifeq ($(PLATFORM),Windows)
	@echo "Copying engine DLLs to application binary folder..."
	@cp -f Basic-OpenGL/*.dll $(BIN_DIR)/ 2>/dev/null || copy /Y Basic-OpenGL\*.dll $(BIN_DIR) >nul 2>&1 || true
endif

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# SHADER RULE
$(OBJ_DIR)/%.o: $(SHADER_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
ifeq ($(PLATFORM),Windows)
	@if not exist "$(OBJ_DIR)" (mkdir "$(OBJ_DIR)" 2>nul || mkdir -p "$(OBJ_DIR)")
else
	@mkdir -p $(OBJ_DIR)
endif

$(BIN_DIR):
ifeq ($(PLATFORM),Windows)
	@if not exist "$(BIN_DIR)" (mkdir "$(BIN_DIR)" 2>nul || mkdir -p "$(BIN_DIR)")
else
	@mkdir -p $(BIN_DIR)
endif

clean:
	@echo "Cleaning up..."
ifeq ($(PLATFORM),Windows)
	@rm -rf $(OBJ_DIR) $(BIN_DIR) 2>/dev/null || (if exist "$(OBJ_DIR)" rd /s /q "$(OBJ_DIR)" & if exist "$(BIN_DIR)" rd /s /q "$(BIN_DIR)")
else
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
endif
# --- Compiler and Flags ---
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I. -IBasic-OpenGL -IBasic-OpenGL/src -DSDL_MAIN_HANDLED -D_WIN32_WINNT=0x0601

# --- UPDATED FLAGS ---
# -L points directly to your SDL2 lib folder containing the .a files
# We keep -mconsole so MinGW expects a standard console main()
LDFLAGS  := -LBasic-OpenGL/ThirdParty/SDL2/lib -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# --- Directories ---
SRC_DIR    := src
OBJ_DIR    := obj
BIN_DIR    := bin
ENGINE_OBJ := Basic-OpenGL/obj

# --- Target ---
TARGET := $(BIN_DIR)/keng.exe

# --- Source and Object Files ---
SRCS := $(wildcard *.cpp) $(wildcard $(SRC_DIR)/*.cpp)
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
	@echo "Copying engine DLLs to application binary folder..."
	@cp -f Basic-OpenGL/*.dll $(BIN_DIR)/ 2>/dev/null || copy /Y Basic-OpenGL\*.dll $(BIN_DIR) >nul 2>&1 || true

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@if not exist "$(OBJ_DIR)" (mkdir "$(OBJ_DIR)" 2>nul || mkdir -p "$(OBJ_DIR)")

$(BIN_DIR):
	@if not exist "$(BIN_DIR)" (mkdir "$(BIN_DIR)" 2>nul || mkdir -p "$(BIN_DIR)")

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR) 2>/dev/null || (if exist "$(OBJ_DIR)" rd /s /q "$(OBJ_DIR)" & if exist "$(BIN_DIR)" rd /s /q "$(BIN_DIR)")
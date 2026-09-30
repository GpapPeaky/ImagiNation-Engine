# =====================================================================
#  Linux build for img (all libraries from Basic-OpenGL/ThirdParty)
# =====================================================================

CXX := g++
CC  := gcc

SRC_DIR    := src
SHADER_DIR := shaders
OBJ_DIR    := obj
BIN_DIR    := bin
ENGINE_DIR := Basic-OpenGL
TP         := $(ENGINE_DIR)/ThirdParty
DEPS       := $(TP)/dependencies/linux

TARGET := $(BIN_DIR)/ine

# Each root contains include/ and lib/
SDL_ROOT  := $(DEPS)/linux_SDL2/SDL2
FT_ROOT   := $(DEPS)/linux_freetype/freetype-2.14.3
LIB_ROOTS := $(SDL_ROOT) $(FT_ROOT)

CPPFLAGS := -I. -I$(ENGINE_DIR) -I$(ENGINE_DIR)/src \
            -I$(TP) -I$(TP)/GLAD/include -I$(TP)/stb_image \
            $(foreach r,$(LIB_ROOTS),-I$(r)/include -I$(r)/include/SDL2 -I$(r)/include/freetype2) \
            -DSDL_MAIN_HANDLED -MMD -MP

CXXFLAGS := -std=c++17 -Wall -Wextra
CFLAGS   := -Wall -Wextra

LDFLAGS := $(foreach r,$(LIB_ROOTS),-L$(r)/lib) \
           $(foreach r,$(LIB_ROOTS),-Wl,-rpath,'$$ORIGIN/../$(r)/lib')

LDLIBS := -lSDL2 -lfreetype -ldl -lm -lpthread

#
# FUTURE NOTICE
#
#
# LDLIBS := -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2 -lfreetype -ldl -lm -lpthread

# --- Game sources ---
SRCS := $(wildcard *.cpp) $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SHADER_DIR)/*.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# --- Engine sources (built fresh for Linux, engine main.cpp excluded) ---
ENGINE_OBJ_DIR := $(OBJ_DIR)/engine
ENGINE_CPP := $(filter-out $(ENGINE_DIR)/src/main.cpp, \
              $(wildcard $(ENGINE_DIR)/src/*.cpp) $(wildcard $(ENGINE_DIR)/shaders/*.cpp))
ENGINE_OBJS := $(patsubst $(ENGINE_DIR)/%.cpp,$(ENGINE_OBJ_DIR)/%.o,$(ENGINE_CPP)) \
               $(ENGINE_OBJ_DIR)/ThirdParty/GLAD/src/glad.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) $(ENGINE_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $@..."
	$(CXX) $(OBJS) $(ENGINE_OBJS) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SHADER_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(ENGINE_OBJ_DIR)/%.o: $(ENGINE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling engine $<..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(ENGINE_OBJ_DIR)/%.o: $(ENGINE_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling engine $<..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

-include $(OBJS:.o=.d) $(ENGINE_OBJS:.o=.d)
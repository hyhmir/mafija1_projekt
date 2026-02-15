# Compiler
CXX = g++

# Target
TARGET = sim

# Folders
SRC_DIR = src
BUILD_DIR = build

# Files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Flags
CXXFLAGS = -std=c++17 -Wall -Wextra -Wshadow -O3 -march=native
DEBUGFLAGS = -g -O0 -fsanitize=address,undefined
LIBS = -lraylib -lGL -lm -ldl -lpthread -lrt

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS)

# Compile .cpp -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0 -fsanitize=address,undefined
debug: clean $(TARGET)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
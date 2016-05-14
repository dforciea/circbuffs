CXX = g++
CC  = gcc

CXXFLAGS = -Wall

INCLUDES = -I.

OBJ_DIR = obj

OBJ_FILES = $(CPP_FILES:%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all: dirs $(OBJ_FILES)

clean:
	rm -f $(OBJ_FILES)

dirs:
	mkdir -p $(OBJ_DIR)

.PHONY: dirs all

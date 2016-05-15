CXX = g++
CC  = gcc

CXXFLAGS = -Wall -fPIC

INCLUDES = -I.

OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

OBJ_FILES = $(CPP_FILES:%.cpp=$(OBJ_DIR)/%.o)

BIN_FILES = $(CPP_MAIN_FILES:%.cpp=$(BIN_DIR)/%)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(LIB_DIR)/$(LIB_NAME) : $(OBJ_FILES)
	$(CXX) -shared $(CXXFLAGS) $(LIBS) -o $@ $?

libs: dirs $(LIB_DIR)/$(LIB_NAME)

all: dirs $(OBJ_FILES) libs

clean:
	rm -f $(OBJ_FILES)
	rm -f $(LIB_DIR)/$(LIB_NAME)
	rm -f $(BIN_FILES)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)

.PHONY: dirs all

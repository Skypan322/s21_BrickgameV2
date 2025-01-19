CC = gcc
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
CFLAGS = -std=c11 -Wall -Wextra
SRC_BACKEND = $(shell find brick_game/tetris -name '*.c')
SRC_BACKEND += $(shell find brick_game/common -name '*.c')
OBJ_BACKEND = $(SRC_BACKEND:%.c=$(BUILD_DIR)/%.o)
SRC_FRONTEND = $(shell find gui/cli -name '*.c')
HEADERS = $(shell find . -name '*.h')
SRC = main_tetris_cli.c $(SRC_BACKEND) $(SRC_FRONTEND)

SRC_SNAKE_BACKEND = $(shell find brick_game/snake -name '*.cpp')
SRC_SNAKE_BACKEND += $(shell find brick_game/common -name '*.c')
OBJ_SNAKE_BACKEND = $(SRC_SNAKE_BACKEND:%.cpp=$(BUILD_DIR)/%.o)
OBJ_SNAKE_BACKEND += $(SRC_SNAKE_BACKEND:%.c=$(BUILD_DIR)/%.o)

SRC_SNAKE = main_snake_cli.cpp $(SRC_SNAKE_BACKEND) $(SRC_FRONTEND)
TESTS_SRC = $(shell find tests -name '*.cpp')
TESTS_LIBS = -lgtest

ifeq ($(OS), Darwin)
    OPEN_CMD = xdg-open
else ifeq ($(shell uname -r | grep -i microsoft),)
	alias chrome="/mnt/c/Program\ Files/Google/Chrome/Application/chrome.exe"
	OPEN_CMD = chrome
else
	OPEN_CMD = xdg-open
endif

BUILD_DIR = build
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:.o=.d)
STYLE_CONFIG = ../materials/linters/.clang-format
DESTDIR = .
BIN = $(DESTDIR)/bin
# OS-specific library flags
ifeq ($(OS), Darwin)
    LIBS = -lm -lncurses
else
    LIBS = -lm -lncursesw -pthread -lrt 
endif

# Default target
all: desktop tetris_cli snake_cli
rebuild: clean_all all

# Create the directories for the build outputs
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

desktop:
	@echo "Building desktop..."
	mkdir -p gui/desktop/build
	@cd gui/desktop/build && cmake .. && make
	cp gui/desktop/build/desktop .

# Linking the executable
tetris_cli: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

snake_cli: libsnake.a
	$(CXX) $(CXXFLAGS) $(SRC_FRONTEND) main_snake_cli.cpp libsnake.a -o snake_cli $(LIBS)

libsnake.a: $(OBJ_SNAKE_BACKEND)
	ar rcs $@ $^

install: desktop tetris_cli snake_cli
	mkdir -p $(BIN)
	cp tetris_cli $(BIN)
	cp snake_cli $(BIN)
	cp desktop $(BIN)

test: libsnake.a
	@echo "Running tests..."
	$(CXX) $(CXXFLAGS) $(TESTS_SRC) libsnake.a $(TESTS_LIBS) -o test 
	./test

uninstall:
	rm -rf $(BIN)

dist: clean_all
	cd .. && tar -czf brick_gameV2.tar.gz src/brick_game src/gui src/Makefile

dvi:
	@echo "Generating documentation..."
	rm -rf ../docs
	mkdir -p ../docs
	@doxygen doxygen.cfg
	@echo "Documentation generated in ../docs"

backend: $(OBJ_BACKEND)
	ar rcs libtetris.a $^
# Clean up
clean:
	rm -rf $(BUILD_DIR)

clean_target:
	rm -f tetris_cli
	rm -f snake_cli
	rm -f desktop
	rm -rf ../docs
	rm -rf ../brick_gameV2.tar.gz
	rm -rf gui/desktop/build
	rm -rf $(BIN)

clean_all: clean clean_test clean_backend clean_target

clean_backend:
	rm -f libtetris.a
	rm -f libsnake.a

clean_test:
	rm -f test


# Debugging the Makefile
cout: 
	@echo "Source files:" $(SRC)
	@echo "Object files:" $(OBJ)
	@echo "Snake source files:" $(SRC_SNAKE)
	@echo "Snake object files:" $(OBJ_SNAKE_BACKEND)
	
clang-format:
	clang-format -style=file:$(STYLE_CONFIG) -i $(SRC) $(TESTS_SRC)
	clang-format -style=file:$(STYLE_CONFIG) -i $(HEADERS) $(TESTS_H)
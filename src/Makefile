

CC = gcc
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -Wextra
CFLAGS = -std=c11 -Wall -Werror -Wextra
SRC_BACKEND = $(shell find brick_game/tetris -name 'cli.c')
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

BUILD_DIR = build
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:.o=.d)
STYLE_CONFIG = ../materials/linters/.clang-format
TEST_LIBS = -lcheck -lsubunit -lm
DESTDIR = .
BIN = $(DESTDIR)/brick_game
# OS-specific library flags
ifeq ($(OS), Darwin)
    LIBS = -lm -lncurses
else
    LIBS = -lm -lncursesw -pthread -lrt 
endif

# Default target
all: tetris_cli
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
	cp gui/desktop/build/desktop $(BIN)

# Linking the executable
tetris_cli: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

snake_cli: libsnake.a
	$(CXX) $(CXXFLAGS) $(SRC_FRONTEND) main_snake_cli.cpp libsnake.a -o snake $(LIBS)

libsnake.a: $(OBJ_SNAKE_BACKEND)
	ar rcs $@ $^

install: tetris
	mkdir -p $(BIN)
	cp tetris $(BIN)

uninstall:
	rm -rf $(BIN)

dist: tetris_cli clean
	cd .. && tar -czf tetris.tar.gz src/tetris

dvi: tetris_cli
	@echo "Generating documentation..."
	cd .. && rm -rf docs
	cd .. && mkdir -p docs
	cd .. && doxygen ACTIVE_TAG_FILES.txt
	cd .. && $(OPEN_CMD) docs/html/index.html

backend: $(OBJ_BACKEND)
	ar rcs libtetris.a $^
# Clean up
clean:
	rm -rf $(BUILD_DIR)

clean_target:
	rm -rf tetris

clean_all: clean clean_test clean_backend clean_target

clean_backend:
	rm -f libtetris.a

# Debugging the Makefile
cout: 
	@echo "Source files:" $(SRC)
	@echo "Object files:" $(OBJ)
	@echo "Snake source files:" $(SRC_SNAKE)
	@echo "Snake object files:" $(OBJ_SNAKE_BACKEND)
	



clang-format:
	clang-format -style=file:$(STYLE_CONFIG) -i $(SRC) $(TESTS_SRC)
	clang-format -style=file:$(STYLE_CONFIG) -i $(HEADERS) $(TESTS_H)
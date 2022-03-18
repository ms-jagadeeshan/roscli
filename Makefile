TARGET = roscli
SRC = src
INC = include
BIN = build

SOURCE_CPP = $(wildcard $(SRC)/*.cpp)
SOURCE_C = $(wildcard $(SRC)/*.c)
TARGET_CPP = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE_CPP:.cpp=)))
TARGET_C = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE_C:.c=)))

CC = gcc
CXX = g++

WARNINGS = -Wall -Wextra -Wunused -pedantic
CCFLAGS = -O1
CXXFLAGS = -O1 -fcompare-debug-second 
INCLUDE = -I$(INC) 
LIBS =  -lncurses -lmenu

# Colors
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
NC = \033[1;0m

install : build
	sudo chown root:root $(BIN)/ttyecho
	sudo cp $(BIN)/ttyecho /usr/bin
	mkdir -p "${HOME}/.config/roscli"
	cp $(BIN)/ros.bash "${HOME}/.ros.bash"

# Build the executable in build folder
build : $(TARGET_C) $(TARGET_CPP) 

# Rules for cpp files
$(BIN)/% : $(SRC)/%.cpp
	@echo "$(GREEN)Compiling...$(NC)"
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@ 

# Rules for c files
$(BIN)/% : $(SRC)/%.c
	@echo "$(GREEN)Compiling...$(NC)"
	@mkdir -p $(BIN)
	$(CC) $(CCFLAGS) $< -o $@ $(LIBS)

.PHONY : install build help run clean uninstall

run: $(BIN)/$(TARGET)
	@echo "$(YELLOW)Running...$(NC)"
	cd $(BIN) && ./$(TARGET)

clean:
	rm -f $(TARGET_CPP) $(TARGET_C)

purge: uninstall
	rm -rf "${HOME}/.config/roscli"

uninstall:
	sudo rm /usr/bin/ttyecho
	rm "${HOME}/.ros.bash"

help :
	@echo "src_cpp   : $(SOURCE_CPP)"
	@echo "src_c     : $(SOURCE_C)"
	@echo "target_cpp  : $(TARGET_CPP)"
	@echo "target_c    : $(TARGET_C)"

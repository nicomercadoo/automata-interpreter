MAKEFLAGS += --no-print-directory
CXX = g++
CXX_FLAGS = -std=c++17 -I $(HEADERS_F)/
W_FLAGS = -Wall -Wextra -Werror -Wpedantic
HEADERS_F = headers
SRC_F = src
BUILD_F = build
OBJS = $(BUILD_F)/main.o $(BUILD_F)/automata.o $(BUILD_F)/automata-rep.o $(BUILD_F)/state.o $(BUILD_F)/symbol.o
BINARY = ./afnd-runner

# TIP: Take a look to usefull phony rules at the end of this file!

### executable compilation rules

main: executable

executable: $(OBJS)
	@echo
	@echo "\e[1;33mCreating executable...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -o $(BINARY) $(OBJS)
	@echo
	@echo "\e[1;33mExecutable created:\e[0m $(BINARY)"

dbg: $(OBJS)
	@echo
	@echo "\e[1;33mCreating executable with debug objects...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -g3 -o $(BINARY) $(OBJS)
	@echo
	@echo "\e[1;33mExecutable with debug objects created: $(BINARY)\e[0m"

### objects files compilation rules

$(BUILD_F)/main.o: $(SRC_F)/main.cpp
	@echo
	@echo "\e[1;33mCompiling main.o...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -c $(SRC_F)/main.cpp -o $(BUILD_F)/main.o

$(BUILD_F)/automata.o: $(SRC_F)/automata.cpp $(HEADERS_F)/automata.hpp
	@echo
	@echo "\e[1;33mCompiling automata.o...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -c $(SRC_F)/automata.cpp -o $(BUILD_F)/automata.o

$(BUILD_F)/automata-rep.o: $(SRC_F)/automata-rep.cpp $(HEADERS_F)/automata-rep.hpp
	@echo
	@echo "\e[1;33mCompiling automata-rep.o...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -c $(SRC_F)/automata-rep.cpp -o $(BUILD_F)/automata-rep.o

$(BUILD_F)/state.o: $(SRC_F)/state.cpp $(HEADERS_F)/state.hpp
	@echo
	@echo "\e[1;33mCompiling state.o...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -c $(SRC_F)/state.cpp -o $(BUILD_F)/state.o

$(BUILD_F)/symbol.o: $(SRC_F)/symbol.cpp $(HEADERS_F)/symbol.hpp
	@echo
	@echo "\e[1;33mCompiling symbol.o...\e[0m"
	@mkdir -p $(BUILD_F)
	$(CXX) $(CXX_FLAGS) -c $(SRC_F)/symbol.cpp -o $(BUILD_F)/symbol.o


### phony rules

# clean project build files and the executable
.PHONY: clean
clean:
	@echo
	@echo "\e[1;33mCleaning project build files and executables...\e[0m"
	rm $(BINARY) ; rm -rf $(BUILD_F)

# enable compiler warnings
.PHONY: warns
warns:
	@echo
	@echo "\e[1;33mWarnings enabled:\e[0m $(W_FLAGS)"
	$(MAKE) CXX_FLAGS=$(CXX_FLAGS) $(W_FLAGS)

# aliases
.PHONY: e
e: executable

.PHONY: d
d: dbg

.PHONY: w
w: warns

.PHONY: c
c: clean

.PHONY: recompile r
recompile r: clean warns

CXX = g++
HEADERS_F = headers
SRC_F = src
BUILD_F = build
OBJS = $(BUILD_F)/main.o $(BUILD_F)/automata.o $(BUILD_F)/automata-rep.o $(BUILD_F)/state.o $(BUILD_F)/symbol.o
BINARY = afnd-runner

main:
	g++ src/* -o afnd-runner -I headers

# main: executable

# executable: $(OBJS)
# 	g++ src/* -o afnd-runner -I headers
# 	$(CXX) -o $(BINARY) $(OBJS) -I $(HEADERS_F)/

# dbg: $(OBJS)
# 	$(CXX) -g3 -o $(BINARY) $(OBJS)

# main.o: $(SRC_F)/main.cpp $(HEADERS_F)/*
# 	$(CXX) -c $(SRC_F)/main.cpp -o $(BUILD_F)/main.o -I $(HEADERS_F)/

# # main.cpp: $(HEADERS_F)/automata.h $(HEADERS_F)/automata-rep.h $(HEADERS_F)/state.h $(HEADERS_F)/symbol.h

# automata.o: automata.cpp $(HEADERS_F)/automata.h
# 	$(CXX) -c automata.cpp -o $(BUILD_F)/automata.o

# automata-rep.o: automata-rep.cpp $(HEADERS_F)/automata-rep.h
# 	$(CXX) -c automata-rep.cpp -o $(BUILD_F)/automata-rep.o

# state.o: state.cpp $(HEADERS_F)/state.h
# 	$(CXX) -c state.cpp -o $(BUILD_F)/state.o

# symbol.o: symbol.cpp $(HEADERS_F)/symbol.h
# 	$(CXX) -c symbol.cpp -o $(BUILD_F)/symbol.o

# .PHONY: clean
# clean:
# 	rm -f $(OBJS) $(BINARY) ; rm -rf $(BUILD_F)





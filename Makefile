# Compiler variables
GOAL = mylang.out
CXX = g++
CXXFLAGS = -std=c++2a -Wall -Wextra -Wno-unused -g -fconcepts

# Relevant directories
INC_DIR = include
SRC_DIR = src
BLD_STS = build_stats
OBJ_DIR = $(SRC_DIR)/obj
LIB_DIR = $(SRC_DIR)/libs

# Build rules
$(GOAL): $(OBJ_DIR)/lex.yy.o $(OBJ_DIR)/parser.tab.o $(OBJ_DIR)/driver.o $(OBJ_DIR)/ast.o $(OBJ_DIR)/data_types.o $(OBJ_DIR)/data.o $(OBJ_DIR)/operators.o $(OBJ_DIR)/semantic.o $(OBJ_DIR)/structs.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/parser.tab.o: $(SRC_DIR)/parser.tab.cpp $(INC_DIR)/parser.tab.hpp $(INC_DIR)/driver.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/lex.yy.o: $(SRC_DIR)/lex.yy.c $(INC_DIR)/parser.tab.hpp $(INC_DIR)/driver.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/driver.o: $(SRC_DIR)/driver.cpp $(INC_DIR)/driver.hpp $(INC_DIR)/color.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/ast.o: $(SRC_DIR)/ast/ast.cpp $(INC_DIR)/ast/ast.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/data_types.o: $(SRC_DIR)/lang/data_types.cpp $(INC_DIR)/lang/data_types.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/data.o: $(SRC_DIR)/lang/data.cpp $(INC_DIR)/lang/data.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/operators.o: $(SRC_DIR)/lang/operators.cpp $(INC_DIR)/lang/operators.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/semantic.o: $(SRC_DIR)/lang/semantic.cpp $(INC_DIR)/lang/semantic.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/structs.o: $(SRC_DIR)/structs.cpp $(INC_DIR)/structs.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/lexer.lex
	flex -o $@ $<

$(SRC_DIR)/parser.tab.cpp $(INC_DIR)/parser.tab.hpp: $(SRC_DIR)/parser.ypp
	bison $< -d && mv *.hh include && mv *.hpp include && mv *.cpp src && sed -i 's?#include "parser.tab.hpp"?#include "../include/parser.tab.hpp"?g' $(SRC_DIR)/parser.tab.cpp

.PHONY: clean
clean:
	rm $(GOAL) $(OBJ_DIR)/*.o $(BIN_DIR)/*.out $(SRC_DIR)/parser.tab.* $(INC_DIR)/parser.tab.* $(BLD_STS)/parser.output \
		$(SRC_DIR)/lex.yy.* $(INC_DIR)/stack.hh $(INC_DIR)/position.hh $(INC_DIR)/location.hh

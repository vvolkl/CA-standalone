
CXXFLAGS=-std=c++11

all: main


main: main.cpp CellularAutomaton.cc
	g++ -std=c++11  main.cpp -o main



CFLAGS := -O3 -Wall -Iincludes -Llib -lraylib
CPPFLAGS := -std=c++23 -O3 -Wall -Iincludes -Llib -lraylib

ifeq ($(OS),Windows_NT)
	CFLAGS += -lwinmm -lgdi32
	CPPFLAGS += -lwinmm -lgdi32
endif

molbuilder: 
	gcc molbuilder.c $(CFLAGS) -o molbuilder.exe

graphbuilder: 
	g++ graphbuilder.cxx $(CPPFLAGS) -o graphbuilder.exe

h-molbuilder:
	cabal run

test_graph:
	g++ test_graph.cxx $(CPPFLAGS) -o test_graph.exe

nodePong:
	g++ nodePong.cxx $(CPPFLAGS) -o nodePong.exe

test_gen_graph:
	g++ test_gen_graph.cxx $(CPPFLAGS) -o test_gen_graph.exe
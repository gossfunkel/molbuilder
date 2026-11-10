CFLAGS := -O3 -Wall -Iincludes -lraylib
CPPFLAGS := -std=c++23 -O3 -Wall -Iincludes -lraylib -lm -lglfw -lX11

ifeq ($(OS),Windows_NT)
	CFLAGS += -L/raylib/raylib/build/raylib -I/raylib/raylib/build/raylib/include -lwinmm -lgdi32
	CPPFLAGS += -L/raylib/raylib/build/raylib -I/raylib/raylib/build/raylib/include -lwinmm -lgdi32
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

graph:
	gcc graph.cxx $(CFLAGS) -o graph.exe


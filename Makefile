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

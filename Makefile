CFLAGS := -O3 -Wall -Iincludes -Llib -lraylib

ifeq ($(OS),Windows_NT)
	CFLAGS += -lwinmm -lgdi32
endif

molbuilder: 
	gcc molbuilder.c $(CFLAGS) -o molbuilder.exe

graphbuilder: 
	g++ graphbuilder.cxx $(CFLAGS) -o graphbuilder.exe

h-molbuilder:
	cabal run

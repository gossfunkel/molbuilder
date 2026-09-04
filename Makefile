CFLAGS := -O3 -Wall -Iincludes -Llib -lraylib

ifeq ($(OS),Windows_NT)
	CFLAGS += -lwinmm -lgdi32
endif

molbuilder: 
	g++ molbuilder.c $(CFLAGS) -o molbuilder.exe

h-molbuilder:
	cabal run

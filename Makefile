###############################
# FILE: Makefile for driver   #
# NAME: Natalie Podrazik      #
# CLASS: CS 441               #
# EMAIL: natalie2@umbc.edu    #
###############################

PROGRAMS = Sudoku.exe

compiler = gcc 

compilerflags = -ansi -Wall -lm

all: $(PROGRAMS)

Sudoku.exe: part1.o util.o
	$(compiler) $(compilerflags) -o Sudoku.exe part1.o util.o

part1.o: part1.c util.h
	$(compiler) $(compilerflags) -c part1.c

util.o: util.c util.h
	${compiler} ${compilerflags} -c util.c

clean:
	rm -f *.o *.c~ Sudoku.exe


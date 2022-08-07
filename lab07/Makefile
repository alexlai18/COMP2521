# COMP2521 Lab 7 Makefile

# !!! DO NOT MODIFY THIS FILE !!!

CC = gcc
CFLAGS = -Wall -Werror -g

.PHONY: all
all: solveBfs solveDfs solveDfsBacktrack solveKeepLeft

solveBfs: solver.c Maze.c solveBfs.c Queue.c helpers.c
	$(CC) $(CFLAGS) solver.c Maze.c solveBfs.c Queue.c helpers.c -o solveBfs

solveDfs: solver.c Maze.c solveDfs.c Stack.c helpers.c
	$(CC) $(CFLAGS) solver.c Maze.c solveDfs.c Stack.c helpers.c -o solveDfs

solveDfsBacktrack: solver.c Maze.c solveDfsBacktrack.c Stack.c helpers.c
	$(CC) $(CFLAGS) solver.c Maze.c solveDfsBacktrack.c Stack.c helpers.c -o solveDfsBacktrack

solveKeepLeft: solver.c Maze.c solveKeepLeft.c Queue.c helpers.c
	$(CC) $(CFLAGS) solver.c Maze.c solveKeepLeft.c Queue.c helpers.c -o solveKeepLeft

.PHONY: clean
clean:
	rm -f solveBfs solveDfs solveDfsBacktrack solveKeepLeft

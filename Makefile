# COMP2521 Assignment 1 Makefile
#
# HOW TO USE
# 1. Ensure that your directory structure looks like:
#    ass1/
#        exmp1/
#            ...
#        exmp2/
#            ...
#        Makefile (this Makefile)
#        invertedIndex.c
#        invertedIndex.h
#        Your supporting files...
#
# 2. Read all the comments below and edit the Makefile accordingly
#
# 3. After you use the make command, executables will be created in
#    the exmp1/ and exmp2/ directories. Change into these directories
#    and run your programs.

CC = gcc
CFLAGS = -Wall -Werror -g

# NOTE:
# None of your .c files should contain a main() function
# List all your supporting C files here
# For example: SUPPORTING_FILES = hello.c world.c
SUPPORTING_FILES = 

# NOTE:
# If you want to create your own test, make a new directory (e.g.,
# called exmp3) containing the test data and a testInvertedIndex.c
# with a main function, add the executable name to the all list below,
# and add a target for it - you can just follow the same pattern.

.PHONY: all
all: exmp1/testInvertedIndex exmp2/testInvertedIndex

exmp1/testInvertedIndex: exmp1/testInvertedIndex.c invertedIndex.c $(SUPPORTING_FILES)
	$(CC) $(CFLAGS) -o exmp1/testInvertedIndex exmp1/testInvertedIndex.c invertedIndex.c $(SUPPORTING_FILES) -lm

exmp2/testInvertedIndex: exmp2/testInvertedIndex.c invertedIndex.c $(SUPPORTING_FILES)
	$(CC) $(CFLAGS) -o exmp2/testInvertedIndex exmp2/testInvertedIndex.c invertedIndex.c $(SUPPORTING_FILES) -lm

.PHONY: clean
clean:
	rm -f */testInvertedIndex


# Makefile
#
# Clement Poh
#
# Makefile for assignment one of COMP20007 2016.
#
# Replace '<STUDENT-ID>' with your Student ID e.g. 269508
STUDENTID = 736901
SUBMIT	= toposort.c graph.c graphio.c report.pdf

# Define the C compiler
CC      = gcc

# Define the compiler flags
#  -Wall to turn on most of the warnings
#  -Wextra to turn on even more warnings
#  -Wpedantic to turn on even more warnings
#  -ansi to ensure ansi compliance
#  -std=c99 to ensure c99 compliance
#  -m32 to compile for 32 bit architectures
#  -O0 to turn off optimisations
#  -g  to include debugging symbols
CFLAGS  = -Wall -Wextra -Wpedantic -ansi -std=c99 -g -O0

# Define any libraries to be linked
LIB		  =

# Define the source, header and object files
HDR		  = src/list.h src/graph.h src/graphio.h src/toposort.h
SRC	  	= src/list.c src/graph.c src/graphio.c src/toposort.c src/main.c
OBJ     = $(SRC:.c=.o)

# The executable name
TARGET	= bin/toposort

.PHONY: all
all: compile daggen

compile: $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIB)

daggen: test/daggen.c
	$(CC) $(CFLAGS) -o bin/daggen test/daggen.c

clean:
	rm -f $(OBJ) *.dot *.png test/tmp.* bin/*

usage: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

dot: $(TARGET)
	./$(TARGET) -p test/t1.dot test/t1.txt
	dot test/t1.dot -Tpng -o bin/t1.png

test: $(TARGET) daggen
	./bin/daggen > test/tmp.txt
	./$(TARGET) -p test/tmp.dot test/tmp.txt
	./$(TARGET) -m 1 test/tmp.txt > test/tmp.out
	./$(TARGET) -v test/tmp.txt < test/tmp.out
	./$(TARGET) -m 2 test/tmp.txt > test/tmp.out
	./$(TARGET) -v test/tmp.txt < test/tmp.out

$(OBJ): $(HDR)

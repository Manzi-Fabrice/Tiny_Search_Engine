# Manzi Fabrice Niyigaba
# COSC 50 Fall 2024, TSE Common Directory Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib -I../set

# Memory-leak testing
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# Libraries and objects
LIBS = ../libcs50/libcs50-given.a
LIB = commonlib.a
OBJS = pagedir.o word.o index.o

# Rule to create the common library
$(LIB): $(OBJS)
	@ar cr $(LIB) $(OBJS)

# Object dependencies on headers
pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

# Clean rule to remove generated files
clean:
	rm -f *~ *.o
	rm -f $(EXEC)

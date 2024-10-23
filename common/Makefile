# Makefile for building the CS50 pagedir module
# Manzi Fabrice Niyigaba, October 2024
# This Makefile compiles the pagedir module and creates a static library `common.a`.

# Variables
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I ../libcs50 
LIBS = ../libcs50/libcs50.a  
LIB = common.a  
OBJS = pagedir.o  


$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)  # Create static library from object files


pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c  


.PHONY: clean
clean:
	rm -rf *.dSYM 
	rm -f *~ *.o  
	rm -f $(LIB)  
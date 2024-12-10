# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../lib -I../common -I../libcs50

# Linker flags and libraries
LIBS = ../common/commonlib.a ../libcs50/libcs50-given.a

# Executable name
EXEC = querier

# Object files
OBJS = querier.o validate.o

# Build querier executable
$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# Dependencies for object files
querier.o: querier.c ../common/pagedir.h ../common/word.h ../libcs50/counters.h ../common/index.h ../common/index.h
validate.o: validate.c validate.h ../libcs50/counters.h

# Pattern rule for building object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Phony targets
.PHONY: clean valgrind test debug

test: 
	./testing.sh 

# Clean up generated files
clean:
	rm -f *~ *.o
	rm -f $(EXEC)

# Run valgrind on querier
valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) ../letters-depth-2 ../letters-2.index


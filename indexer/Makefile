# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../lib -I../common -I../libcs50

# Linker flags and libraries
LIBS = ../common/commonlib.a ../libcs50/libcs50-given.a

# Executable names
EXEC = indexer
INDEXTEST = indextest

# Object files
OBJS = indexer.o ../common/pagedir.o ../common/word.o ../common/index.o
ITOBJS = indextest.o ../common/pagedir.o 

# Build indexer executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# Build indextest executable
$(INDEXTEST):  $(ITOBJS)
	$(CC) $(CFLAGS) $(ITOBJS) $(LIBS) -o $@

# Dependencies for object files
indexer.o: indexer.c ../common/pagedir.h ../common/word.h ../common/index.h ../libcs50/hashtable.h
indextest.o: indextest.c ../common/pagedir.h ../common/index.h ../libcs50/hashtable.h

# Pattern rule for building object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Phony targets
.PHONY: clean valgrind test

# Clean up generated files
clean:
	rm -f *~ *.o
	rm -f $(EXEC) $(INDEXTEST)

# Run valgrind on indexer
# Run valgrind on indexer with arguments
valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) ../data/letters_test testing.txt


# Run valgrind on indextest
valgrind-indextest: $(INDEXTEST)
	valgrind --leak-check=full --show-leak-kinds=all ./$(INDEXTEST)

# Run tests
test: $(EXEC)
	bash testing.sh

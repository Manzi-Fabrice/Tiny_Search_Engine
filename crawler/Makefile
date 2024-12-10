# Makefile for building and testing the CS50 Crawler module
# Manzi Fabrice Niyigaba, October 2024
# This Makefile compiles the crawler module and links it with the required libraries.

# variables 
OBJ = crawler.o
LIBS = ../libcs50/libcs50-given.a  ../common/common.a  
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50  
CC = gcc  


crawler: $(OBJ) $(LIBS)
	$(CC) $(CFLAGS) $^ -o crawler $(LIBS)  


crawler.o: crawler.c crawler.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/bag.h
	$(CC) $(CFLAGS) -c crawler.c 


.PHONY: clean test valgrind


test:
	./testing.sh &> testing.out  

valgrind: crawler
	valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2


clean:
	rm -rf testing.out  
	rm -rf *.o crawler  

/*
 * indexer.h - header file for CS50 TSE Indexer module
 *
 * The indexer is responsible for reading files from a specified directory,
 * building an in-memory index of words and their occurrences in documents,
 * and saving that index to a file.
 *
 * Manzi Fabrice Niyigaba, October 2024
 */

#ifndef __INDEXER_H
#define __INDEXER_H

#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

typedef hashtable_t index_t;


/**************** functions ****************/

/**************** index_build ****************/
/* Builds an in-memory index by scanning each webpage in a given page directory.
 * 
 * Caller provides:
 *   the directory path where the pages are stored (pageDirectory),
 *   and an allocated hashtable to store the index.
 * We do:
 *   iterate over each page in the directory, loading the page data,
 *   and adding each valid word (length >= 3) to the index.
 *   The index is a hashtable where each word maps to a counters object.
 *   Each counters object holds document IDs and counts of word occurrences.
 * Caller is responsible for:
 *   providing a valid page directory and an allocated hashtable for indexing.
 * Notes:
 *   If an error occurs (e.g., page loading fails), a message is printed to stderr.
 */
void index_build(char* pageDirectory, index_t* index);

/**************** indexPage ****************/
/* Processes each page, adding words and their occurrences to the index.
 * 
 * Caller provides:
 *   a loaded webpage (page), document ID (docID), and an index (hashtable).
 * We do:
 *   extract each word from the webpage, normalize it, and if its length
 *   is >= 3, add it to the hashtable. If the word already exists, increment
 *   the count in the corresponding document's counters.
 * Caller is responsible for:
 *   ensuring the page, docID, and index are valid.
 */
void indexPage(webpage_t* page, int docID, index_t* index);

#endif // __INDEXER_H

#ifndef __INDEXER_H
#define __INDEXER_H

#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

typedef struct index {
    hashtable_t *ht;  // Pointer to the hashtable
    // Other fields if needed
} index_t;


/**************** functions ****************/

void index_build(char* pageDirectory, index_t* index);

void indexPage(webpage_t* page, int docID, index_t* index);

/* Add semicolon at the end of the function prototype */
index_t* index_load(char* file); // Add semicolon

void index_delete(index_t* index); // Ensure this is declared if not already

index_t* index_new(int size);

void index_save(const char* filename, index_t* index);



#endif // __INDEXER_H

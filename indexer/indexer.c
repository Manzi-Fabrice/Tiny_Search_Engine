/*
 * indexer.c - CS50 indexer module
 *
 * Manzi Fabrice Niyigaba October 20 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "indexer.h"


// Function prototypes
void index_build(char* pageDirectory, index_t* index);
void indexPage(webpage_t* page, int docID, index_t* index);

int main(const int argc, char* argv[]){
    if (argc != 3){
        fprintf(stderr, "Invalid number of inputs\n");
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];

    if (pagedir_verify(pageDirectory) != 0){
        fprintf(stderr, "Failed to validate provided directory '%s'\n", pageDirectory);
        exit(2);
    }

    // Create a new index
    index_t* index = index_new(800);
    if (index->ht == NULL){
        fprintf(stderr, "Failed to create index\n");
        exit(3);
    }

    // Build the index from the page directory
    index_build(pageDirectory, index);

    // Save the index to a file
    index_save(indexFilename, index);

    // Clean up
    index_delete(index);

    return 0;
}

/**************** index_build() ****************/
/* see indexer.h for more information */
void index_build(char* pageDirectory, index_t* index) {
    int docID = 1;
    webpage_t* page;
    char filename[16];
    char* pathname;
    FILE* fp;

    sprintf(filename, "%d", docID);
    pathname = get_pathname(pageDirectory, filename);
    fp = fopen(pathname, "r");

    while (fp != NULL) {
        fclose(fp);
        page = pagedir_load(pathname);  
        if (page == NULL) {
            fprintf(stderr, "Unable to load the page '%s'\n", pathname);
            free(pathname);
            docID++;
            sprintf(filename, "%d", docID);
            pathname = get_pathname(pageDirectory, filename);
            fp = fopen(pathname, "r");
            continue;
        }

        // Passes the webpage and docID to indexPage
        indexPage(page, docID, index);

        // Clean up after processing the page
        webpage_delete(page);
        free(pathname);

        // Prepare for next iteration
        docID++;
        sprintf(filename, "%d", docID);
        pathname = get_pathname(pageDirectory, filename);
        fp = fopen(pathname, "r");
    }
    free(pathname);  // Free the last pathname allocated
}

/**************** indexPage() ****************/
/* see indexer.h for more information */
void indexPage(webpage_t* page, int docID, index_t* index) {
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word) >= 3) {  
            char* normalized_word = normalize(word);  
            if (normalized_word != NULL) {
                counters_t* wordcounts = hashtable_find(index->ht, normalized_word);
                if (wordcounts == NULL) {
                    wordcounts = counters_new();
                    if (wordcounts == NULL) {
                        fprintf(stderr, "Failed to create counters for word\n");
                        free(normalized_word);
                        free(word);
                        continue;  // Skip to next word
                    }
                    if (!hashtable_insert(index->ht, normalized_word, wordcounts)) {
                        fprintf(stderr, "Failed to insert into hashtable\n");
                        counters_delete(wordcounts);
                        free(normalized_word);
                        free(word);
                        continue;
                    }
                } 
                int current_count = counters_get(wordcounts, docID);
                counters_set(wordcounts, docID, current_count + 1); 
            } 
            free(normalized_word);
            free(word);
            
        } else {
            free(word);  // Free word if it's shorter than 3 characters
        }
    }
}

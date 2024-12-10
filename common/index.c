/*
 * index.c - CS50 index module
 *
 * Manzi Fabrice Niyigaba October 20 2024
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>

#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "index.h"
#include "../libcs50/file.h"



// Function prototypes for helper functions
static void index_save_helper(void *arg, const char *key, void* item);
static void counters_iterate_helper(void* file, const int id, const int count);
void index_load_helper(FILE* fp, index_t* index);
index_t* index_load(char* file);

/**************** index_new() ****************/
/* Creates a new index (hashtable) */
index_t* index_new(const int num_slots) {
    index_t* index = malloc(sizeof(index_t));  // Allocate memory for the new index
    if (index == NULL) {
        return NULL;  // Handle memory allocation failure
    }

    index->ht = hashtable_new(num_slots);  // Initialize the hashtable within the index
    if (index->ht == NULL) {
        free(index);  // Free index if hashtable allocation fails
        return NULL;
    }

    return index;
}


/**************** index_delete() ****************/
/* Deletes the index and frees associated memory */
void index_delete(index_t* index) {
    hashtable_delete(index->ht, (void (*)(void *)) counters_delete);
    free(index);
}

/**************** index_save() ****************/
/* Saves the index to a file */
void index_save(const char *fname, index_t* index){
    FILE* fp= fopen(fname, "w");
    if (fp == NULL){
        fprintf(stderr, "Failed to open the file '%s' for writing\n", fname);
        return;
    }
    hashtable_iterate(index->ht, fp, index_save_helper);
    fclose(fp);
}

/**************** index_save_helper() ****************/
/* Helper function for hashtable_iterate to save each word and its counts */
static void index_save_helper(void *arg, const char *key, void* item){
    FILE *fp = (FILE*) arg;
    counters_t* ctrs = (counters_t*) item;
    fprintf(fp, "%s", key);
    counters_iterate(ctrs, fp, counters_iterate_helper);
    fprintf(fp, "\n");
}

/**************** counters_iterate_helper() ****************/
/* Helper function for counters_iterate to save each docID and count */
static void counters_iterate_helper(void* file, const int id, const int count){
    fprintf((FILE*)file, " %d %d", id, count);
}



void index_load_helper(FILE* fp, index_t* index) {
    char* line;
    while ((line = file_readLine(fp)) != NULL) {
        char* saveptr;
        char* token = strtok_r(line, " ", &saveptr);
        if (token == NULL) {
            free(line);
            continue;
        }

        char* word = token;

        counters_t* ctrs = counters_new();
        if (ctrs == NULL) {
            fprintf(stderr, "Error: Memory allocation for counters failed.\n");
            free(line);
            continue;
        }

        // Parse docID-count pairs
        int docID = 0, count = 0;
        int token_index = 0;
        bool valid_line = true;

        // Process each token (docID and count pairs)
        while ((token = strtok_r(NULL, " ", &saveptr)) != NULL) {
            if (token_index % 2 == 0) {  // docID expected
                docID = atoi(token);
            } else {  // count expected
                count = atoi(token);
                if (count <= 0 || docID <= 0) {  // Invalid data check
                    valid_line = false;
                    break;
                }
                counters_set(ctrs, docID, count);
            }
            token_index++;
        }

        // Ensure even number of tokens and valid pairs
        if (token_index % 2 != 0 || !valid_line) {
            fprintf(stderr, "Error: Malformed line in index file.\n");
            counters_delete(ctrs);  // Free counters if line is malformed
            free(line);             // Free line before moving to the next iteration
            continue;
        }

        // Insert word and counters into the hashtable
        if (!hashtable_insert(index->ht, word, ctrs)) {  // Check for insertion failure
            fprintf(stderr, "Error: Failed to insert into hashtable.\n");
            counters_delete(ctrs);  // Free counters if insertion fails
        }

        free(line);  // Free line at the end of each iteration
    }
}


index_t* index_load(char* file) {
    index_t* index = index_new(100);  
    if (index == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for index.\n");
        return NULL;
    }

    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "failed to open the indexer's file for reading");
        index_delete(index);  // Clean up index if file open fails
        return NULL;
    }

    char* line = file_readLine(fp);
    if (line == NULL) {
        fprintf(stderr, "Empty file");
        free(line);
        index_delete(index);  // Clean up index if file is empty
        fclose(fp);
        return NULL;
    }
    free(line);  // Free the first line after reading

    index_load_helper(fp, index);  // Populate index
    fclose(fp);  // Close file after reading
    return index;
}


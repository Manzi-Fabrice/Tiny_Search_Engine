/*
 * querier.c - CS50 querier module
 * 
 * This module processes search queries to retrieve relevant documents from an indexed dataset.
 * It relies on the crawler module for page data and the indexer module for the search index.
 * 
 * Manzi Fabrice Niyigaba, November 2024
 */



# define _GNU_SOURCE
# include "validate.h"
# include<stdio.h>
# include<stdlib.h>
# include <string.h>
# include <ctype.h>
# include <stdbool.h>
# include "../common/word.h"
# include "../libcs50/counters.h"
# include "../common/pagedir.h"
# include "../common/index.h"
# include "../libcs50/file.h"
#include "../common/index.h"


/*************** qeryctrs ***************
 * Used to store intermediate and final counters
 * during query execution.
 * - `final`: cumulative counters for the final query result.
 * - `temp`: temporary counters used for intermediate query logic.
 */
struct qeryctrs {
    counters_t* final;       // cumulative counters for entire query
    counters_t* temp;        // intermediate counters for logic steps
};

/*************** doc_score_t ***************
 * Represents a document's score for ranking purposes.
 * - `docID`: unique identifier for the document.
 * - `score`: relevance score calculated for the document.
 */
typedef struct doc_score_t {
    int docID;               // document identifier
    int score;               // relevance score for ranking
} doc_score_t;

/*************** and_helper_args ***************
 * Arguments struct used in AND operations during query execution.
 * - `temp`: stores intersection results for AND logic.
 * - `curr`: current counters being intersected with `temp`.
 */
struct and_helper_args {
    counters_t* temp;        // intersection results
    counters_t* curr;        // current counters in intersection
};

/*************** score_helper_args ***************
 * Helper structure for populating the document scores array.
 * - `scores`: array of doc_score_t to populate with results.
 * - `index`: current position in the scores array for insertion.
 */
struct score_helper_args {
    doc_score_t* scores;     // array to hold document scores
    int index;               // current index in scores array
};


// Function Prototypes
index_t* validate_and_load_index(int argc, char* argv[]);
void process_queries(index_t* index, const char* page_directory);
counters_t* execute_query(char** word, int word_count, index_t* index);
doc_score_t* rank_documents(counters_t* final, int* num_docs);
void display_output(doc_score_t* scores, int num_docs, const char* pagedir);


int main(int argc, char* argv[])
{
    index_t* index = validate_and_load_index(argc, argv);
    if (index == NULL) {
        fprintf(stderr, "Error: Failed to validate inputs and load index.\n");
        exit(1);
    }
    char* page_directory = argv[1];

    // Start processing user queries
    process_queries(index, page_directory);

    // Clean up and exit
    index_delete(index); 
    return 0;
}



/**************** validate_and_load_index ****************/
/* Validates command-line arguments and loads the index.
 *
 * Parameters:
 *   argc - number of arguments
 *   argv - array of argument strings
 *
 * Returns:
 *   Loaded index if inputs are valid; exits on error.
 */

index_t* validate_and_load_index(int argc, char* argv[]){
    if (argc!=3){
        fprintf(stderr, "invalid number of inputs");
        exit(1);
    }

    char* pageDirectory = argv[1];
    char* indexerfile = argv[2];

    if (pagedir_verify(pageDirectory) != 0) {
        fprintf(stderr, "Invalid directory provided\n");
        exit(2);
    }
    index_t* index = index_load(indexerfile);
    if (index == NULL) {
        fprintf(stderr, "Failed to load the index from file: %s\n", indexerfile);
        exit(3);
    }
    return index;
}


/**************** process_queries ****************/
/* Processes user queries and displays matching documents.
 *
 * Parameters:
 *   index - loaded index for query processing
 *   page_directory - directory of crawled pages for document paths
 *
 * Returns:
 *   None; exits on EOF or error.
 */
void process_queries(index_t* index, const char* page_directory) {
    char* input = NULL;
    size_t len = 0;

    while (1) {
        printf("Query? ");
        ssize_t nread = getline(&input, &len, stdin);

        // Check for EOF or getline errors
        if (nread == -1) {
            printf("\n");
            break;
        }

        if (nread <= 1) {  // only '\n' or empty input
            continue;
        }


        // Validate input pointer before using it
        if (input == NULL) {
            fprintf(stderr, "Error: input buffer is NULL.\n");
            continue;
        }

        // Remove newline character if present
        if (nread > 0 && input[nread - 1] == '\n') {
            input[nread - 1] = '\0';
        }
        char* cleaned_query = query_clean(input);

        // coment out these lines for graceful memory free
        if (strcmp(cleaned_query, "exit") == 0) {
            free(cleaned_query);
            break;
        }

        int word_count = 0;
        char** words = validate(cleaned_query, &word_count);
        if (words == NULL) {
            free(cleaned_query);
            continue;
        }
        if (!operator_validate(words, word_count)) {
            free_memory(words, &word_count);
            free(cleaned_query);
            continue;
        }

        printf("Query: %s\n", cleaned_query);

        counters_t* result = execute_query(words, word_count, index);
        if (result == NULL) {
            printf("No documents match.\n");
            printf("-----------------------------------------------\n");
        } else {
            // Rank and display the results
            int num_docs = 0;
            doc_score_t* scores = rank_documents(result, &num_docs);
            display_output(scores, num_docs, page_directory);

            // Clean up ranking resources
            free(scores);
            printf("-----------------------------------------------\n");
        }

        counters_delete(result);
        free_memory(words, &word_count);
        free(cleaned_query);
    }

    free(input);
}



/************** counters_copy_helper ***************
 * Copies each (key, count) pair from one counters structure to another.
 *
 * Inputs:
 *   arg - destination counters structure (as void pointer)
 *   key - unique document ID
 *   count - count associated with the document ID
 */
void counters_copy_helper(void* arg, int key, int count)
{
    counters_t* temp = arg;
    counters_set(temp, key, count);
}

/************** counters_and_helper ***************
 * Helper function for AND operations on counters.
 * Updates the target counters structure with the minimum count for matching keys.
 *
 * Inputs:
 *   arg - structure containing temporary and current counters
 *   key - unique document ID
 *   count - count in the temporary counters structure
 */
void counters_and_helper(void* arg, const int key, const int count) {
    struct and_helper_args* args = arg;
    int curr_count = counters_get(args->curr, key);

    if (curr_count > 0) {
        counters_set(args->temp, key, (count < curr_count) ? count : curr_count);
        
    } else {
        counters_set(args->temp, key, 0);
    }
}

/************** counters_set_to_zero ***************
 * Sets all counts in a given counters structure to zero.
 *
 * Inputs:
 *   arg - counters structure to update
 *   key - unique document ID
 *   count - current count (unused)
 */
void counters_set_to_zero(void*arg, const int key, const int count){
    counters_set((counters_t*)arg, key,0);
}

/************** and_intersect_counters ***************
 * Performs an AND intersection on two counters structures.
 * Stores the minimum counts in the first counters structure.
 *
 * Inputs:
 *   temp - counters structure to store intersected counts
 *   curr - counters structure to intersect with `temp`
 */
void and_intersect_counters(counters_t* temp, counters_t* curr){
    if (curr==NULL){
        counters_iterate(temp, temp, counters_set_to_zero);
        return;
    }
    struct and_helper_args args = {temp, curr};
    counters_iterate(temp, &args, counters_and_helper);
}



/************** counters_or_helper ***************
 * Helper for OR operation; adds counts for matching keys across two counters.
 *
 * Inputs:
 *   arg - destination counters structure (as void pointer)
 *   key - unique document ID
 *   count - count from the source counters structure
 */
void counters_or_helper(void* arg, int key, int count) {
    counters_t* final = arg;  // Final counters as argument
    int final_count = counters_get(final, key);
    counters_set(final, key, final_count + count);
}

/************** or_merge_counters ***************
 * Merges two counters structures with an OR operation,
 * adding counts from the source to the destination structure.
 *
 * Inputs:
 *   final - counters structure to accumulate counts
 *   temp - source counters structure
 */
void or_merge_counters(counters_t* final, counters_t* temp) {

    if (temp != NULL) {
        counters_iterate(temp, final, counters_or_helper);
    }
}

/************** execute_query ***************
 * Executes a search query on the index using OR/AND logic.
 * Combines results in `final` counters for all valid words.
 *
 * Inputs:
 *   word - array of words from the query
 *   word_count - number of words in the query
 *   index - index structure for document search
 *
 * Returns:
 *   counters_t* - final counters with combined results for the query
 */
counters_t* execute_query(char** word, int word_count, index_t* index){
    struct qeryctrs ctrs;
    ctrs.final = counters_new();
    ctrs.temp = NULL;

    for (int i = 0; i < word_count; i++){
        if (strcmp(word[i], "or") == 0) {
            // Merge ctrs.temp into ctrs.final for OR
            or_merge_counters(ctrs.final, ctrs.temp);
            counters_delete(ctrs.temp);
            ctrs.temp = NULL;
        } 
        else if (strcmp(word[i], "and") != 0) {
            counters_t* curr = hashtable_find(index->ht, word[i]);

            if (ctrs.temp == NULL) {
                // Start new temp counters for the word
                ctrs.temp = counters_new();
                if (curr != NULL) {
                    counters_iterate(curr, ctrs.temp, counters_copy_helper);
                }
            } else {
                // Perform AND with current counters
                and_intersect_counters(ctrs.temp, curr);
            }
        }
    }

    // Final OR merge if temp has remaining values
    if (ctrs.temp != NULL) {
        or_merge_counters(ctrs.final, ctrs.temp);
        counters_delete(ctrs.temp);
    }

    return ctrs.final;
}


/************** compare_scores ***************
 * Compares two document scores, used for ranking results.
 *
 * Inputs:
 *   a - first document score
 *   b - second document score
 *
 * Returns:
 *   difference in scores for sorting in descending order
 */
int compare_scores(const void* a, const void* b) {
    const doc_score_t* score_a = (const doc_score_t*)a;
    const doc_score_t* score_b = (const doc_score_t*)b;
    return score_b->score - score_a->score;
}

/************** add_to_doc_score_array ***************
 * Helper to populate an array with document scores.
 *
 * Inputs:
 *   arg - struct holding score array and current index
 *   doc_id - document ID
 *   score - score for the document
 */
void add_to_doc_score_array(void* arg, const int doc_id, const int score) {
    struct score_helper_args* args = arg;
    args->scores[args->index].docID = doc_id;
    args->scores[args->index].score = score;
    args->index++;
}

/************** count_docs ***************
 * Counts documents with positive scores.
 *
 * Inputs:
 *   arg - pointer to document count
 *   key - document ID
 *   count - score for the document
 */
void count_docs(void* arg, const int key, const int count) {
    int* num_docs = arg;
    if (count > 0) {
        (*num_docs)++;
    }
}


/************** rank_documents ***************
 * Creates a ranked array of documents based on their scores.
 *
 * Inputs:
 *   final - counters structure with document scores
 *   num_docs - pointer to hold the number of ranked documents
 *
 * Returns:
 *   doc_score_t* - array of documents sorted by score, or NULL if empty
 */
doc_score_t* rank_documents(counters_t* final, int* num_docs) {
    *num_docs = 0;
    counters_iterate(final, num_docs, count_docs);

    if (*num_docs == 0) {
        return NULL; // no document found
    }

    doc_score_t* scores = malloc(*num_docs * sizeof(doc_score_t));

    if (scores == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for document scores.\n");
        return NULL;
    }

    // Reset an index for adding scores
    struct score_helper_args {
        doc_score_t* scores;
        int index;
    } args = {scores, 0};

    counters_iterate(final, &args, add_to_doc_score_array);

    qsort(scores, *num_docs, sizeof(doc_score_t), compare_scores);

    return scores;
}


/************** display_output ***************
 * Displays ranked query results with document scores and URLs.
 *
 * Inputs:
 *   scores - array of document scores
 *   num_docs - number of documents to display
 *   pagedir - directory path for document retrieval
 *
 * Returns:
 *   None; prints results to stdout
 */
void display_output(doc_score_t* scores, int num_docs, const char* pagedir) {
    printf("Matches %d documents (ranked):\n", num_docs);

    for (int i = 0; i < num_docs; i++) {
        int doc_id = scores[i].docID;
        int score = scores[i].score;

        char pathname[256];
        snprintf(pathname, sizeof(pathname), "%s/%d", pagedir, doc_id);

        FILE* file = fopen(pathname, "r");
        if (file != NULL) {
            char url[1024];  
            if (fgets(url, sizeof(url), file) != NULL) {  
                url[strcspn(url, "\n")] = '\0';
                printf("score\t%d doc\t%d: %s\n", score, doc_id, url);
            } else {
                fprintf(stderr, "Error: Unable to read URL from file %s\n", pathname);
            }
            fclose(file);
        } else {
            fprintf(stderr, "Error: Unable to open file %s\n", pathname);
        }
    }
    printf("-----------------------------------------------\n");
}

/*
 * pagedir.c - CS50 pagedir module
 *
 * Manzi Fabrice Niyigaba October 20 2024
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "../libcs50/webpage.h"
#include "pagedir.h"

/**************** function prototypes ****************/
bool pagedir_init(const char* pageDirectory);
bool save_webpage_dir(const webpage_t* page, const char* pageDirectory, const int docID);


/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool pagedir_init(const char* pageDirectory)
{
    int path_len = strlen(pageDirectory) + strlen("/.crawler") + 1;
    char* crawler_path = malloc(path_len);
    if (crawler_path == NULL) {
        fprintf(stderr, "Memory allocation failed for crawler_path.\n");
        return false;
    }
    snprintf(crawler_path, path_len, "%s/.crawler", pageDirectory);

    FILE* fp = fopen(crawler_path, "w");
    free(crawler_path);

    if (fp == NULL) {
        fprintf(stderr, "Failed to create .crawler file in directory: %s\n", pageDirectory);
        return false;
    }
    fclose(fp);
    return true;
}


/**************** save_webpage_dir() ****************/
/* see pagedir.h for description */
bool save_webpage_dir(const webpage_t* page, const char* pageDirectory, const int docID)
{
    // Get the length of docID as string
    int len = snprintf(NULL, 0, "%d", docID);
    char *strID = malloc(len + 1);
    snprintf(strID, len + 1, "%d", docID);

    // Calculate the size needed for the filename string
    int filename_len = strlen(pageDirectory) + strlen(strID) + 2; // pageDirectory + "/" + id + null terminator
    char *filename = malloc(filename_len * sizeof(char));
    assert(filename != NULL);

    // Construct the filename by concatenating the pageDirectory, "/", and strID
    snprintf(filename, filename_len, "%s/%s", pageDirectory, strID);

    // Open the file for writing
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        free(filename);
        free(strID);
        return false;
    }

    fprintf(fp, "%s\n", webpage_getURL(page));  // Save the URL
    fprintf(fp, "%d\n", webpage_getDepth(page));  // Save the depth
    fprintf(fp, "%s\n", webpage_getHTML(page));  // Save the HTML

    fclose(fp);  // Close the file

    free(filename);
    free(strID);
    return true;
}

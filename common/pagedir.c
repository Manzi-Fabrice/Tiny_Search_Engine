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
#include "../libcs50/file.h"
#include "pagedir.h"

/**************** function prototypes ****************/
bool pagedir_init(const char* pageDirectory);
bool save_webpage_dir(const webpage_t* page, const char* pageDirectory, const int docID);
char* get_pathname(const char* pageDirectory, const char* filename);
int pagedir_verify(const char* pageDirectory);
webpage_t* pagedir_load(const char* pathname);

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

/**************** get_pathname() ****************/
/* see pagedir.h for description */
char* get_pathname(const char* pageDirectory, const char* filename) {
    int length = strlen(pageDirectory) + strlen(filename) + 2; 
    char* pathname = malloc(length);
    if (pathname == NULL) {
        fprintf(stderr, "Failed to allocate memory for pathname\n");
        exit(1);
    }
    sprintf(pathname, "%s/%s", pageDirectory, filename);
    return pathname;
}

/**************** pagedir_verify() ****************/
/* see pagedir.h for description */
int pagedir_verify(const char* pageDirectory) {
    char* pathname = get_pathname(pageDirectory, ".crawler");
    FILE* file = fopen(pathname, "r");
    free(pathname);
    if (file == NULL) {
        return -1; // Indicate failure
    }
    fclose(file);
    return 0; 
}

/**************** pagedir_load() ****************/
/* see pagedir.h for description */
webpage_t* pagedir_load(const char* pathname) {
    FILE* fp = fopen(pathname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", pathname);
        return NULL;
    }

    // Read the URL line using file_readLine
    char* url = file_readLine(fp);
    if (url == NULL) {
        fprintf(stderr, "Failed to read URL from file: %s\n", pathname);
        fclose(fp);
        return NULL;
    }

    // Read the depth line
    char* depth_str = file_readLine(fp);
    if (depth_str == NULL) {
        fprintf(stderr, "Failed to read depth from file: %s\n", pathname);
        free(url);
        fclose(fp);
        return NULL;
    }
    //convert depth to integer
    int depth = atoi(depth_str);
    free(depth_str);

    // Read the HTML content using file_readFile
    char* html = file_readFile(fp);
    fclose(fp);  
    if (html == NULL) {
        fprintf(stderr, "Failed to read HTML content from file: %s\n", pathname);
        free(url);
        return NULL;
    }

    // Create a new webpage_t object
    webpage_t* page = webpage_new(url, depth, html);
    if (page == NULL) {
        fprintf(stderr, "Failed to create webpage object\n");
        free(url);
        free(html);
        return NULL;
    }
    return page;
}
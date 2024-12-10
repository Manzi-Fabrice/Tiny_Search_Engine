/*
 * pagedir.h - header file for CS50 pagedir module
 *
 * The pagedir module provides functions for initializing a directory
 * for crawler output and saving webpages to that directory in a format
 * suitable for later use by the indexer. It also includes functionality
 * for verifying and loading page files.
 *
 * Manzi Fabrice Niyigaba, October 2024
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/* Initialize the given directory for crawler output.
 *
 * Caller provides:
 *   a valid directory path where pages will be stored.
 * We do:
 *   attempt to create a file named ".crawler" in the directory
 *   to confirm that the directory is writable and valid.
 * We return:
 *   true if the directory is valid and writable, false otherwise.
 * Notes:
 *   If the directory is not writable or does not exist, the function
 *   will return false and print an error message.
 */
bool pagedir_init(const char* pageDirectory);

/**************** save_webpage_dir ****************/
/* Save the given webpage to the specified directory with a unique docID.
 *
 * Caller provides:
 *   a valid webpage pointer, a valid directory path, and a unique docID.
 * We do:
 *   create a file in the directory named after the docID and save the
 *   webpage's URL, depth, and HTML content to the file.
 * We return:
 *   true if the webpage was successfully saved, false otherwise.
 * Caller is responsible for:
 *   ensuring the directory exists and managing the webpage and docID.
 * Notes:
 *   The file will be named "pageDirectory/docID" where docID is an integer.
 *   If the file cannot be created, the function will return false and print
 *   an error message.
 */
bool save_webpage_dir(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** get_pathname ****************/
/* Constructs a full pathname for a given file in the specified directory.
 *
 * Caller provides:
 *   a valid page directory and a filename (typically a docID as a string).
 * We do:
 *   calculate the required memory for the pathname string, allocate it,
 *   and concatenate the directory and filename.
 * We return:
 *   the constructed pathname as a string, or NULL if memory allocation fails.
 * Caller is responsible for:
 *   freeing the returned pathname when done.
 * Notes:
 *   The returned pathname will be "pageDirectory/filename".
 *   If allocation fails, an error message is printed.
 */
char* get_pathname(const char* pageDirectory, const char* filename);

/**************** pagedir_verify ****************/
/* Verifies that the given directory is a valid crawler directory.
 *
 * Caller provides:
 *   a valid directory path where pages are expected to be stored.
 * We do:
 *   check if a ".crawler" file exists in the specified directory.
 * We return:
 *   0 if the directory contains the ".crawler" file, -1 otherwise.
 * Caller is responsible for:
 *   providing a valid directory path.
 * Notes:
 *   If the ".crawler" file does not exist, this function will return -1.
 */
int pagedir_verify(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Loads a webpage from the specified file path.
 *
 * Caller provides:
 *   a valid file pathname where the page is stored.
 * We do:
 *   open the file, read the URL, depth, and HTML content,
 *   and use them to construct a new webpage_t object.
 * We return:
 *   a pointer to the loaded webpage if successful, or NULL if any step fails.
 * Caller is responsible for:
 *   freeing the webpage_t object returned by this function.
 * Notes:
 *   If the file cannot be opened or the page content cannot be read, 
 *   an error message is printed and NULL is returned.
 */
webpage_t* pagedir_load(const char* pathname);

#endif // __PAGEDIR_H

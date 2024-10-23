/*
 * pagedir.h - header file for CS50 pagedir module
 *
 * The pagedir module provides functions for initializing a directory
 * for crawler output and saving webpages to that directory in a format
 * suitable for later use by the indexer.
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

#endif // __PAGEDIR_H

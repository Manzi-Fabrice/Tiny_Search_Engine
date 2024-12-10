// validate.h - header file for query validation functions in querier module

#ifndef VALIDATE_H
#define VALIDATE_H

#include <stdbool.h>

/*************** query_clean ***************
 * Cleans and normalizes the input query string.
 * Input:
 * query - the raw input query string.
 * Output:
 * A normalized version of the query string.
 */
char* query_clean(const char* query);

/*************** print_error ***************
 * Displays an error message to the user with optional detail.
 * Inputs:
 * message - the main error message.
 * detail - additional error details (optional).
 */
void print_error(const char* message, const char* detail);

/*************** validate ***************
 * Tokenizes, validates, and builds an array of valid words from the input query.
 * Inputs:
 * query - the input query string.
 * count - pointer to hold the count of valid words in the query.
 * Output:
 * An array of valid words from the query, or NULL on validation failure.
 */
char** validate(char* query, int* count);

/*************** operator_validate ***************
 * Ensures operators in the query are used correctly.
 * Inputs:
 * string_array - an array of query strings.
 * count - number of elements in the array.
 * Output:
 * Returns true if operators are used correctly, otherwise false.
 */
bool operator_validate(char** string_array, int count);

/*************** free_memory ***************
 * Frees memory allocated for the array of words.
 * Inputs:
 * result - array of words to free.
 * count - pointer to the count of words in the array.
 */
void free_memory(char** result, int* count);

#endif // VALIDATE_H

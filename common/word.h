/*
 * word.h - header file for CS50 TSE Word module
 *
 * The word module provides a function to normalize words by converting
 * them to lowercase, making word comparisons case-insensitive.
 *
 * Manzi Fabrice Niyigaba, October 2024
 */

#ifndef __WORD_H
#define __WORD_H

#include <stddef.h>

/**************** functions ****************/

/**************** normalize ****************/
/* Normalizes a word by converting all uppercase letters to lowercase.
 *
 * Caller provides:
 *   a valid string representing the word to be normalized.
 * We return:
 *   a newly allocated string containing the normalized word,
 *   or NULL if memory allocation fails.
 * Caller is responsible for:
 *   freeing the returned string when done.
 * Notes:
 *   If the input word is already in lowercase, the returned string
 *   will be identical to the input, except it will be separately allocated.
 */
char* normalize(const char* word);

#endif // __WORD_H

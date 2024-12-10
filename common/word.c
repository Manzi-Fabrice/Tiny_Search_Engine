/*
 * word.c - CS50 word module
 *
 * Manzi Fabrice Niyigaba October 20 2024
 */
#include <ctype.h>
#include "word.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// Function prototype
char* normalize(const char* word);



/**************** normalize() ****************/
/* see word.h for description */
char* normalize(const char* word){
    int length = strlen(word);
    char* normalized = malloc(length + 1);
    if (normalized == NULL) {
        fprintf(stderr, "Failed to allocate memory for normalization\n");
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        normalized[i] = tolower((unsigned char)word[i]);
    }
    normalized[length] = '\0';
    return normalized;
}

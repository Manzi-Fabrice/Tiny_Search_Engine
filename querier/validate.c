/* 
 * validate.c - query validation for 'querier' module
 *
 * Provides functions to clean, validate, and tokenize user queries for the querier.
 * Works with querier to ensure queries follow expected input format and operator usage.
 * See validate.h for declarations and DESIGN.md for integration details.
 *
 * Manzi Fabrice Niyigaba, CS50, November 2024
 */

# include<stdio.h>
# include<stdlib.h>
# include <string.h>
# include <ctype.h>
# include <stdbool.h>
# include "validate.h"
# include "word.h"


/*************** print_error ***************/
// see validate.h for more information
void print_error(const char* message, const char* detail) {
    if (detail != NULL) {
        printf("Error: %s %s\n", message, detail);
    } else {
        printf("Error: %s\n", message);
    }
}

/*************** query_clean ***************/
// see validate.h for more information
char* query_clean(const char* query) {
    return normalize(query);
}

/*************** free_memory ***************/
// see validate.h for more information
void free_memory(char** result, int* count) {
    for (int i = 0; i < *count; i++) {
        free(result[i]);
    }
    free(result);
}

/*************** validate ***************/
// see validate.h for more information
char** validate(char* query, int* count)
{
    const char* operant_1 = "or";
    char** result = malloc(strlen(query) * sizeof(char*));
    if (result == NULL) {
        print_error("failed to allocate memory", NULL);
        return NULL;
    }

    *count = 0;
    int i = 0, start = 0;
    while (query[i] != '\0') {
        if (!isalpha(query[i]) && !isspace(query[i])) {
            printf("Error: bad character '%c' in query.\n", query[i]);
            free_memory(result, count);
            return NULL;
        }

        if (query[i] == ' ') {
            if (start != i) {
                int length = i - start;
                char* word = malloc((length + 1) * sizeof(char));
                if (word == NULL) {
                    print_error("failed to allocate memory", NULL);
                    free_memory(result, count);
                    return NULL;
                }
                strncpy(word, query + start, length);
                word[length] = '\0';

                if (length < 3 && strcmp(word, operant_1) != 0) {
                    char message[100];
                    snprintf(message, sizeof(message), "'%s' is an invalid word", word);
                    print_error(message, NULL);
                    free(word);
                    free_memory(result, count);
                    return NULL;
                }
                result[*count] = word;
                (*count)++;
            }
            start = i + 1;
        }
        i++;
    }

    if (start < i) {
        int length = i - start;
        char* word = malloc((length + 1) * sizeof(char));
        if (word == NULL) {
            print_error("failed to allocate memory for the last word", NULL);
            free_memory(result, count);
            return NULL;
        }
        strncpy(word, query + start, length);
        word[length] = '\0';

        if (length < 3 && strcmp(word, operant_1) != 0) {
            char message[100];
            snprintf(message, sizeof(message), "'%s' is an invalid word", word);
            print_error(message, NULL);
            free(word);
            free_memory(result, count);
            return NULL;
        }
        result[*count] = word;
        (*count)++;
    }
    char** temp = realloc(result, (*count + 1) * sizeof(char*));
    if (temp == NULL) {
        print_error("failed to allocate memory", NULL);
        free_memory(result, count);
        return NULL;
    }
    result = temp;
    return result;
}


/*************** operator_validate ***************/
// see validate.h for more information
bool operator_validate(char** string_array, int count) {
    const char* operators[] = {"and", "or"};

    if (strcmp(string_array[0], operators[0]) == 0 || strcmp(string_array[0], operators[1]) == 0) {
        char message[100];
        snprintf(message, sizeof(message), "'%s' cannot be first", string_array[0]);
        print_error(message, NULL);
        return false;
    }

    if (strcmp(string_array[count - 1], operators[0]) == 0 || strcmp(string_array[count - 1], operators[1]) == 0) {
        char message[100];
        snprintf(message, sizeof(message), "'%s' cannot be last", string_array[count - 1]);
        print_error(message, NULL);
        return false;
    }

    for (int i = 1; i < count - 1; i++) {
        if (strcmp(string_array[i], operators[0]) == 0 || strcmp(string_array[i], operators[1]) == 0) {
            if (strcmp(string_array[i + 1], operators[0]) == 0 || strcmp(string_array[i + 1], operators[1]) == 0) {
                char message[100];
                snprintf(message, sizeof(message), "Consecutive operators '%s' and '%s' found", string_array[i], string_array[i + 1]);
                print_error(message, NULL);
                return false;
            }
        }
    }
    return true;
}

/*
 * crawler.c - CS50 crawler module
 *
 * Manzi Fabrice Niyigaba October 20 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
# include "crawler.h"

/**************** local functions ****************/
// not visible outside this function 
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** other functions ****************/
void crawl(char *seedURL, char* pageDirectory, int maxDepth);


/**************** main() ****************/
int main(const int argc, char* argv[]) 
{
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;

    // Parse command-line arguments
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Start crawling
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

/**************** parseArgs() ****************/
/* see crawler.h for description */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
    if (argc != 4) {
        fprintf(stderr, "invalid numnber of inputs\n");
        exit(1);
    }
    // Validate pageDirectory by initializing it
    if (!pagedir_init(argv[2])) {
        fprintf(stderr, "Invalid pageDirectory: %s\n", argv[2]);
        exit(2);
    }

    if ((atoi(argv[3]))<0 || (atoi(argv[3]))>10){
        fprintf(stderr, "invalid depth");
        exit(3);
    }

 
    if (!isInternalURL(argv[1])) {
        fprintf(stderr, "Invalid seedURL: %s\n", argv[1]);
        exit(4);
    }


    *seedURL = argv[1];  
    *pageDirectory = argv[2];
    *maxDepth = atoi(argv[3]);
}



/**************** crawl() ****************/
/* see crawler.h for description */
void crawl(char *seedURL, char *pageDirectory, int maxDepth)
{
    int id = 0;
    bag_t *pagesToCrawl = bag_new();
    hashtable_t *pagesSeen = hashtable_new(1000);

    // Dynamically allocate memory for seedURL
    char* seedURLCopy = malloc(strlen(seedURL) + 1);
    if (seedURLCopy == NULL) {
        fprintf(stderr, "Memory allocation failed for seedURL\n");
        exit(1);
    }
    strcpy(seedURLCopy, seedURL);

    // Add the seed URL to the hashtable and the bag
    hashtable_insert(pagesSeen, seedURLCopy, "");
    webpage_t *seed_page = webpage_new(seedURLCopy, 0, NULL);
    bag_insert(pagesToCrawl, seed_page);

    webpage_t *current_page;

    while ((current_page = bag_extract(pagesToCrawl)) != NULL) {
        if (webpage_fetch(current_page)) {
            id++;
            save_webpage_dir(current_page, pageDirectory, id);

            if (webpage_getDepth(current_page) < maxDepth) {
                pageScan(current_page, pagesToCrawl, pagesSeen);
            }
        } else {
            fprintf(stderr, "Failed to fetch the webpage: %s\n", webpage_getURL(current_page));
        }
        webpage_delete(current_page);
    }

    // Clean up
    hashtable_delete(pagesSeen, NULL);
    bag_delete(pagesToCrawl, NULL);
}


/**************** pageScan() ****************/
/* see crawler.h for description */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen)
{
    int pos = 0;
    char *next_url;

    while ((next_url = webpage_getNextURL(page, &pos)) != NULL) {
        char *normalizedURL = normalizeURL(next_url);
        bool exist = hashtable_find(pagesSeen, normalizedURL);
        bool valid_url = isInternalURL(normalizedURL);

        if (!exist && valid_url && normalizedURL) {
            if (hashtable_insert(pagesSeen, normalizedURL, "")) {
                char* url = malloc(strlen(normalizedURL) + 1);
                if (url != NULL) {
                    strcpy(url, normalizedURL);
                    webpage_t* newPage = webpage_new(url, webpage_getDepth(page) + 1, NULL);
                    bag_insert(pagesToCrawl, newPage);
                }
            }
        } else {
            free(normalizedURL);
        }
    }
}
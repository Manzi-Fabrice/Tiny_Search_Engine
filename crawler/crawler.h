/*
 * crawler.h - header file for CS50 Crawler module
 *
 * The crawler is responsible for starting at a given seed URL,
 * crawling the web up to a specified depth, and saving web pages
 * to a designated directory. It uses a bag to store pages yet to
 * be crawled and a hashtable to track pages that have already been seen.
 *
 * Manzi Fabrice Niyigaba, October 2024
 */

#ifndef __CRAWLER_H
#define __CRAWLER_H

#include <stdbool.h>

/**************** global types ****************/

/**************** functions ****************/

/**************** parseArgs ****************/
/* Parse the command-line arguments for the crawler.
 * 
 * Caller provides:
 *   the number of command-line arguments (argc),
 *   the array of argument strings (argv),
 *   pointers to store the seed URL, page directory, and max depth.
 * We do:
 *   validate the number of arguments and the validity of the seed URL
 *   (ensuring it is an internal URL and properly normalized),
 *   ensure the page directory can be written to,
 *   and verify that the max depth is within allowed limits.
 * We guarantee:
 *   The seed URL is valid, and max depth is a valid integer between 0 and 10.
 * Caller is responsible for:
 *   passing valid pointers and managing memory for the URL and directory.
 * Notes:
 *   If any argument is invalid, the function will exit with an error message.
 */
static void parseArgs(const int argc, char* argv[],
               char** seedURL, char** pageDirectory, int* maxDepth);

/**************** crawl ****************/
/* Start crawling from the seed URL, visiting pages and saving them to disk.
 * 
 * Caller provides:
 *   a seed URL, a page directory where pages will be saved,
 *   and the maximum crawl depth (0 to 10).
 * We do:
 *   initialize a bag for pages yet to be crawled and a hashtable to track
 *   pages that have already been seen. Begin with the seed URL at depth 0.
 *   For each page fetched, save it to the directory and scan for new URLs
 *   to continue crawling, up to the maximum depth.
 * We guarantee:
 *   All pages up to the max depth are fetched and saved.
 * Caller is responsible for:
 *   ensuring the seed URL and directory are valid.
 */
void crawl(char* seedURL, char* pageDirectory, int maxDepth);

/**************** pageScan ****************/
/* Scan a webpage for URLs and add valid ones to the bag of pages to crawl.
 * 
 * Caller provides:
 *   a webpage (page), a bag for pages yet to be crawled (pagesToCrawl),
 *   and a hashtable of pages already seen (pagesSeen).
 * We do:
 *   extract URLs from the webpage, normalize them, and add each new,
 *   internal URL to the bag and hashtable. Each newly discovered page
 *   is added with its depth incremented by one.
 * We guarantee:
 *   No duplicate pages are added to the bag or hashtable.
 * Caller is responsible for:
 *   passing valid pointers for the webpage, bag, and hashtable.
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

#endif // __CRAWLER_H

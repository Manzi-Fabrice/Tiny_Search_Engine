# CS50 Crawler Module

**Author:** Manzi Fabrice Niyigaba  
**Date:** October 20, 2024

## Overview

The `crawler` module recursively fetches web pages starting from a given **seed URL** up to a specified **depth**, storing each page in a given **directory**. It uses data structures from the CS50 library, such as `bag` and `hashtable`, to manage the pages to be crawled and to track visited pages.

## Assumptions

1. The seed URL must be valid and internal.
2. The page directory must exist and be writable.
3. The max depth must be between 0 and 10.
4. Pages are fetched successfully using `webpage_fetch()`.
5. Duplicate URLs are not fetched more than once.
6. Memory is managed correctly and efficiently.
7. The crawler will handle broken links gracefully and continue with the next page.

## Potential Issues and Testing

### 1. **Invalid Seed URL**
   - If the seed URL is invalid or external, the program will terminate with an error message.

### 2. **Invalid Page Directory**
   - The program expects a valid, writable directory; otherwise, it will exit with an error.

### 3. **Max Depth Exceeds Limit**
   - Depth values outside the range of 0 to 10 will cause the program to terminate with an error.

### 4. **Memory Management**
   - Memory allocation failures may cause the program to crash, particularly if crawling many pages.

### 5. **Duplicate Pages**
   - Duplicate URLs are handled by the hashtable, ensuring that pages are only fetched once.

### 6. **Broken Links**
   - The crawler will log failures to fetch specific pages but continue with other available URLs.

### 7. **Non-HTML Content**
   - The crawler is not optimized for handling non-HTML content (e.g., images or PDFs), which may lead to unexpected behavior.

## Testing

The program is tested using `testing.sh`, and results are logged in `testing.out`. Example commands:
```bash
make test
make valgrind
```

Valgrind is used to check for memory leaks:
```bash
make valgrind
```


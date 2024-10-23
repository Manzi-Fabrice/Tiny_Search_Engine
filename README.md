# CS50, Tiny Search Engine (TSE)
## Manzi Fabrice Niyigaba 
### Date: October 2024

> Github-username: Manzi-Niyigaba 

## Overview

This Tiny Search Engine (TSE) consists of three subsystems that work together to crawl websites, index the content, and allow users to search through the collected data. Each subsystem is a standalone program executed from the command line, and they interact through files stored in the file system.

### 1. **Crawler**
   The crawler starts with a specified seed URL and retrieves webpages by parsing the initial webpage, extracting embedded URLs, and crawling those pages. It continues this process within a specified depth, ensuring that each URL is only visited once. The crawler saves the content of each webpage, along with its URL and depth, in files. Once the crawling process is complete, the indexed documents are ready for the next phase.

>For more detailed information checkout crawler/README.md

### 2. **Indexer**
   The indexer takes the stored webpages and extracts all keywords from them. It then builds a lookup table (index) that maps each keyword to the documents (webpages) where the word appears. This index is saved to a file for efficient querying in the next phase.

>For more detailed information checkout indexer/README.md

### 3. **Query Engine**
   The query engine allows users to search through the indexed data. It loads the index file and searches for pages containing the requested keywords. The results are ranked based on criteria like the frequency of keywords within each page, and the results are presented to the user.

>For more detailed information checkout querier/README.md

## Subsystems Interaction

The subsystems inter-connect through files:
- The **crawler** generates a directory of webpages that the **indexer** processes.
- The **indexer** saves an index file that the **query engine** loads to respond to user queries.

Each subsystem can be run independently but is crucial for the overall functionality of the search engine.


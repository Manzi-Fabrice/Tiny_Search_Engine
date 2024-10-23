# Manzi Fabrice Niyigaba 
## COSC 50 Fall 2024, TSE
### Common Directory
***



**The common directory contains shared utility functions and resources used across different modules of the project, primarily focused on handling web page directories and storing crawled data. The components in this directory are designed to ensure consistency and reusability in how pages are saved and managed throughout the system. **

##### Components 
***

 1. **pagedir.c: **This file contains the implementation of functions that help initialize directories for storing web pages and save crawled web pages in a specific format. 
 
 2. **pagedir.h:** This header file provides the function declarations for pagedir.c, 
  
 3. **Makefile:** The Makefile is responsible for compiling the pagedir.c file into an object file and bundling it into a library that can be used by other modules.
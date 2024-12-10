# Manzi Fabrice Niyigaba
## COSC 50 Fall 2024, TSE
### Common Directory
***

**The common directory contains shared utility functions and resources used across different modules of the TSE (Tiny Search Engine) project. These utilities support consistent handling of web page directories, indexing, word normalization, and more, ensuring reusability and modularity across the project.**

##### Components
***

1. **pagedir:** Provides functions to initialize and manage directories for storing crawled web pages. For details on its functions, please refer to `pagedir.h`.

2. **index:** Provides functionality to create, save, and manage an in-memory index structure, which stores word occurrences by document. For further details, please refer to `index.h`.

3. **word:** Provides a function to normalize words by converting them to lowercase, enabling case-insensitive word handling across the project. For further details, see `word.h`.

4. **Makefile:** Compiles the `pagedir.c`, `index.c`, and `word.c` source files into object files and bundles them into a library that can be linked with other modules.

***

This setup ensures a structured approach, where each component serves a specific purpose, and individual functions are detailed in their respective header files.

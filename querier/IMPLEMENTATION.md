
### Querier Implementation Overview

The querier reads and validates user queries, processes them using an index, and outputs ranked documents matching the query criteria.

#### Key Design Choices
1. **Modular Validation with `validate.c`:**
   The `validate.c` module handles query validation. This makes it easier to modify validation rules, such as changing delimiters, without altering core query processing logic.This design choice enhances modularity and supports future customizations.


2. **Robust Input Handling with `getline()`:**
   By using `getline()` (enabled by `_GNU_SOURCE`), the querier can safely handle large inputs without risking overflow, as it doesn’t assume fixed input lengths. This flexibility makes the querier robust under stress tests.

3. **Efficient Data Management with Structs:**
   Specialized structures (`qeryctrs`, `doc_score_t`, and `and_helper_args`) streamline data flow and memory management. Each struct supports specific tasks, aiding complex query handling and optimizing document processing.

#### Implementation Details
- **Query Parsing and Validation:**  
  Queries are cleaned, parsed, and validated for correct syntax, including boolean operators (AND, OR) and invalid characters.
  
- **AND/OR Logic Handling:**  
  AND sequences are processed in `temp` counters, while OR results accumulate in `final` counters, enabling efficient query handling. Helper functions (`counters_and_helper`, `counters_or_helper`) support intersections and unions.

- **Document Ranking and Display:**  
  Documents are ranked in descending score order using `qsort` in `rank_documents`. Each document’s URL is retrieved from `pageDirectory` and displayed with its score and ID.




#### Testing Plan

**Automated Testing:**
Run make test to execute predefined tests in testing.sh, with results saved in test_output.out. 
testing.sh try different tests to verify the functionality of the program. It also uses fuzzquery.c to generate random words and it proves to work according to the design spec. 

> NOTE: The program outputs "Query?" as noted in the test_output.out twice due to the querier’s prompt and immediate display of results; However this does not affect functionality.

**Interactive Testing:**
To run the querier interactively:

Copy code
```bash
./querier <page_directory> <index_filename>
```

Enter search terms interactively, pressing Ctrl+D to exit. Example paths:

PAGE_DIRECTORY="../test_data/test_depth_2"
INDEX_FILENAME="../test_data/test_index_2"
Ensure the test files exist by copying indexer outputs from the CS50 shared directory:

```bash
cp ~/cs50-dev/shared/tse/output/letters-2 ../test_data/test_depth_2
cp ~/cs50-dev/shared/tse/output/letters-2.index ../test_data/test_index_2
```
Memory Testing with Valgrind:
Run `make valgrind` to ensure no memory leaks.
# Design Specification for querier
## Abstract Data Structures

To ensure modularity and clarity, I implemented key data structures that manage different aspects of query processing, scoring, and document ranking.

### **struct qeryctrs**
This structure manages two counters_t types:
final to accumulate results for the entire query.
temp to store intermediate counters for logical steps.
By having dedicated counters for intermediate and cumulative results, qeryctrs enables the code to handle both AND and OR logic efficiently within the same structure.

### **doc_score_t**
This structure holds each document’s unique ID (docID) and calculated score, allowing the program to track and rank each document by relevance.
Storing document scores separately allows for easier sorting and scoring while keeping the primary query processing logic modular.

### **Helper Structures for Processing**

**and_helper_args:** Used specifically for AND logic, managing intermediate counters.

**score_helper_args:** Manages an array of document scores (doc_score_t) and tracks the current index, allowing the program to populate scores for each document efficiently.


## **Pseudocode Outline**
### **Main Program Flow**

**Argument Validation** using validate.c see validate.h for more information

**Index Loading from indexFilename** into an index_t structure to enable rapid access to word frequencies across documents.

**Query Processing Loop:**
Continuously read user queries from stdin with getline() 

**For each query:**

Clean and validate input
Ensure logical operators (AND, OR) are correctly placed within the query syntax.
Print the parsed query for user reference.

**Query Execution:**
Separate each query into AND and OR sequences.
Use temp for intermediate results in AND logic and merge temp into final counters for OR sequences.

**Scoring and Ranking:**
Calculate scores by intersecting counters for AND logic (minimum count) and summing scores for OR logic.
Rank documents based on score using qsort() for efficient ordering.

**Output Results:**
Display ranked documents, showing the score, document ID, and URL (retrieved from pageDirectory).
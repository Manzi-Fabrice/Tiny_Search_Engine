# Querier 
## CS50 Fall 2025, Dartmouth College
### Manzi Fabrice Niyigaba
---
The querier module processes search queries to retrieve relevant documents from an indexed dataset, adhering closely to the CS50 design specification. 

It uses the validate.c module to ensure that queries are properly formatted before processing, enhancing modularity and code separation. For further insights into this design decision, see IMPLEMENTATION.md.

To handle complex queries efficiently, querier uses specialized structs and employs getline() (from _GNU_SOURCE) to accommodate large inputs, minimizing potential stack overflow risks during stress testing. 

For display purposes, it assumes pathnames are limited to 256 characters and URLs to 1024 characters—parameters that are practical for most real-world applications.

> For further information on the design and implementation details, see DESIGN.md and IMPLEMENTATION.md.

To make sure that we trust the validity of the input I decided to use the input provided in cs50's shared directory.

Please refer to IMPLEMENTATION.md on how to make copy of the input before you test the program.
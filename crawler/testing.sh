#!/bin/bash
# Full testing script for CS50 Crawler module
# Manzi Fabrice Niyigaba, October 2024
# This script tests valid and invalid inputs for the crawler module.

# Create directories for storing crawled pages
mkdir -p ../data/letters
mkdir -p ../data/wikipedia

# Test 1: Testing with out-of-range depth (Greater than 10: 15)
echo "### Testing with out-of-range depth: Greater than 10 (15) ###"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 15
if [ $? -ne 0 ]; then
    echo "Depth out of range: Greater than 10"
fi

echo""

# Test 2: Testing with out-of-range depth (Less than 0: -10)
echo "### Testing with out-of-range depth: Less than 0 (-10) ###"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters -10
if [ $? -ne 0 ]; then
    echo "Depth out of range: Less than 0"
fi

echo""

# Test 3: Testing with an unavailable directory
echo "### Testing with unavailable directory ###"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/nonexistent_directory 2
if [ $? -ne 0 ]; then
    echo "The directory not found"
fi

echo""

# Test 4: Testing with invalid link
echo "### Testing with invalid link ###"
./crawler http:/google.com.html ../data/letters 2
if [ $? -ne 0 ]; then
    echo "We can't crawl the website due to an invalid link"
fi

echo""

# Test 5: Testing with too many arguments
echo "### Testing with too many arguments ###"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Linked_list.html ../data/letters 9 12 23
if [ $? -ne 0 ]; then
    echo "Too many arguments were provided"
fi


echo""

# Now proceed to valid tests

# Test 6: Run the crawler for the letters website
echo "### Crawling the letters website at depth 3 ###"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 3

# Count and display the number of files found in the data/letters directory
num_letters_files=$(ls ../data/letters | wc -l)
echo "Total number of files found in ../data/letters after crawling at depth 3: $num_letters_files"

echo""

# Test 7: Run the crawler for the wikipedia Linked_list page
echo "### Crawling the wikipedia Linked_list page at depth 1 ###"
echo "The wikipedia contains more urls for convinience we crawl only till depth 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Linked_list.html ../data/wikipedia 1


echo""

# Count and display the number of files found in the data/wikipedia directory
num_wikipedia_files=$(ls ../data/wikipedia | wc -l)
echo "Total number of files found in ../data/wikipedia after crawling at depth 1: $num_wikipedia_files"

echo""

# Final directory check with summaries
echo "### Final summary ###"
echo "Total number of files in ../data/letters: $num_letters_files"
echo "Total number of files in ../data/wikipedia: $num_wikipedia_files"

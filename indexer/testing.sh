#!/bin/bash
# Simple testing script for the CS50 TSE Indexer module
# [Your Name], [Date]
# This script tests valid and invalid inputs for the indexer module.

# Remove any existing testing.out and indexer.out files
rm -f testing.out indexer.out


# Test 1: No arguments
echo "### Test 1: No arguments ###" >> testing.out
./indexer >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled no arguments" >> testing.out
else
    echo "Indexer failed to handle no arguments" >> testing.out
fi
echo "" >> testing.out

# Test 2: One argument
echo "### Test 2: One argument ###" >> testing.out
./indexer ../data/letters >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled one argument" >> testing.out
else
    echo "Indexer failed to handle one argument" >> testing.out
fi
echo "" >> testing.out

# Test 3: Three or more arguments
echo "### Test 3: Three or more arguments ###" >> testing.out
./indexer ../data/letters ../data/letters.index extra_arg >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled too many arguments" >> testing.out
else
    echo "Indexer failed to handle too many arguments" >> testing.out
fi
echo "" >> testing.out

# Test 4: Invalid pageDirectory (non-existent path)
echo "### Test 4: Invalid pageDirectory (non-existent path) ###" >> testing.out
./indexer ../data/nonexistent_directory ../data/index.index >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled non-existent pageDirectory" >> testing.out
else
    echo "Indexer failed to handle non-existent pageDirectory" >> testing.out
fi
echo "" >> testing.out

# Test 5: Invalid pageDirectory (not a crawler directory)
echo "### Test 5: Invalid pageDirectory (not a crawler directory) ###" >> testing.out
mkdir -p ../data/emptydir
./indexer ../data/emptydir ../data/index.index >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled invalid pageDirectory (not a crawler directory)" >> testing.out
else
    echo "Indexer failed to handle invalid pageDirectory (not a crawler directory)" >> testing.out
fi
echo "" >> testing.out

# Test 6: Invalid indexFilename (non-existent path)
echo "### Test 6: Invalid indexFilename (non-existent path) ###" >> testing.out
./indexer ../data/letters /nonexistent_path/index.index >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled invalid indexFilename (non-existent path)" >> testing.out
else
    echo "Indexer failed to handle invalid indexFilename (non-existent path)" >> testing.out
fi
echo "" >> testing.out

# Test 7: Invalid indexFilename (read-only directory)
echo "### Test 7: Invalid indexFilename (read-only directory) ###" >> testing.out
mkdir -p ../data/readonlydir
chmod -w ../data/readonlydir
./indexer ../data/letters ../data/readonlydir/index.index >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled indexFilename in read-only directory" >> testing.out
else
    echo "Indexer failed to handle indexFilename in read-only directory" >> testing.out
fi
chmod +w ../data/readonlydir  # Reset permissions
echo "" >> testing.out

# Test 8: Invalid indexFilename (existing, read-only file)
echo "### Test 8: Invalid indexFilename (existing, read-only file) ###" >> testing.out
touch ../data/readonly.index
chmod -w ../data/readonly.index
./indexer ../data/letters ../data/readonly.index >> testing.out 2>&1
if [ $? -ne 0 ]; then
    echo "Indexer correctly handled existing, read-only index file" >> testing.out
else
    echo "Indexer failed to handle existing, read-only index file" >> testing.out
fi
chmod +w ../data/readonly.index  # Reset permissions
rm ../data/readonly.index  # Clean up
echo "" >> testing.out

# Test 9: Valid indexing of letters directory
echo "Running valid indexing test on letters directory..."
./indexer ../data/letters ../data/letters.index
if [ $? -eq 0 ]; then
    echo "Indexer successfully indexed letters directory"
else
    echo "Indexer failed to index letters directory"
fi
echo ""

# Write only the contents of the index file to indexer.out
cat ../data/letters.index > indexer.out

echo "Testing complete."

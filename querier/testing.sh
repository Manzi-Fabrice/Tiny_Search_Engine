#!/bin/bash
# setup_fuzz_testing.sh - Automated test script for querier program
#
# Usage: ./setup_fuzz_testing.sh
#
# This script:
#   - Verifies the existence of required files (querier, page directory, index file, fuzzquery)
#   - Tests querier with basic parameter validation and functional queries
#   - Runs fuzz testing with random queries
#
# Results are saved to 'test_output.out'.
#
# Manzi Fabrice Niyigaba, November 04


# Define the location of the querier executable, page directory, and updated index file path
QUERIER_EXEC="./querier"
PAGE_DIRECTORY="../test_data/test_depth_2"
INDEX_FILENAME="../test_data/test_index_2"
OUTPUT_FILE="test_output.out"
FUZZQUERY_EXEC="./fuzzquery"

# Clear or create the output file
> "$OUTPUT_FILE"

# Function to log all messages only to output file
log() {
    echo "$1" >> "$OUTPUT_FILE"
}

log "=========================================================="
log "Testing querier parameter validation"

# Test 1: Too few arguments
log "Test 1: Running querier with too few arguments"
$QUERIER_EXEC "$PAGE_DIRECTORY" >> "$OUTPUT_FILE" 2>&1
if [ $? -ne 0 ]; then
    log " : Test 1 Passed"
else
    log " : Test 1 Failed "
fi

# Test 2: Too many arguments
log "Test 2: Running querier with too many arguments"
$QUERIER_EXEC "$PAGE_DIRECTORY" "$INDEX_FILENAME" "extra_argument" >> "$OUTPUT_FILE" 2>&1
if [ $? -ne 0 ]; then
    log " : Test 2 Passed"
else
    log " : Test 2 Failed"
fi

log "=========================================================="
log "Testing querier basic functionality"

# Test 3: Single-word query 'home'
log "Test 3: Single-word query 'home'"
echo "home" | $QUERIER_EXEC "$PAGE_DIRECTORY" "$INDEX_FILENAME" >> "$OUTPUT_FILE" 2>&1
if [ $? -eq 0 ]; then
    log "Test 3 Passed: Query 'home' executed successfully"
else
    log "Test 3 Failed: Query 'home' encountered an error"
fi

# Test 4: AND conjunction
log "Test 4: Query 'home AND back'"
echo "home AND back" | $QUERIER_EXEC "$PAGE_DIRECTORY" "$INDEX_FILENAME" >> "$OUTPUT_FILE" 2>&1
if [ $? -eq 0 ]; then
    log "Test 4 Passed: Query 'home AND back' executed successfully"
else
    log "Test 4 Failed: Query 'home AND back' encountered an error"
fi

# Additional tests can be continued here in the same manner...

log "=========================================================="
log "Basic Fuzz Testing with fuzzquery"

# Fuzz Testing: Random queries
if [ -f "$FUZZQUERY_EXEC" ]; then
    log "Running fuzz tests with 5 random queries..."
    $FUZZQUERY_EXEC "$INDEX_FILENAME" 5 3 | $QUERIER_EXEC "$PAGE_DIRECTORY" "$INDEX_FILENAME" >> "$OUTPUT_FILE" 2>&1
    if [ $? -eq 0 ]; then
        log "Fuzz Test Passed: Querier handled fuzz queries successfully"
    else
        log "Fuzz Test Failed: Querier encountered errors during fuzz testing"
    fi
else
    log "Fuzzquery executable not found. Skipping fuzz tests."
fi



log "=========================================================="
log "Testing complete. Review results in $OUTPUT_FILE."

#!/bin/bash
set -e

# This script makes travis exit a job when no changes were detected in the commit that triggered the job.
# 
# Example usage:
# ./check-changes.sh path/to/dir

# 1. Get all the arguments of the script
# https://unix.stackexchange.com/a/197794
PATH_TO_CHECK="$*"

# 2. Make sure the paths to search are not empty
if [ -z "$PATH_TO_CHECK" ]; then
    echo "Please provide the paths to check for changes."
    echo "Example usage:"
    echo "./check-changes.sh path/to/dir"
    exit 1
fi

cd "$PATH_TO_CHECK"

CHANGED_FILES=$(git diff --name-status HEAD~1...HEAD .)
if [ -z "$CHANGED_FILES" ]
then
    # nothing changed, skip building
    echo "No changes in $PATH_TO_CHECK, terminate"
   
    # this indicates to the parent script that the build can be terminated
    exit 137
fi

echo "There were changes in $PATH_TO_CHECK changes: $CHANGED_FILES"
exit 0
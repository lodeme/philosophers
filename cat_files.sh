#!/bin/bash

# Define the output file
output_file="all_files_content.txt"

# Check if output file already exists and remove it to start fresh
if [ -f "$output_file" ]; then
    rm "$output_file"
fi

# Concatenate all text from files in includes and src directories
find includes src -type f -name '*.c' -or -name '*.h' | while read file; do
    echo "Processing $file"
    cat "$file" >> "$output_file"
    # Optional: Add a separator between files
    echo -e "\n\n" >> "$output_file"
done

echo "All contents have been concatenated into $output_file"

#!/bin/bash

# Declare variables for the old and new version
old_version="v0_4"
new_version="v0_5"

# File paths
files=("include/simple_enum/simple_enum.hpp" "include/simple_enum/ranges_views.hpp")

# Loop through each file path
for file_path in "${files[@]}"; do
    # Check if the file exists
    if [ -f "$file_path" ]; then
        # Use sed to replace the old version with the new version in the file
        sed -i "s/simple_enum::inline $old_version/simple_enum::inline $new_version/g" "$file_path"
        echo "Version updated from $old_version to $new_version in $file_path."
    else
        echo "File $file_path does not exist."
    fi
done

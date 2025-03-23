#!/bin/bash

for file in *; do
    if [ -f "$file" ]; then
        new_name=$(echo "$file" | tr 'A-Z' 'a-z')
        if [ "$file" != "$new_name" ]; then
            mv "./$file" "./$new_name"
            echo "Renamed '$file' to '$new_name'"
        fi
    fi
done


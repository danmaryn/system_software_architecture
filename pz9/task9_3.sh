#!/bin/bash

echo "Original content" > my_user_file.txt

sudo cp my_user_file.txt "$HOME/root_copied_file.txt"

echo "Attempting to modify the file as a regular user..."
echo "Modified content" >> "$HOME/root_copied_file.txt"

echo "Attempting to delete the file as a regular user..."
rm -f "$HOME/root_copied_file.txt"

if [ ! -f "$HOME/root_copied_file.txt" ]; then
    echo "The file was successfully deleted."
fi

rm -f my_user_file.txt
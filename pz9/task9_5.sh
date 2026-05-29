#!/bin/bash

FILE="temp_file.txt"

echo "Initial data" > "$FILE"

test_access() {
    echo "Permissions: $(stat -c '%a' "$FILE") | Owner: $(stat -c '%U:%G' "$FILE")"
    
    if cat "$FILE" >/dev/null 2>&1; then
        echo "Read: YES"
    else
        echo "Read: NO"
    fi

    if echo "test" >> "$FILE" 2>/dev/null; then
        echo "Write: YES"
    else
        echo "Write: NO"
    fi
    echo ""
}

test_access

sudo chown root:root "$FILE"

sudo chmod 644 "$FILE"
test_access

sudo chmod 600 "$FILE"
test_access

sudo chmod 666 "$FILE"
test_access

sudo rm -f "$FILE"
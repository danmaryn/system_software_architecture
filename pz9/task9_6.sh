#!/bin/bash

FILES=("$HOME/.bashrc" "/etc/shadow" "/usr/bin/ls")

for f in "${FILES[@]}"; do
    echo "--- File: $f ---"
    ls -l "$f" 2>/dev/null
    
    echo "Attempting to read..."
    if head -n 1 "$f" >/dev/null 2>&1; then
        echo "Read: SUCCESS"
    else
        echo "Read: DENIED"
    fi
    
    echo "Attempting to write..."
    if echo "# test" >> "$f" 2>/dev/null; then
        echo "Write: SUCCESS"
    else
        echo "Write: DENIED"
    fi
    
    echo "Attempting to execute..."
    if "$f" --help >/dev/null 2>&1; then
        echo "Execute: SUCCESS"
    else
        echo "Execute: DENIED / FAILED"
    fi
    echo ""
done
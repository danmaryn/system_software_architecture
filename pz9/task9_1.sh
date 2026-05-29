#!/bin/bash

MY_UID=$(id -u)
FOUND=0

echo "My UID: $MY_UID"
echo "Other regular users (UID >= 1000):"
echo "----------------------------------"

while IFS=: read -r username password uid gid gecos home shell; do
    if [ "$uid" -ge 1000 ] 2>/dev/null && [ "$uid" -lt 60000 ] 2>/dev/null && [ "$uid" -ne "$MY_UID" ] 2>/dev/null; then
        echo "Username: $username, UID: $uid"
        FOUND=1
    fi
done < <(getent passwd)

if [ "$FOUND" -eq 0 ]; then
    echo "No other regular users found on this system."
fi
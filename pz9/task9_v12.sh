#!/bin/bash

(
    CUR_PROCS=$(ps -u $(whoami) --no-headers | wc -l)
    LIMIT=$((CUR_PROCS + 5))
    
    echo "Current processes: $CUR_PROCS"
    echo "Setting max processes limit to: $LIMIT"
    
    ulimit -u $LIMIT
    
    for i in {1..20}; do
        sleep 2 &
        if [ $? -eq 0 ]; then
            echo "Process $i spawned"
        fi
    done
    
    wait
) 2>&1
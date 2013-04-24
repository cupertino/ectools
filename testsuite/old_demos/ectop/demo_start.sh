#!/bin/sh

LOAD=(10 20 60)
CMD=burnP6

echo -n "starting cpuburn ... "

# get length of an array
tLen=${#LOAD[@]}
 
# use for loop read all nameservers
for (( i=0; i<${tLen}; i++ ));
do
$CMD &
PID=$!
echo cpulimit -p $PID -l ${LOAD[$i]}
cpulimit -p $PID -l ${LOAD[$i]} &
done

echo "[done]"


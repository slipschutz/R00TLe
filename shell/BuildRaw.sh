#!/bin/bash


if [ $# -ne 1 ]; then 
    echo "Usage: ./BuildRaw.sh RunNum"
    exit;
fi

if [ ! -L "./evtfiles" ]; then
    echo "Need sym link to the evt files"
    echo "Make a sym link called ./evtfiles"
    exit;
fi

if [ ! -L "./rootfiles" ]; then
    echo "Need sym link to the root files"
    echo "Make a sym link called ./rootfiles"
    exit;
fi

#runNum=$1;
runNum=$(printf "%04d" $1)



echo "Build Raw Root File"
for file in $(ls ./evtfiles/run-${runNum}-??.evt)
do
    
    Evt2Raw $file ./rootfiles/$(echo $file | awk ' BEGIN{FS="/"}{print $3}' | sed s/.evt/-RAW.root/g)
done
exit





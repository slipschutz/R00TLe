#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Need to give a run Number"
    exit
fi

runNum=$(printf "%04d" $1)
theFile=${R00TLeEvtFilesPath}/run-${runNum}-00.evt
if [ -f $theFile ]; then
    /usr/opt/nscldaq/10.2-112/bin/dumper --source=file://$theFile | head -4 | tail -3
fi
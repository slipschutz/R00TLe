#!/bin/bash

runNum=$(printf "%04d" $1)
theFile=/events/e10003/complete/run-${runNum}-00.evt
if [ -f $theFile ]; then
    /usr/opt/nscldaq/current/bin/dumper --source=file://$theFile | head -4 | tail -3
fi
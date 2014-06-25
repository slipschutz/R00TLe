#!/bin/bash

runNum=$(printf "%04d" $1)
theFile=/events/e10003/complete/run-${runNum}-00.evt

/usr/opt/nscldaq/current/bin/dumper --source=file://$theFile | head
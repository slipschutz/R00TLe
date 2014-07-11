#!/bin/bash


if [ $# -ne 1 ]; then 
    echo "Usage: ./BuildData.sh RunNum"
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
#if [ $runNum -lt 10 ] && [ $runNum -ge 0 ]; then
#    runNum="000$runNum";
#elif [ $runNum -ge 10 ] && [ $runNum -lt 100 ]; then
#    runNum="00$runNum"
#elif [ $runNum -ge 100 ] && [ $runNum -lt 1000 ]; then
#    runNum="0$runNum"
#elif [ $runNum -ge 1000 ]; then
#    runNum=$runNum;
#fi

if stat -t -- ./evtfiles/run-${runNum}-*.evt >/dev/null 2>&1
then
    for file in $(ls ./evtfiles/run-${runNum}-??.evt)
    do
	Evt2Cal $file ./rootfiles/$(basename $file .evt).root
    done
else
    echo "Cannot find evt file for run $runNum "
fi

#evtFile=run-$runNum-00.evt
#rootFile=run-$runNum-00.root
#
#
#if [ ! -f "./evtfiles/$evtFile" ]; then
#    echo "Can not find evt file " $evtFile
#fi
#
#Evt2Cal ./evtfiles/$evtFile ./rootfiles/$rootFile




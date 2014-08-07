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


if [ -f ./rootfiles/run-${runNum}-*-RAW.root ]; then
    echo "Build From Raw Root"
    for file in $(ls ./rootfiles/run-${runNum}-*-RAW.root)
    do

	Raw2Cal $file ./$(echo $file | sed s/-RAW//g) $runNum
    done
    exit
fi



if stat -t -- ./evtfiles/run-${runNum}-*.evt >/dev/null 2>&1
then
    for file in $(ls ./evtfiles/run-${runNum}-??.evt)
    do
	Evt2Cal $file ./rootfiles/$(basename $file .evt).root $runNum
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




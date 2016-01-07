#!/bin/bash



if [ $# -lt 1 ]; then 
    echo "Usage: ./BuildData.sh RunNum [-flags value]"
    exit;
fi





while test $# -gt 0; do
    case "$1" in
        -h|--help)
            echo
	    echo "Usage: ./BuildData.sh RunNum [-flags value]"
	    echo "-n,--CorNumber Specify a run number for the corrections file"
	    echo "that is different from the run being built"
	    echo
            exit 0
            ;;
        -n)
            shift
            if test $# -gt 0; then
                fileNum=$1
            else
                echo "no corrections run number given"
                exit 1
            fi
            shift
            ;;
        --CorNumber*)
            export fileNum=`echo $1 | sed -e 's/^[^=]*=//g'`
            shift
            ;;
	*-)
	    echo "Unknown Option $1"
            break
            ;;
        *)
	    runNum=$(printf "%04d" $1)
	    fileNum=$1
	    shift
	    ;;

    esac
done
 
echo $runNum
echo $fileNum



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


if [ -f ./rootfiles/run-${runNum}-00-RAW.root ]; then
    echo "Build From Raw Root"
    count=0
    for file in $(ls ./rootfiles/run-${runNum}-*-RAW.root)
    do
	Raw2Cal $file ./$(echo $file | sed s/-RAW//g) $fileNum &> log${fileNum}_${count} &
	count=$(($count+1))
    done
    exit
fi



if stat -t -- ./evtfiles/run-${runNum}-*.evt >/dev/null 2>&1
then
    for file in $(ls ./evtfiles/run-${runNum}-??.evt)
    do
	Evt2Cal $file ./rootfiles/$(basename $file .evt).root $fileNum
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




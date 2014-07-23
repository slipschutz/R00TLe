#!/bin/bash


if [ $# -ne 1 ]; then 
    echo "Usage: ./BuildData.sh RunNum"
    exit;
fi


##
##Check to see if the enviroment variables have been set
##
flag="";
flag=$(env | grep R00TLeEvtFilesPath);
if [ ${flag} == "" ]; then
    echo "No Evt File Path"
    exit
fi


flag="";
flag=$(env | grep R00TLeRootFilesPath);
if [ ${flag} == "" ]; then
    echo "No ROOT File Path"
    exit
fi
runNum=$(printf "%04d" $1)

source R00TLeLogon.sh sam
pwd
if stat -t -- ${R00TLeEvtFilesPath}/run-${runNum}-*.evt >/dev/null 2>&1
then
    for file in $(ls ${R00TLeEvtFilesPath}/run-${runNum}-??.evt)
    do
	command="${R00TLeInstall}/bin/Evt2Cal $file ${R00TLeRootFilesPath}/$(basename $file .evt).root"
	echo $command | qsub -N "R00TLeBuild" -m a -j oe -l walltime=01:00:00
    done
else
    echo "Cannot find evt file for run $runNum "
fi

#!/bin/bash



temp=$(ls -lhtr --time-style=long-iso ~/stagearea/complete/* | tail -1 | awk ' { print $6 " " $7  }')
timeOfMostRecentFile=$(date -d "${temp}" "+%s")

currentTime=$(date "+%s")

absTDiff=$(echo $(($timeOfMostRecentFile-$currentTime)) | awk '{ if ($1<0){print -1*$1}else{print $1}}')

echo $absTDiff

exit

BuildRaw.sh $1

runNum=$(printf "%04d" $1)
echo $runNum
scp ./rootfiles/run-${runNum}-*RAW.root e10003@fishtank:/mnt/analysis/e10003/rootfiles/


ssh e10003@fishtank 'bash -s' < ${R00TLeInstall}/shell/BuildData.sh $1
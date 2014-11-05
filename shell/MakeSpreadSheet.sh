#!/bin/bash




newestFile=$(ls -lhtr ${R00TLeEvtFilesPath} | awk '{ if ($0 ~/-00.evt/){print}}' | tail -1 | awk ' { print $9}')
newestRunNum=$(echo $newestFile | awk ' BEGIN{FS="-"} {print $2+1-1}')




#TITLE line
echo "Run Number,Run Start Time,Run Title,Run Total Size,Number Of Run Segments"

for ((i=0;i<=$newestRunNum;i++))
do

runNum=$(printf "%04d" $i)
fileStem=${R00TLeEvtFilesPath}/run-${runNum}
theFile=${R00TLeEvtFilesPath}/run-${runNum}-00.evt

if [ -f $theFile ]; then #THE FILE EXISTS 
    /usr/opt/nscldaq/current/bin/dumper --source=file://$theFile | head -4 | tail -3 > TEMP__$i

    startTime=$(head -1 TEMP__$i | awk ' {print $1 " " $2 " " $3 " " $4 " " $5}')

    title=$(head -2 TEMP__$i | tail -1 | awk 'BEGIN{FS=":"}{print $2}')
    
    size=$(du -hc $theFile | tail -1 | awk ' {print $1}')
    
    numSegments=$(ls $fileStem* | wc -l)

    echo $runNum","$startTime","$title","$size","$numSegments

    rm -f TEMP__$i
else
    echo $runNum",0,NO DATA,0,0"

fi

done
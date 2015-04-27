#!/bin/bash


if [ $# -ne 2 ]; then
    echo "Must give two arguments"
    echo "Usage: ReBuildData.sh StartRunNum EndRunNum"
    exit
fi

start=$1
end=$2;

if [ $start -ge $end ]; then
    echo "Invaild RunNumber range"
        
fi
count=0;
numAtATime=6;
for ((i=$start;i<=$end;i++)); do
    if [ $count -lt $(($numAtATime-1)) ]; then
	echo "BuildData.sh $i &> ./log$i &"
	$(BuildData.sh $i &> ./log$i &)
	count=$(($count+1))
    else 
	echo "BuildData.sh $i &> ./log$i"
	$(BuildData.sh $i &> ./log$i )

	count=0;
    fi
done
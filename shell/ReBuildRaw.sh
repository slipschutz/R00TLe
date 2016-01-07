#!/bin/bash


if [ $# -ne 2 ]; then
    echo "Must give two arguments"
    echo "Usage: ReBuildRaw.sh StartRunNum EndRunNum"
    exit
fi

start=$1
end=$2;

if [ $start -ge $end ]; then
    echo "Invaild RunNumber range"
        
fi
count=0;
numAtATime=12;
for ((i=$start;i<=$end;i++)); do
    if [ $count -lt $(($numAtATime-1)) ]; then
	echo "BuildRaw.sh $i &> ./log$i &"
	$(BuildRaw.sh $i &> ./log$i &)
	count=$(($count+1))
    else 
	echo "BuildRaw.sh $i &> ./log$i"
	$(BuildRaw.sh $i &> ./log$i )

	count=0;
    fi
done
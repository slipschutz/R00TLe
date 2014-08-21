#!/bin/bash


listOfUsers=$(ls ${R00TLeInstall}/users/);

for i in $listOfUsers 
do
rm -f ${R00TLeInstall}/users/$i/.R00TLelogonfile
R00TLeLogon.sh $i
done

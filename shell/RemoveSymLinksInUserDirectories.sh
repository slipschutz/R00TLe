#!/bin/bash


listOfUsers=$(ls ${R00TLeInstall}/users/);

for i in $listOfUsers 
do
unlink ${R00TLeInstall}/users/$i/evtfiles
unlink ${R00TLeInstall}/users/$i/rootfiles
done

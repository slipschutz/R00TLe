#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Give input/output files"
    exit
fi
evtFile=$1
rootFile=$2

export R00TLe_PRM=${R00TLeInstall}/prm

echo "The prm file is in "$R00TLe_PRM
echo "THe evt file " $evtFile
echo "the root file "$rootFile
${R00TLeInstall}/bin/Evt2Cal $evtFile $rootFile


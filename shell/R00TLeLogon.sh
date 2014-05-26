#!/bin/bash


if [ $# -ne 1 ]; then
    echo "Usage: R00TLeLogon AUserName"
    exit;
fi

#look to see if there is a R00TLeInstall
flag=$(env | grep "R00TLeInstall")

if [ "$flag" == "" ]; then #failed

    echo "Run the Make Install for R00TLe"
    echo "Then source your .bashrc"
    exit;

fi


cd $R00TLeInstall

##make directory for user
if [ ! -d users/$1 ]; then
    mkdir -p users/$1
else 
    echo "User found exiting..."
#    exit;
fi
cd users/$1 # change directory in to the users directory

### Make a rootrc
rm -f .rootrc
echo "Rint.HistSize 500" >> .rootrc
echo "Rint.HistSave 100" >> .rootrc

echo "Unix.*.Root.DynamicPath: .:${R00TLeInstall}/lib:\$(ROOTSYS)/lib" >> .rootrc
echo "Unix.*.Root.MacroPath: .:${R00TLeInstall}/lib:\$(ROOTSYS)/lib" >> .rootrc
echo "Rint.Logon: ${R00TLeInstall}/users/$1/rootlogon.C">>.rootrc

###Make a rootlogon
rm -f rootlogon.C

echo "rootlogon(){">>rootlogon.C
echo "cout<<\"\\nWelcome to R00TLe\\n\"<<endl;" >>rootlogon.C
echo "gSystem->Load(\"libLendaEvent.so\");">>rootlogon.C
echo "gSystem->Load(\"libS800.so\");">>rootlogon.C
echo "gSystem->Load(\"libDDASEvent.so\");">>rootlogon.C
echo "gSystem->Load(\"libLendaSettings.so\");">>rootlogon.C
echo "return;">>rootlogon.C
echo "}">>rootlogon.C










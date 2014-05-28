#!/bin/bash


if [ $# -ne 1 ] && [ $# -ne 2 ] ; then
    echo "Usage: R00TLeLogon AUserName"
    return
fi

if [ $# -eq 2 ] && [ "$1" == "-f" ]; then
    echo
    echo $2 >> $R00TLeInstall/users/.knownusers
    echo "User $2 Added to known Users"
    echo "To Login as $2 run:"
    echo "R00TLeLogon.sh $2"
    return
fi

#check to see if user is known
if [ ! -f $R00TLeInstall/users/.knownusers ]; then
    echo
    echo "N known users file found"
    echo "Run 'R00TLeLogon.sh -f A_User_Name' to create one"
    return
fi
userStat=$(cat $R00TLeInstall/users/.knownusers | grep $1)
if [ "$userStat" == "" ]; then
    echo
    echo "User $1 not found"
    echo "Run 'R00TLeLogon.sh -f $1' to force"
    return
fi
#look to see if there is a R00TLeInstall
flag=$(env | grep "R00TLeInstall")

if [ "$flag" == "" ]; then #failed

    echo "Did not find R00TLeInstall enviroment variable"
    echo "Run the Make Install for R00TLe"
    echo "Then source your .bashrc"
    return

fi

echo "Setting enviorment"
export ANAROOT_PRM=/user/e10003/R00TLe/prm

cd $R00TLeInstall

##make directory for user
if [ ! -d users/$1 ]; then
    echo "Making a directory for $1 in ${R00TLeInstall}/users"
    mkdir -p users/$1
    echo "Switching to working directory"
    cd users/$1 # change directory in to the users directory
else 
    echo "User $1 found. Won't make start up files"
    echo "Switching to working directory"
    cd users/$1 # change directory in to the users directory
    return
fi

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










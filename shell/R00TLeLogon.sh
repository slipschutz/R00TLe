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
    echo "No known users file found"
    echo "Run 'R00TLeLogon.sh -f A_User_Name' to create one"
    return
fi
user=$1
userStat=$(cat $R00TLeInstall/users/.knownusers | awk ' { if($1=="'$user'"){print 1}}')
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

echo "Setting enviorment Variables"
export ANAROOT_PRM=/user/e10003/R00TLe/prm
export R00TLe_User=$1
cd $R00TLeInstall

##make directory for user
if [ ! -f users/$1/.R00TLelogonfile ]; then
    echo "Making a directory for $1 in ${R00TLeInstall}/users"
    mkdir -p users/$1
    echo "Switching to working directory..."
    cd users/$1 # change directory in to the users directory
    touch .R00TLelogonfile
else 
    echo "User $1 found."
    echo "Switching to working directory..."
    cd users/$1 # change directory in to the users directory
    return
fi

##Should now be in the directory for the user
echo "Generating ROOT start up file..."
### Make a rootrc
rm -f .rootrc
echo "Rint.HistSize 500" >> .rootrc
echo "Rint.HistSave 100" >> .rootrc
echo "Canvas.ShowEventStatus: true">>.rootrc
echo "Unix.*.Root.DynamicPath: .:${R00TLeInstall}/lib:\$(ROOTSYS)/lib" >> .rootrc
echo "Unix.*.Root.MacroPath: .:${R00TLeInstall}/scripts:${R00TLeInstall}/users/${1}/macros:\$(ROOTSYS)/macros" >> .rootrc

echo "Rint.Logon: ${R00TLeInstall}/users/$1/rootlogon.C">>.rootrc
echo "ACLiC.IncludePaths: -I$HOME/R00TLe/src/include">>.rootrc

echo "Rint.History ${R00TLeInstall}/users/$1/.roothist">>.rootrc

###Make a rootlogon
rm -f rootlogon.C

#copy the skelton version of the rootlogon
echo "Generating ROOT logon file..."
cp ${R00TLeInstall}/skelton/rootlogon.C .

echo "rootlogon(){" >> temp_R00TLe
echo "cout<<\"\\nHello $1,\"<<endl;" >>temp_R00TLe


cat rootlogon.C | awk ' {if ($0 !~/rootlogon()/){print}}' >> temp_R00TLe

mv -f temp_R00TLe rootlogon.C

if [ -d src ] || [ -d macros ] || [ -d histograms ]; then
    echo "Found histogrmer already.  Won't over write"
else 
echo "Copying over skelton histogramer..."
cp -r ${R00TLeInstall}/skelton/src/ .
cp -r ${R00TLeInstall}/skelton/macros/ .
cp -r ${R00TLeInstall}/skelton/histograms/ .
fi


if [ ! -L ./evtfiles ]; then
echo "Making sym link to evt files"
    ln -s ${R00TLeEvtFilesPath} ./evtfiles
fi

if [ ! -L rootfiles ]; then
    echo "Making sym link to root files"
    ln -s ${R00TLeRootFilesPath} ./rootfiles
fi
#!/bin/bash


if [ $# -ne 1 ] && [ $# -ne 2 ] ; then
    echo "Usage: R00TLeLogon AUserName"
    return
fi
AutoAddScripts=0;

if [ $# -eq 2 ] && [ "$1" == "-f" ]; then
    echo
    echo $2 >> $R00TLeInstall/users/.knownusers
    echo "User $2 Added to known Users"
    R00TLeLogon.sh $2;
    return
elif [ $# -eq 2 ] && [ "$2" == "-a" ]; then
    AutoAddScripts=1;
elif [ $# -eq 2 ] && [ "$2" == "-t" ]; then
    ${R00TLeInstall}/shell/sl 
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

    echo "Did not find R00TLeInstall environment variable"
    echo "Run the Make Install for R00TLe"
    echo "Then source your .bashrc"
    return

fi

echo "Setting environment Variables"

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
echo "ACLiC.IncludePaths: -I${R00TLeInstall}/src/include">>.rootrc

echo "Rint.History ${R00TLeInstall}/users/$1/.roothist">>.rootrc

###Make a rootlogon
rm -f rootlogon.C
rm -f rootlogoff.C
#copy the skelton version of the rootlogon
echo "Generating ROOT logon file..."
cp -f ${R00TLeInstall}/skeleton/rootlogon.C .
cp -f ${R00TLeInstall}/skeleton/rootlogoff.C .

echo "rootlogon(){" >> temp_R00TLe
echo "cout<<\"\\nHello $1,\"<<endl;" >>temp_R00TLe


cat rootlogon.C | awk ' {if ($0 !~/rootlogon()/){print}}' >> temp_R00TLe

echo "gROOT->ProcessLine(\".include ${R00TLeInstall}/src/include\");">>temp_R00TLe

if [ $AutoAddScripts -eq 1 ]; then 
    echo "Auto loading scripts to rootlogon.C"
    for file in $(ls ${R00TLeInstall}/scripts/*.C) 
    do
	echo "gROOT->ProcessLine(\".L $file\" );" >> temp_R00TLe
    done
else
    echo "gROOT->ProcessLine(\".L D.C\" );" >> temp_R00TLe
    echo "gROOT->ProcessLine(\".L Find.C\" );" >> temp_R00TLe
fi

# for file in $(ls ${R00TLeInstall}/users/$1/macros/*.C) 
# do
# echo "gROOT->ProcessLine(\".L $file\" );" >>temp_R00TLe
# done


echo "return;">>temp_R00TLe
echo "}">>temp_R00TLe

mv -f temp_R00TLe rootlogon.C

if [ -d src ] || [ -d macros ] || [ -d histograms ] || [ -d TimingAnalyzer ]; then
    echo "Found histogrmer already.  Won't over write"
else 
echo "Copying over skeleton histogramer..."
cp -r ${R00TLeInstall}/skeleton/src/ .
cp -r ${R00TLeInstall}/skeleton/macros/ .
cp -r ${R00TLeInstall}/skeleton/histograms/ .
cp -r ${R00TLeInstall}/src/TimingAnalyzer/ .
cp ${R00TLeInstall}/skeleton/invmap.inv .
cp ${R00TLeInstall}/skeleton/AllTheCorrections.txt .
fi





if [ -L ./evtfiles ]; then
    unlink ./evtfiles
elif [ -e ./evtfiles ]; then
    echo 
    echo "<R00TLeLogon.sh:Error> Cannot have file called 'evtfiles' must be a symlink to the evtfiles"
    echo
    return
fi


echo "Making sym link to evt files"
ln -s ${R00TLeEvtFilesPath} ./evtfiles

if [ -L ./rootfiles ]; then
    unlink ./rootfiles
elif [ -e ./rootfiles ]; then
    echo 
    echo "<R00TLeLogon.sh:Error> Cannot have file called 'rootfiles' must be a symlink to the rootfiles"
    echo
    return
    
fi

echo "Making sym link to root files"
ln -s ${R00TLeRootFilesPath} ./rootfiles

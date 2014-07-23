#!/bin/bash



Build_Directory=$(pwd);
cd ../
R00TLe_HEAD=$(pwd);
LIB_DIRECTORY=$(pwd)/lib
BIN_DIRECTORY=$(pwd)/bin

SHELL_DIRECTORY=$(pwd)/shell
cd $Build_Directory

#See if there are already things 
#added in bashrc about R00TLe

flag=$(cat ~/.bashrc | grep R00TLe)

if [ "$flag" == "" ]; then #nothing found
    
    while [ 1 -eq 1 ]; do
	echo "Enter the full path to the EVT files"
	read Path_To_Evt_Files;
	
	if [ -d ${Path_To_Evt_Files} ]; then
	    break;
	else
	    echo "Cannot find "${Path_To_Evt_Files}
	fi
    done


    while [ 1 -eq 1 ]; do
	echo "Enter the full path to the ROOT files"
	read Path_To_ROOT_Files;
	
	if [ -d ${Path_To_ROOT_Files} ]; then
	    break;
	else
	    echo "Cannot find "${Path_To_ROOT_Files}
	fi
    done

    
    echo "Installing..."
    echo "Adding lines to ~/.bashrc"
    printf "\n#Begin R00TLe settings\n">>~/.bashrc
    echo "export PATH=\$PATH:${BIN_DIRECTORY}">>~/.bashrc
    echo "export PATH=\$PATH:$SHELL_DIRECTORY">>~/.bashrc
    echo "export R00TLeInstall=$R00TLe_HEAD">>~/.bashrc
    echo "export R00TLeEvtFilesPath=${Path_To_Evt_Files}">>~/.bashrc
    echo "export R00TLeRootFilesPath=${Path_To_ROOT_Files}">>~/.bashrc
    echo "alias root='root -l'">>~/.bashrc
    echo "alias R00TLeLogon.sh='source R00TLeLogon.sh'">>~/.bashrc
    printf "#End R00TLe settings\n">>~/.bashrc
    echo 
    echo "R00TLe Install Successful"
    echo "Source your bashrc"
    echo

else 
    echo "Seems like install has already been run"
fi




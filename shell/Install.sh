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
    echo "\n#Begin R00TLe settings">>~/.bashrc
    echo "export PATH=\$PATH:${BIN_DIRECTORY}">>~/.bashrc
    echo "export PATH=\$PATH:$SHELL_DIRECTORY">>~/.bashrc
    echo "export R00TLeInstall=$R00TLe_HEAD">>~/.bashrc
    echo "alias root='root -l'">>~/.bashrc
    echo "#End R00TLe settings">>~/.bashrc
fi


echo 
echo "R00TLe Install Successful"
echo



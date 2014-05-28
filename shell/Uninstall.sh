#!/bin/bash


#See if there are already things 
#added in bashrc about R00TLe

flag=$(cat ~/.bashrc | grep R00TLe)

if [ "$flag" != "" ]; then #Something found 
    cp -f ~/.bashrc ./.bashrc_backup
    cat ~/.bashrc | awk ' BEGIN{flag=1;}{
       if ( $0 ~/Begin R00TLe/ && flag==1){flag=-1;}
       if ( $0 ~/End R00TLe/ && flag==-1){flag=1;}
       if (flag==1 && $0!~/End R00TLe/){print}
} ' > R00TLetemp
cp -f ./R00TLetemp ~/.bashrc
rm -f ./R00TLetemp

echo "Uninstall succeeded."
echo "Back up of ~/.bashrc before Unistall saved in ./.bashrc_backup"
else 
    echo "Seems like make install has not been run"
    echo "Can't Unistall.."
fi




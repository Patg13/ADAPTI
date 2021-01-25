#!/bin/bash

##################################################################
#								 #
#	   Script de creation du fichier group ADAPTI		 #
#								 #
#                 Dependance principale ADAPTI 			 #
#								 #
#								 #
#                     Par : Patrick Gagne			 #
#								 #
#		  Version 5.0 (28 octobre 2019)			 #
#								 #
##################################################################

install_path=$1
nom_analyse=$2


file_list=$( ls *.Complete.fasta )


touch $2.group
for i in $file_list
do
  ech_name=$( echo "$i" | sed 's/\.Complete.fasta.*//g' )
  $install_path/Nettoyage/generate_group.py $i $ech_name >> $2.group
done


$install_path/Nettoyage/screen_duplicates.py $2.group

#!/bin/bash

echo "Resetting ADAPTI" 

current_dir=$( pwd )
current_dir=$( echo $current_dir )

install_path="$current_dir""/Programmes/Programmes_Modules"

reset_lines=$( grep -h "INSTALLATION_PATH=" $install_path/Pipelines/optionfile_*.txt )


for line in $reset_lines
do
    sed -i "s;$line;INSTALLATION_PATH=installation_path_here;g" $install_path/Pipelines/optionfile_*.txt
done


database_lines=$( grep -h -E "^DATABASE=" $install_path/Pipelines/optionfile_*.txt )

for line in $database_lines
do
    sed -i "s;^$line;DATABASE=database_location_here;g" $install_path/Pipelines/optionfile_*.txt
done

fasta_database_lines=$( grep -h "FASTA_DATABASE=" $install_path/Pipelines/optionfile_*.txt )

for line in $fasta_database_lines
do
    sed -i "s;^$line;FASTA_DATABASE=fasta_database_location_here;g" $install_path/Pipelines/optionfile_*.txt
done

rm $install_path/Traitement_Cox/CoxFIlter
rm $install_path/Nettoyage/derIplicate
rm $install_path/Nettoyage/HomopRemover
rm $install_path/Nettoyage/Illumicut
rm $install_path/Extraction_ITS/PatFinder

echo "Reset DONE"


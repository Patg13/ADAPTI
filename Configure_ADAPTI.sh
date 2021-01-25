#!/bin/bash

current_dir=$( pwd )
current_dir=$( echo $current_dir )

install_path="$current_dir""/Programmes/Programmes_Modules"
sources_path="$current_dir""/Programmes/Sources"

if [ -e $install_path/Traitement_Cox/CoxFIlter ] && [ -e $install_path/Nettoyage/derIplicate ] && [ -e $install_path/Nettoyage/HomopRemover ] && [ -e $install_path/Nettoyage/Illumicut ] && [ -e $install_path/Extraction_ITS/PatFinder ]
then
    echo "WARNING: ADAPTI previous configuration files found"
    read -p "Press [RETURN] to reconfigure ADAPTI or ctrl-c to cancel configuration"
    ./Reset.sh
fi

skip_compilation=0
if [ $skip_compilation -eq 0 ]
then
    error=0
    cd $sources_path

    tar -zxvf Cox_FIlter_V2.tar.gz
    cd $sources_path/Cox_FIlter_V2/CoxFIlterV1/Release/
    sed -i "s;SOURCE_FOLDER_HERE;$sources_path;g" makefile subdir.mk
    make
    mv CoxFIlter $install_path/Traitement_Cox/
    cd $sources_path
    rm -R Cox_FIlter_V2/
    if [ ! -e $install_path/Traitement_Cox/CoxFIlter ]
    then
        echo "ERROR: CoxFIlter make failed, aborting ADAPTI configuration"
        exit 1
    fi

    tar -zxvf DerIplicate_V2.tar.gz
    cd $sources_path/DerIplicate_V2/DNAlibPG/Release/
    make
    cd $sources_path/DerIplicate_V2/derIplicateV2/Release/
    sed -i "s;SOURCE_FOLDER_HERE;$sources_path;g" makefile subdir.mk
    make
    mv derIplicate $install_path/Nettoyage/
    cd $sources_path
    rm -R DerIplicate_V2/
    if [ ! -e $install_path/Nettoyage/derIplicate ]
    then
        echo "ERROR: derIplicate make failed, aborting ADAPTI configuration"
        exit 1
    fi

    tar -zxvf Homop_Remover_V2_5.tar.gz
    cd $sources_path/Homop_Remover_V2_5/HomopRemoverV2/Release/
    sed -i "s;SOURCE_FOLDER_HERE;$sources_path;g" makefile subdir.mk
    make
    mv HomopRemover $install_path/Nettoyage/
    cd $sources_path
    rm -R Homop_Remover_V2_5/
    if [ ! -e $install_path/Nettoyage/HomopRemover ]
    then
        echo "ERROR: HomopRemover make failed, aborting ADAPTI configuration"
        exit 1
    fi

    tar -zxvf Illumicut_V5.tar.gz
    cd $sources_path/Illumicut_V5/DNAlibPG/Release/
    make
    cd $sources_path/Illumicut_V5/Illumicut/Release/
    sed -i "s;SOURCE_FOLDER_HERE;$sources_path;g" makefile subdir.mk
    make
    mv Illumicut $install_path/Nettoyage/
    cd $sources_path
    rm -R Illumicut_V5/
    if [ ! -e $install_path/Nettoyage/Illumicut ]
    then
        echo "ERROR: Illumicut make failed, aborting ADAPTI configuration"
        exit 1
    fi

    tar -zxvf PatFinder_V2.tar.gz
    cd $sources_path/PatFinder_V2/DNAlibPG/Release/
    make
    cd $sources_path/PatFinder_V2/PatFinder/Release/
    sed -i "s;SOURCE_FOLDER_HERE;$sources_path;g" makefile subdir.mk
    make
    mv PatFinder $install_path/Extraction_ITS/
    cd $sources_path
    rm -R PatFinder_V2/
    if [ ! -e $install_path/Extraction_ITS/PatFinder ]
    then
        echo "ERROR: PatFinder make failed, aborting ADAPTI configuration"
        exit 1
    fi

fi

sed -i "s;installation_path_here;$install_path;g" $install_path/Pipelines/optionfile_*.txt


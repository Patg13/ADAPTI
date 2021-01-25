#!/bin/bash

##################################################################
#								 #
#		 ITS1 Extraction Script for ADAPTI	 	 #
#								 #
#       ADAPTI dependency for ITS1 abundance analysis type	 #
#								 #
#								 #
#                     By : Patrick Gagne			 #
#								 #
#		   Version 5.0 (june 8 2019)			 #
#								 #
##################################################################

#!!! IMPORTANT NOTICE !!!

# Input file should'nt have the ;size=xxxx; tag , This tag could cause an error during the full sequence fetching
# It's recommanded to remove the size tags after ITS extraction, just before the second dereplication 

option_file=$1
base_file=$2
nom_analyse=$3
namefile=$4
rep_temp=$5

source $option_file


#install_path=$( grep -oP '(?<=INSTALLATION_PATH=).*' $option_file )
#nb_thread=$( grep -oP '(?<=NB_THREAD=).*' $option_file )
#pf_ssu_ep=$( grep -oP '(?<=F_SSU_EXTRACTION_PATTERN=).*' $option_file )
#pf_58s_ep=$( grep -oP '(?<=F_58S_EXTRACTION_PATTERN=).*' $option_file )
#pf_mm=$( grep -oP '(?<=PATTERN_MAX_MISMATCH=).*' $option_file )



prefix_name=$( echo $base_file | sed 's/\.fasta.*//g' )

$INSTALLATION_PATH/Extraction_ITS/PatFinder -f $base_file -o $prefix_name.minITS3A.fasta -s $prefix_name.ITS3A_FAILED.fasta -i B -t $NB_THREAD -p $F_SSU_EXTRACTION_PATTERN -m $PATTERN_MAX_MISMATCH
$INSTALLATION_PATH/Extraction_ITS/PatFinder -f $prefix_name.minITS3A.fasta -o $prefix_name.minSSUP.fasta -s $prefix_name.SSUP_FAILED.fasta -i A -t $NB_THREAD -p $F_58S_EXTRACTION_PATTERN -m $PATTERN_MAX_MISMATCH

$INSTALLATION_PATH/Extraction_ITS/Get_seqs_more.py -fasta $prefix_name.SSUP_FAILED.fasta -full $base_file -out $prefix_name.SSUP_FAILED.fs.fasta

cat $prefix_name.ITS3A_FAILED.fasta $prefix_name.SSUP_FAILED.fs.fasta >> $prefix_name.ITS1_FAILED.fasta

$INSTALLATION_PATH/Extraction_ITS/fastac2.py $prefix_name.ITS1_FAILED.fasta

$INSTALLATION_PATH/Extraction_ITS/Enh_ITSx.sh -f $prefix_name.ITS1_FAILED.fasta -c "ITSx --allow_reorder T --table T --detailed_results T --not_found T --silent T --saw_raw T --preserve t" -t $NB_THREAD -g fungi -s SSU,ITS1 -o $prefix_name.ITS_recup

#echo -e "######### Rapport Extraction ITS #########\n" >> $ANALYSIS_NAME.ITS_ext_report.txt

seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py $prefix_name.minITS3A.fasta )
rejet_count=$( $INSTALLATION_PATH/Rapport/fastac.py $prefix_name.ITS3A_FAILED.fasta )
read_count_good=$( $INSTALLATION_PATH/Rapport/readsc.py -in $prefix_name.minITS3A.fasta -name $namefile )
read_count_rej=$( $INSTALLATION_PATH/Rapport/readsc.py -in $prefix_name.ITS3A_FAILED.fasta -name $namefile )
echo -e "# sequences after ITS3A pattern Extraction : $seq_count ($read_count_good reads)" >> $nom_analyse.ITS_ext_report.txt
echo -e "# sequences rejected by ITS3A pattern Extraction : $rejet_count ($read_count_rej reads)\n" >> $nom_analyse.ITS_ext_report.txt

seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py $prefix_name.minSSUP.fasta )
rejet_count=$( $INSTALLATION_PATH/Rapport/fastac.py $prefix_name.SSUP_FAILED.fasta )
read_count_good=$( $INSTALLATION_PATH/Rapport/readsc.py -in $prefix_name.minSSUP.fasta -name $namefile )
read_count_rej=$( $INSTALLATION_PATH/Rapport/readsc.py -in $prefix_name.SSUP_FAILED.fasta -name $namefile )
echo -e "# sequences after SSUP pattern Extraction : $seq_count ($read_count_good reads)" >> $nom_analyse.ITS_ext_report.txt
echo -e "# de sequences rejected by SSUP pattern Extraction : $rejet_count ($read_count_rej reads)\n" >> $nom_analyse.ITS_ext_report.txt

seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py $prefix_name.ITS1_FAILED.fasta )
read_count=$( $INSTALLATION_PATH/Rapport/readsc.py -in  $prefix_name.ITS1_FAILED.fasta -name $namefile )
echo -e "# sequences rejected for ITS1 Extraction : $seq_count ($read_count reads)\n" >> $nom_analyse.ITS_ext_report.txt

         
#echo -e "##########################################\n" >> $ANALYSIS_NAME.ITS_ext_report.txt


mv $prefix_name.minSSUP.fasta $prefix_name.ITS1.fasta

cat ./$prefix_name.ITS_recup/$prefix_name.ITS_recup.ITS1.fasta >> $prefix_name.ITS1.fasta

seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py ./$prefix_name.ITS_recup/$prefix_name.ITS_recup.ITS1.fasta )
read_count=$( $INSTALLATION_PATH/Rapport/readsc.py -in ./$prefix_name.ITS_recup/$prefix_name.ITS_recup.ITS1.fasta -name $namefile )
echo -e "# sequences saved by ITSx : $seq_count ($read_count reads)\n" >> $nom_analyse.ITS_ext_report.txt

echo "ITS1 Extraction Report:$nom_analyse.ITS_ext_report.txt:rep:" >> $rep_temp



#!/bin/bash

adaptivar_version="0.86 (august 1 2019)"
variable_tag="adaptivar_"
invalidation_tag=0


##################################################################
#								 #
#		   ADAPTI Main Analysis Script			 #
#								 #
#        	 WARNING ! EXPERIMENTAL ! WARNING         	 #
#								 #
#								 #
#                     by : Patrick Gagne			 #
#								 #
#	          Version 0.8 (october 4 2018)			 #
#								 #
##################################################################

# Analysis Types :
# 1 : Fungi Pathogen
# 2 : Fungi Soil
# 3 : Fungi Soil With ITS extraction
# 4 : Cox

#################################
# Functions Definition Section  #
#################################


function calculate_time(){
    i=$1
    ((sec=i%60, i/=60, min=i%60, hrs=i/60))
    timestamp=$(printf "%d:%02d:%02d" $hrs $min $sec)
    echo $timestamp
}

function add_new_extension(){
    filename=$1
    next=$2
    lastext="."$( echo $filename | awk -F. '{print $NF}' )
    filename_len=${#filename}
    last_ext_len=${#lastext}
    base_filename=${filename:0:filename_len-last_ext_len}
    next_len=${#next}
    if [ $next_len -eq 0 ]
    then
        echo "$base_filename"
    fi
    if [ $next_len -ne 0 ]
    then
        echo "$base_filename"".$next"
    fi
    
}

function debug_print(){
    if [ "$DEBUG_MODE" == "true" ]
    then
        echo "DEBUG   $1"
    fi
}

function error_watch(){
    if [ $? -ne 0 ]
    then
        echo "ERROR: Catastrophic failure detected in ADAPTI pipeline"
        echo "ADAPTI HALTED"
        exit 2
    fi
}

function create_checkpoint(){
   chkptname=$1
   touch $chkptname.adapti.chkpt
   set | grep "^$variable_tag" > $chkptname.adapti.chkpt
   echo "ANALYSIS_NAME=$ANALYSIS_NAME" >> $chkptname.adapti.chkpt
}


# Checkpointing General Condition
#if [ ! -e name.adapti.chkpt ] || [ "$IGNORE_CHECKPOINTS" == "true" ] || [ $invalidation_tag -eq 1 ]
#then
#    code
#    create_checkpoint name
#else
#    source name.adapti.chkpt
#fi

# END OF FUNCTIONS DEFINITION SECTION ############################################

############################
#  OPTION PARSING SECTION  #
############################

adaptivar_option_file=$1
adaptivar_test_mode=$2

if [ ! $adaptivar_option_file ]
then
    echo "ERROR: Options File not specified"
    exit 1
fi

if [ ! -e $adaptivar_option_file ]
then
    echo "ERROR: $adaptivar_option_file not found"
    exit 1
fi

source $adaptivar_option_file

if [ "$INSTALLATION_PATH" == "installation_path_here" ]
then
    echo "ERROR: ADAPTI installation path not specified in option file $INSTALLATION_PATH"
    exit 2
fi

if [ "$adaptivar_test_mode" == "test" ]
then
    $INSTALLATION_PATH/Pipelines/testmode.sh $adaptivar_option_file
    exit $?
fi

print_logo() {
echo ""
echo '^    ^    ^    ^    ^    ^    ^    ^'
echo ' \  / \  / \  / \  / \  / \  / \  /'
echo '  \/|A|\/|D|\/|A|\/|P|\/|T|\/|I|\/'
echo '  /\| |/\| |/\| |/\| |/\| |/\| |/\'
echo ' /  \ /  \ /  \ /  \ /  \ /  \ /  \'
echo 'V    V    V    V    V    V    V    V'
echo ""
echo 'Automatic Dna Analysis & Processing Tool for Illumina'
echo "Version $adaptivar_version"
echo "By : Patrick Gagne"
}

print_logo

# END OF OPTIONS PARSING SECTION ############################################

# DEPRECIATED OPTION PARSING METHOD
#install_path=$( grep -oP '(?<=^INSTALLATION_PATH=).*' $adaptivar_option_file )

####################################
#    Reads Preparation Section     #
####################################

if [ "$ANALYSIS_TYPE" != "ITSPatho" ] && [ "$ANALYSIS_TYPE" != "ITSSoil" ] && [ "$ANALYSIS_TYPE" != "SoilExtITS1" ] && [ "$ANALYSIS_TYPE" != "Cox" ]
then
    echo "ERROR: Analysis type: $ANALYSIS_TYPE not recognized"
    exit 1
fi



if [ «"$ANALYSIS_NAME"» == «"RANDOM"» ]
then
    random_number=$RANDOM
    random_number="$random_number"
    analysis_date=$( date +"%d%b%Y" )
    analysis_time=$( date +"%R" | sed 's/:/h/g' )
    ANALYSIS_NAME="Analysis$random_number""_"$ANALYSIS_TYPE"_$analysis_date""_$analysis_time"
    check=$( ls * | grep "$ANALYSIS_NAME" | head -n 1 )
    while [ «"$check"» != «""» ]
    do
        random_number=$RANDOM
        random_number="$random_number"
        analysis_date=$( date +"%d%b%Y" )
        analysis_time=$( date +"%R" | sed 's/:/h/g' )
        ANALYSIS_NAME="Analysis$random_number""_"$ANALYSIS_TYPE"_$analysis_date""_$analysis_time"
        check=$( ls * | grep "$ANALYSIS_NAME" | head -n 1)
    done
fi


if [ ! $ANALYSIS_NAME ]
then
    echo "ERROR, analysis name not specified"
    exit
fi


adaptivar_rep_temp=$ANALYSIS_NAME.report.temp
adaptivar_result_folder="$ANALYSIS_NAME""_Results"
adaptivar_inter_folder="$ANALYSIS_NAME""_intermediate"
mkdir $adaptivar_result_folder
mkdir $adaptivar_inter_folder
cd $adaptivar_result_folder
mkdir OTUS_Extraction

cd ..

if [ "$REASSEMBLE_SAMPLES" == "true" ]
then
    $INSTALLATION_PATH/Reassemblage/Auto_Reassembly_Module.sh $adaptivar_option_file $ANALYSIS_NAME $adaptivar_rep_temp $ANALYSIS_NAME.time_report.txt
    error_watch
fi

adaptivar_start_time=$(date +'%s')

adaptivar_start_time_step=$(date +'%s')

echo "Generating group file..."
$INSTALLATION_PATH/Nettoyage/make_group.sh $INSTALLATION_PATH $ANALYSIS_NAME
error_watch

cat *.Complete.fasta > $ANALYSIS_NAME.fasta

mv *.Complete.fasta $adaptivar_inter_folder

adaptivar_mainfile=$ANALYSIS_NAME.fasta

adaptivar_groupfile=$ANALYSIS_NAME.group

$INSTALLATION_PATH/Nettoyage/correct_seqnames.sh $ANALYSIS_NAME
error_watch

adaptivar_end_time_step=$(date +'%s')


if [ "$REASSEMBLE_SAMPLES" == "true" ]
then
    echo "# sequences after reassembly:$adaptivar_mainfile:seq:">>$adaptivar_rep_temp
fi


echo -e "\nAnalysis Preparation time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))"
echo -e "\nAnalysis Preparation time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))" >> $ANALYSIS_NAME.time_report.txt

# END OF READS PREPARATION SECTION #########################################

###############################
#   Reads Cleaning Section    #
###############################

adaptivar_start_time_step=$(date +'%s')

debug_print $adaptivar_mainfile

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "trimmed.fasta" )
$INSTALLATION_PATH/Nettoyage/Illumicut -f $adaptivar_mainfile -g $adaptivar_outfile -l $MINIMUM_LENGTH -m $MAX_MISMATCH_PRIMER -a $MAX_AMBIG_COUNT -o $MAXIMAL_PRIMER_OFFSET -t $NB_THREAD -s $ANALYSIS_NAME.trim.scrap -u $ANALYSIS_NAME.primer_trim.report.txt -p $FORWARD_PRIMER -r $REVERSE_PRIMER
error_watch
adaptivar_mainfile=$adaptivar_outfile

debug_print $adaptivar_mainfile

echo "# sequences after primers trimming:$adaptivar_mainfile:seq:">>$adaptivar_rep_temp
echo "# sequences rejected after primers trimming:$ANALYSIS_NAME.trim.scrap:seq:">>$adaptivar_rep_temp
echo "Illumicut Report file:$ANALYSIS_NAME.primer_trim.report.txt:rep:">>$adaptivar_rep_temp

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "unique.fasta" )
$INSTALLATION_PATH/Nettoyage/derIplicate -f $adaptivar_mainfile -o $adaptivar_outfile -n $ANALYSIS_NAME.trimmed.names
error_watch
adaptivar_mainfile=$adaptivar_outfile
adaptivar_namefile=$ANALYSIS_NAME.trimmed.names

debug_print $adaptivar_mainfile

echo "# sequences after dereplication:$adaptivar_mainfile:readn:$adaptivar_namefile">>$adaptivar_rep_temp

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "sized_sort.fasta" )
$INSTALLATION_PATH/Nettoyage/Size_Adder.py -fas $adaptivar_mainfile -name $adaptivar_namefile -sort -out $adaptivar_outfile
error_watch
adaptivar_mainfile=$adaptivar_outfile

debug_print $adaptivar_mainfile

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "NO_HOMOP.fasta" )
$INSTALLATION_PATH/Nettoyage/HomopRemover -f $adaptivar_mainfile -o $adaptivar_outfile -s $ANALYSIS_NAME.SCRAP_HOMOP_$MAX_HOMOP_LENGTH.fasta -l $MAX_HOMOP_LENGTH -i $MIN_SEQUENCES_SIZE -t $NB_THREAD
error_watch
adaptivar_mainfile=$adaptivar_outfile

debug_print $adaptivar_mainfile

echo "# sequences after Homopolymers filtering:$adaptivar_mainfile:reads:">>$adaptivar_rep_temp
echo "# sequences rejected by Homopolymers filtering:$ANALYSIS_NAME.SCRAP_HOMOP_$MAX_HOMOP_LENGTH.fasta:reads:">>$adaptivar_rep_temp


if [ "$ANALYSIS_TYPE" == "Cox" ]
then
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "NO_STOP.fasta" )
    $INSTALLATION_PATH/Traitement_Cox/CoxFIlter -f $adaptivar_mainfile -o $adaptivar_outfile -s $ANALYSIS_NAME.SCRAP_STOP_$MAX_STOP_CODONS.fasta -l $MAX_STOP_CODONS -t $NB_THREAD
    error_watch
    adaptivar_mainfile=$adaptivar_outfile
    echo "# sequences after Stop codons filtering:$adaptivar_mainfile:reads:">>$adaptivar_rep_temp
    echo "# sequences rejected by Stop codons filtering:$ANALYSIS_NAME.SCRAP_STOP_$MAX_STOP_CODONS.fasta:reads:">>$adaptivar_rep_temp
    debug_print $adaptivar_mainfile
fi

if [ "$CHIMERAS_FILTERING" == "true" ]
then
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "NO_CHIMERAS.fasta" )
    adaptivar_chimeras=$ANALYSIS_NAME.chimeras.fasta
    adaptivar_uchime_out=$ANALYSIS_NAME.uchime_output.txt
    if [ "$CHIMERAS_FILTERING_METHOD" == "vsearch_denovo1" ]
    then
        vsearch --uchime_denovo $adaptivar_mainfile --nonchimeras $adaptivar_outfile --chimeras $adaptivar_chimeras --uchimeout $adaptivar_uchime_out 
    fi
    if [ "$CHIMERAS_FILTERING_METHOD" == "vsearch_denovo2" ]
    then
        vsearch --uchime2_denovo $adaptivar_mainfile --nonchimeras $adaptivar_outfile --chimeras $adaptivar_chimeras --uchimeout $adaptivar_uchime_out
    fi
    if [ "$CHIMERAS_FILTERING_METHOD" == "vsearch_denovo3" ]
    then
        vsearch --uchime3_denovo $adaptivar_mainfile --nonchimeras $adaptivar_outfile --chimeras $adaptivar_chimeras --uchimeout $adaptivar_uchime_out
    fi
    error_watch
    echo "# sequences after Chimeras removal ($CHIMERAS_FILTERING_METHOD Algorithm):$adaptivar_outfile:reads:">>$adaptivar_rep_temp
    echo "# chimerical sequences:$adaptivar_chimeras:reads:">>$adaptivar_rep_temp
    adaptivar_mainfile=$adaptivar_outfile
fi



adaptivar_end_time_step=$(date +'%s')
cp $adaptivar_mainfile "$adaptivar_result_folder/OTUS_Extraction/"
echo -e "\nSequences Cleaning time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))"
echo -e "\nSequences Cleaning time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))" >> $ANALYSIS_NAME.time_report.txt

if [ "$ANALYSIS_TYPE" == "SoilExtITS1" ]
then
    adaptivar_start_time_step=$(date +'%s')
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "desized.fasta" )
    $INSTALLATION_PATH/Extraction_ITS/Size_Remover.py -fas $adaptivar_mainfile -out $adaptivar_outfile
    error_watch
    adaptivar_mainfile=$adaptivar_outfile
    adaptivar_orifile=$adaptivar_mainfile

    debug_print $adaptivar_mainfile

    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "ITS1.fasta" )
    $INSTALLATION_PATH/Extraction_ITS/extract_ITS.sh $adaptivar_option_file $adaptivar_mainfile $ANALYSIS_NAME $adaptivar_namefile $adaptivar_rep_temp
    error_watch
    adaptivar_mainfile=$adaptivar_outfile

    debug_print $adaptivar_mainfile
    
    echo "# sequences after ITS extraction:$adaptivar_mainfile:readn:$adaptivar_namefile">>$adaptivar_rep_temp
    
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "unique.fasta" )
    $INSTALLATION_PATH/Nettoyage/derIplicate -f $adaptivar_mainfile -r $adaptivar_namefile -n $ANALYSIS_NAME.ITS1.names  -o $adaptivar_outfile
    error_watch
    adaptivar_mainfile=$adaptivar_outfile
    adaptivar_namefile=$ANALYSIS_NAME.ITS1.names

    debug_print $adaptivar_mainfile

    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "sized_sort.fasta" )
    $INSTALLATION_PATH/Nettoyage/Size_Adder.py -fas $adaptivar_mainfile -name $adaptivar_namefile -sort -out $adaptivar_outfile
    error_watch
    #adaptivar_mainfile=$( add_new_extension $adaptivar_mainfile "sized_sort.fasta" )
    adaptivar_mainfile=$adaptivar_outfile

    debug_print $adaptivar_mainfile

    echo "# sequences after Dereplication ITS1:$adaptivar_mainfile:reads:">>$adaptivar_rep_temp
    
    adaptivar_end_time_step=$(date +'%s')
    cp $adaptivar_mainfile "$adaptivar_result_folder/OTUS_Extraction/"
    echo -e "\nITS Extraction time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))"
    echo -e "\nITS Extraction time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))" >> $ANALYSIS_NAME.time_report.txt

fi

# END OF READS CLEANING SECTION ####################################################

#####################################
# Vsearch Clustering Method Section #
#####################################

adaptivar_start_time_step=$(date +'%s')

if [ "$CLUSTERING_METHOD" == "vsearch" ]
then
    if [ "$ANALYSIS_TYPE" != "SoilExtITS1" ]
    then
        adaptivar_outfile=$ANALYSIS_NAME.otus.fasta
        adaptivar_upfile=$ANALYSIS_NAME.otu.up
        adaptivar_freqfile=$ANALYSIS_NAME.freq
        adaptivar_corrlist=$ANALYSIS_NAME.corrlist.txt
        adaptivar_consensefile=$ANALYSIS_NAME.otus.consense.fasta
        
        if [ "$CLUSTERING_ALGORITHM" == "cluster_size" ]
        then
            vsearch --cluster_size $adaptivar_mainfile --id $OTU_ID --iddef $OTU_ID_DEFINITION --centroids $adaptivar_outfile --consout $adaptivar_consensefile --relabel OTU_ --sizein --sizeout --uc $adaptivar_upfile --threads $NB_THREAD
        fi
        if [ "$CLUSTERING_ALGORITHM" == "cluster_unoise" ]
        then
            vsearch --cluster_unoise $adaptivar_mainfile --id $OTU_ID --minsize $MINIMUM_SIZE --iddef $OTU_ID_DEFINITION --centroids $adaptivar_outfile --consout $adaptivar_consensefile --relabel OTU_ --sizein --sizeout --uc $adaptivar_upfile --threads $NB_THREAD
        fi
        error_watch
        adaptivar_mainfile=$adaptivar_outfile
        
        if [ "$USE_CONSENSUS_OTUS" == "true" ]
        then
            adaptivar_mainfile=$adaptivar_consensefile
            cp $adaptivar_consensefile $adaptivar_consensefile.bkp
            sed -i 's/centroid=//g' $adaptivar_mainfile
            sed -i 's/;seqs=.*;size/;size/g' $adaptivar_mainfile
        fi
        
        debug_print $adaptivar_mainfile

        $INSTALLATION_PATH/Clustering/otu_info_extract_vsearch.py -up $adaptivar_upfile -group $adaptivar_groupfile -name $adaptivar_namefile -out $adaptivar_freqfile
        error_watch
        adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "new.fasta" )
        adaptivar_freqout=$( add_new_extension $adaptivar_freqfile "new.freq" )
        $INSTALLATION_PATH/Clustering/Otu_Renamer.py -fas $adaptivar_mainfile -freq $adaptivar_freqfile -fasout $adaptivar_outfile -freout $adaptivar_freqout -corout $adaptivar_corrlist
        error_watch
        adaptivar_freqfile=$adaptivar_freqout
        adaptivar_mainfile=$adaptivar_outfile
        
        debug_print $adaptivar_mainfile

    fi
    if [ "$ANALYSIS_TYPE" == "SoilExtITS1" ]
    then
        adaptivar_outfile=$ANALYSIS_NAME.otus.ITS1.fasta
        adaptivar_upfile=$ANALYSIS_NAME.otu.up
        adaptivar_freqfile=$ANALYSIS_NAME.freq
        adaptivar_corrlist=$ANALYSIS_NAME.corrlist.txt

        if [ "$CLUSTERING_ALGORITHM" == "cluster_size" ]
        then
            vsearch --cluster_size $adaptivar_mainfile --id $OTU_ID --iddef $OTU_ID_DEFINITION --centroids $adaptivar_outfile --consout $adaptivar_consensefile --relabel OTU_ --sizein --sizeout --uc $adaptivar_upfile --threads $NB_THREAD
        fi
        if [ "$CLUSTERING_ALGORITHM" == "cluster_unoise" ]
        then
            vsearch --cluster_unoise $adaptivar_mainfile --id $OTU_ID --minsize $MINIMUM_SIZE --iddef $OTU_ID_DEFINITION --centroids $adaptivar_outfile --consout $adaptivar_consensefile --relabel OTU_ --sizein --sizeout --uc $adaptivar_upfile --threads $NB_THREAD
        fi
        error_watch
        adaptivar_mainfile=$adaptivar_outfile
        $INSTALLATION_PATH/Clustering/otu_info_extract_vsearch.py -up $adaptivar_upfile -group $adaptivar_groupfile -name $adaptivar_namefile -out $adaptivar_freqfile
        error_watch
        $INSTALLATION_PATH/Extraction_ITS/Full_Sequence_Extractor_vsearch.py -otus $adaptivar_mainfile -uparse $adaptivar_upfile -orifas $adaptivar_orifile -out $ANALYSIS_NAME.otus.fullseqs.fasta
        error_watch
        adaptivar_freqout=$( add_new_extension $adaptivar_freqfile "new.freq" )
        $INSTALLATION_PATH/Clustering/Otu_Renamer.py -fas $ANALYSIS_NAME.otus.fullseqs.fasta -freq $adaptivar_freqfile -fasout $ANALYSIS_NAME.otus.fullseqs.new.fasta -freout $adaptivar_freqout -corout $adaptivar_corrlist
        error_watch
        adaptivar_freqfile=$adaptivar_freqout
        $INSTALLATION_PATH/Clustering/Otu_Renamer.py -fas $ANALYSIS_NAME.otus.ITS1.fasta -fasout $ANALYSIS_NAME.otus.ITS1.new.fasta
        error_watch
        if [ "$BLAST_TARGET" == "fullseqs" ]
        then
            adaptivar_mainfile=$ANALYSIS_NAME.otus.fullseqs.new.fasta
        fi
        if [ "$BLAST_TARGET" == "ITS1" ]
        then
            adaptivar_mainfile=$ANALYSIS_NAME.otus.ITS1.new.fasta
        fi
        debug_print $adaptivar_mainfile
    fi    
fi

# END OF VSEARCH CLUSTERING METHOD SECTION #############################################

#####################################
#  Swarm Clustering Method Section  #
#####################################

if [ «"$CLUSTERING_METHOD"» == «"swarm"» ]
then
    adaptivar_fastidious_opt=""
    adaptivar_no_otu_breaking_opt=""
    if [ "$FASTIDIOUS_MODE" == "true"  ] && [ $SWARM_DIFFERENCES -eq 1 ]
    then
        adaptivar_fastidious_opt="--ceiling $MAX_MEMORY_AVAIL --fastidious"
    fi
    if [ "$NO_OTU_BREAKING" == "true" ]
    then
        adaptivar_no_otu_breaking_opt="--no-otu-breaking"
    fi
    if [ "$ANALYSIS_TYPE" != "SoilExtITS1" ]
    then
        adaptivar_outfile=$ANALYSIS_NAME.otus.fasta
        adaptivar_upfile=$ANALYSIS_NAME.otu.up
        adaptivar_freqfile=$ANALYSIS_NAME.freq
        adaptivar_corrlist=$ANALYSIS_NAME.corrlist.txt
        adaptivar_swarm_out=$ANALYSIS_NAME.swarm_out.txt
        adaptivar_swarm_stats=$ANALYSIS_NAME.swarm_stats.txt
#        adaptivar_consensefile=$ANALYSIS_NAME.otus.consense.fasta
        

        swarm --threads $NB_THREAD --differences $SWARM_DIFFERENCES --usearch-abundance --seeds $adaptivar_outfile --uclust-file $adaptivar_upfile --output-file $adaptivar_swarm_out --statistics-file $adaptivar_swarm_stats $adaptivar_fastidious_opt $adaptivar_no_otu_breaking_opt $adaptivar_mainfile
        error_watch

        adaptivar_mainfile=$adaptivar_outfile
        
#        if [ "$USE_CONSENSUS_OTUS" == "true" ]
#        then
#            adaptivar_mainfile=$adaptivar_consensefile
#            cp $adaptivar_consensefile $adaptivar_consensefile.bkp
#            sed -i 's/centroid=//g' $adaptivar_mainfile
#            sed -i 's/;seqs=.*;size/;size/g' $adaptivar_mainfile
#        fi
        
        debug_print $adaptivar_mainfile

        $INSTALLATION_PATH/Clustering/otu_info_extract_swarm.py -up $adaptivar_upfile -group $adaptivar_groupfile -name $adaptivar_namefile -out $adaptivar_freqfile
        error_watch
        adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "new.fasta" )
        adaptivar_freqout=$( add_new_extension $adaptivar_freqfile "new.freq" )
        $INSTALLATION_PATH/Clustering/Otu_Renamer.py -fas $adaptivar_mainfile -freq $adaptivar_freqfile -fasout $adaptivar_outfile -freout $adaptivar_freqout -corout $adaptivar_corrlist
        error_watch
        adaptivar_freqfile=$adaptivar_freqout
        adaptivar_mainfile=$adaptivar_outfile
        
        debug_print $adaptivar_mainfile

    fi
    if [ "$ANALYSIS_TYPE" == "SoilExtITS1" ]
    then
        adaptivar_outfile=$ANALYSIS_NAME.otus.ITS1.fasta
        adaptivar_upfile=$ANALYSIS_NAME.otu.up
        adaptivar_freqfile=$ANALYSIS_NAME.freq
        adaptivar_corrlist=$ANALYSIS_NAME.corrlist.txt
        adaptivar_swarm_out=$ANALYSIS_NAME.swarm_out.txt
        adaptivar_swarm_stats=$ANALYSIS_NAME.swarm_stats.txt
        
        swarm --threads $NB_THREAD --differences $SWARM_DIFFERENCES --usearch-abundance --seeds $adaptivar_outfile --uclust-file $adaptivar_upfile --output-file $adaptivar_swarm_out --statistics-file $adaptivar_swarm_stats $adaptivar_fastidious_opt $adaptivar_no_otu_breaking_opt $adaptivar_mainfile
        error_watch
        adaptivar_mainfile=$adaptivar_outfile
        $INSTALLATION_PATH/Clustering/otu_info_extract_swarm.py -up $adaptivar_upfile -group $adaptivar_groupfile -name $adaptivar_namefile -out $adaptivar_freqfile
        error_watch
        # NON TESTÉ !!!!!
        $INSTALLATION_PATH/Extraction_ITS/Full_Sequence_Extractor_swarm.py -otus $adaptivar_mainfile -uparse $adaptivar_upfile -orifas $adaptivar_orifile -out $ANALYSIS_NAME.otus.fullseqs.fasta
        error_watch
        adaptivar_freqout=$( add_new_extension $adaptivar_freqfile "new.freq" )
        $INSTALLATION_PATH/Clustering/Otu_Renamer.py -fas $ANALYSIS_NAME.otus.fullseqs.fasta -freq $adaptivar_freqfile -fasout $ANALYSIS_NAME.otus.fullseqs.new.fasta -freout $adaptivar_freqout -corout $adaptivar_corrlist
        error_watch
        adaptivar_freqfile=$adaptivar_freqout
        $INSTALLATION_PATH/Clustering/Otu_Renamer.py -fas $ANALYSIS_NAME.otus.ITS1.fasta -fasout $ANALYSIS_NAME.otus.ITS1.new.fasta
        error_watch
        if [ "$BLAST_TARGET" == "fullseqs" ]
        then
            adaptivar_mainfile=$ANALYSIS_NAME.otus.fullseqs.new.fasta
        fi
        if [ "$BLAST_TARGET" == "ITS1" ]
        then
            adaptivar_mainfile=$ANALYSIS_NAME.otus.ITS1.new.fasta
        fi
        debug_print $adaptivar_mainfile
    fi 
fi

# END OF SWARM CLUSTERING METHOD SECTION ############################

adaptivar_end_time_step=$(date +'%s')
cp $adaptivar_mainfile $adaptivar_result_folder
cp $adaptivar_upfile $adaptivar_result_folder/OTUS_Extraction
cp $adaptivar_corrlist $adaptivar_result_folder/OTUS_Extraction
echo -e "\nClustering time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))"
echo -e "\nClustering time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))" >> $ANALYSIS_NAME.time_report.txt


echo "# OTUs after $CLUSTERING_METHOD clustering:$adaptivar_mainfile:reads:">>$adaptivar_rep_temp
$INSTALLATION_PATH/Rapport/Size_Histogram_Generator.py -in $adaptivar_mainfile -d -head >> $ANALYSIS_NAME.otus_histo.txt

# END OF CLUSTERING SECTION ###########################################

###################
#  BLAST SECTION  #
###################

adaptivar_start_time_step=$(date +'%s')

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "blast" )

adaptivar_otufasta=$adaptivar_mainfile

debug_print $adaptivar_mainfile

if [ "$BLAST_METHOD" == "blastn" ]
then
    echo "Blast in progress..."
    blastn -query $adaptivar_mainfile -db $DATABASE -max_target_seqs $MAX_TARGET_SEQ -outfmt "6 qseqid stitle sacc pident length bitscore" -out $adaptivar_outfile -num_threads $NB_THREAD
    error_watch
    adaptivar_mainfile=$adaptivar_outfile
fi

if [ "$BLAST_METHOD" == "blast_enh" ]
then
    $INSTALLATION_PATH/Blast/Enh_blastn.sh -q $adaptivar_mainfile -c "blastn -db $DATABASE -max_target_seqs $MAX_TARGET_SEQ -outfmt '6 qseqid stitle sacc pident length bitscore'" -o $adaptivar_outfile -p $NB_THREAD 
    error_watch
    adaptivar_mainfile=$adaptivar_outfile 
fi

if [ "$BLAST_METHOD" == "vsearch" ]
then
    #vsb_bad_opt=""
    #if [ "$BAD_SEQ_OUTPUT" == "true" ]
    #then
    #    vsb_bad_opt="--dbnotmatched "$( add_new_extension $adaptivar_mainfile "rejected.fasta" )
    #fi
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "preblast" )
    vsearch --usearch_global $adaptivar_mainfile -db $FASTA_DATABASE --userfields query+target+evalue+id+alnlen+bits --userout $adaptivar_outfile --query_cov $QUERY_COVERAGE_MIN --maxhits $MAX_TARGET_SEQ --fasta_width 0 --id $IDENTITY_MIN --threads $NB_THREAD --qmask none --dbmask none --maxaccepts $MAX_ACCEPT --maxrejects $MAX_REJECT --notrunclabels
    error_watch
    adaptivar_mainfile=$adaptivar_outfile
    debug_print $adaptivar_mainfile
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "blast" )
    $INSTALLATION_PATH/Blast/Vsearch_Blast_Converter.py -blast $adaptivar_mainfile -out $adaptivar_outfile
    error_watch
    adaptivar_mainfile=$adaptivar_outfile
    debug_print $adaptivar_mainfile
fi

if [ "$BAD_SEQ_OUTPUT" == "true" ]
then
    $INSTALLATION_PATH/Blast/Extract_Bad_Seqs_Fasta.py -fasta $adaptivar_otufasta -blast $adaptivar_mainfile -out $ANALYSIS_NAME.blast_reject.fasta
    error_watch
fi

if [ "$TRUNCATE_IDENTIFICATION_LINE" == "true" ]
then
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "blast.temp" )
    adaptivar_bkpfile=$( add_new_extension $adaptivar_mainfile "fullname_backup.blast" )
    $INSTALLATION_PATH/Blast/Format_Blast_Iden.py -blast $adaptivar_mainfile -column 2 -max_chars $MAX_IDENTIFICATION_CHARS -out $adaptivar_outfile
    error_watch
    mv $adaptivar_mainfile $adaptivar_bkpfile
    mv $adaptivar_outfile $adaptivar_mainfile
fi 

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "freq.blast" )
$INSTALLATION_PATH/Blast/Blast_Freq_Hybrider.py -blast $adaptivar_mainfile -freq $adaptivar_freqfile -out $adaptivar_outfile
error_watch
adaptivar_otufreqfile=$adaptivar_outfile
adaptivar_mainfile=$adaptivar_outfile

debug_print $adaptivar_mainfile

cp $adaptivar_mainfile $adaptivar_result_folder

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "besthits.blast" )
$INSTALLATION_PATH/Blast/BestHit_Extractor.py -blast $adaptivar_mainfile -header -out $adaptivar_outfile
error_watch
adaptivar_mainfile=$adaptivar_outfile

debug_print $adaptivar_mainfile

adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "besthits_with_missing.blast" )
$INSTALLATION_PATH/Blast/Missing_Results_Completion.py -table $adaptivar_mainfile -freq $adaptivar_freqfile  -out $adaptivar_outfile
error_watch
adaptivar_mainfile=$adaptivar_outfile

debug_print $adaptivar_mainfile

$INSTALLATION_PATH/Blast/Correct_Size_column.py -in $adaptivar_mainfile -header -out $adaptivar_mainfile
$INSTALLATION_PATH/Blast/Correct_Size_column.py -in $adaptivar_otufreqfile -header -out $adaptivar_otufreqfile
error_watch
cp $adaptivar_mainfile $adaptivar_result_folder

if [ "$ANALYSIS_TYPE" == "ITSPatho" ]
then
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "PATHO.blast" )
    $INSTALLATION_PATH/Fongique_Pathogene/BH_Pathogene_Extractor.py -in $adaptivar_mainfile -list $INSTALLATION_PATH/Fongique_Pathogene/Pathogene_List.txt -bhead -out $adaptivar_outfile
    error_watch
    cp $adaptivar_outfile $adaptivar_result_folder
fi

if [ "$ANALYSIS_TYPE" == "ITSSoil" ] || [ "$ANALYSIS_TYPE" == "SoilExtITS1" ]
then
    if [ "$COMPRESS_OTUS_BY_ACC" == "true" ]
    then
        adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "acc_compressed.blast" )
        $INSTALLATION_PATH/Fongique_Sol/Besthits_CNF.py -in $adaptivar_mainfile -aln_thr $ALIGN_LENGTH_THRESHOLD -report $ANALYSIS_NAME.Compression_Filtering.report.txt -out $adaptivar_outfile    
        error_watch
        adaptivar_mainfile=$adaptivar_outfile
    fi
    adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "with_biof.blast" )
    $INSTALLATION_PATH/Fongique_Sol/Biological_Function_Adder.py -in $adaptivar_mainfile -list $INSTALLATION_PATH/Fongique_Sol/Fungi_Trophic_States_Types.txt -silent -bhead -out $adaptivar_outfile
    error_watch
    adaptivar_mainfile=$adaptivar_outfile

    debug_print $adaptivar_mainfile
    if [ "$REMOVE_SINGLETONS_FINAL" == "true" ]
    then
        adaptivar_outfile=$( add_new_extension $adaptivar_mainfile "NO_SINGLETONS.blast" )
        awk '$2 > 1 { print $0 }' $adaptivar_mainfile >> $adaptivar_outfile
        cp $adaptivar_outfile $adaptivar_result_folder
        adaptivar_mainfile=$adaptivar_outfile
        debug_print $adaptivar_mainfile
    fi
fi      

adaptivar_end_time_step=$(date +'%s')
echo -e "\nBlast + formatting time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))"
echo -e "\nBlast + formatting time:\t$(calculate_time $((adaptivar_end_time_step - adaptivar_start_time_step)))" >> $ANALYSIS_NAME.time_report.txt


adaptivar_end_time=$(date +'%s')
echo -e "\n\nTotal Analysis time:\t$(calculate_time $((adaptivar_end_time - adaptivar_start_time)))"
echo -e "\ntotal Analysis time:\t$(calculate_time $((adaptivar_end_time - adaptivar_start_time)))" >> $ANALYSIS_NAME.time_report.txt

# END OF BLAST SECTION ###########################################

###############################
#  Report Production Section  #
###############################

adaptivar_report="$ANALYSIS_NAME.analysis_report.txt"
adaptivar_time_line=$( date )
echo "Analysis Report : $ANALYSIS_NAME" > $adaptivar_report
echo $adaptivar_time_line >> $adaptivar_report
echo "ADAPTI Version : $adaptivar_version" >> $adaptivar_report
echo -e "\n" >> $adaptivar_report

echo "OTUs size repartition:$ANALYSIS_NAME.otus_histo.txt:rep:" >>$adaptivar_rep_temp


while read line
do
    desc=$( echo $line | awk -F: '{print $1}' )
    file=$( echo $line | awk -F: '{print $2}' )
    r_type=$( echo $line | awk -F: '{print $3}' )
    name=$( echo $line | awk -F: '{print $4}' )
    debug_print "Description : $desc"
    debug_print "File Type : $file"
    debug_print "Report Type : $r_type"
    debug_print "Name file : $name"
    if [ "$r_type" == "seq" ]
    then
        seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py $file )
        echo -e "$desc : $seq_count\n" >> $adaptivar_report
    fi
    if [ "$r_type" == "rep" ]
    then
        echo -e "------------- $desc ------------\n" >> $adaptivar_report
        cat $file >> $adaptivar_report
        echo -e "--------------------------------\n" >> $adaptivar_report
    fi
    if [ "$r_type" == "reads" ]
    then
        seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py $file )
        read_count=$( $INSTALLATION_PATH/Rapport/readsc.py -in $file -size )
        echo -e "$desc : $seq_count ($read_count reads)\n" >> $adaptivar_report
    fi
    if [ "$r_type" == "readn" ]
    then
        seq_count=$( $INSTALLATION_PATH/Rapport/fastac.py $file )
        read_count=$( $INSTALLATION_PATH/Rapport/readsc.py -in $file -name $name )
        echo -e "$desc : $seq_count ($read_count reads)\n" >> $adaptivar_report
    fi
    if [ "$r_type" == "direct" ]
    then
        echo -e "$desc\n" >> $adaptivar_report
    fi
done<$adaptivar_rep_temp

# END OF REPORT PRODUCTION SECTION ############################################

##############################
#  Analysis Wrapping Section #
##############################

cp $adaptivar_report $adaptivar_result_folder
cp $ANALYSIS_NAME.time_report.txt $adaptivar_result_folder
mv $ANALYSIS_NAME.* $adaptivar_inter_folder
mv $adaptivar_inter_folder $adaptivar_result_folder
if [ "$REASSEMBLE_SAMPLES" == "true" ]
then
    mv "$ANALYSIS_NAME""_Reassembly" $adaptivar_result_folder
fi
cp $adaptivar_option_file $adaptivar_result_folder

echo -e "\n############ Analysis DONE ############\n"

# END OF PROGRAM ###############################################################



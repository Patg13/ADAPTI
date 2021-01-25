#!/bin/bash

function calculate_time(){
    i=$1
    ((sec=i%60, i/=60, min=i%60, hrs=i/60))
    timestamp=$(printf "%d:%02d:%02d" $hrs $min $sec)
    echo $timestamp
}

function debug_print(){
    if [ "$DEBUG_MODE" == "true" ]
    then
        echo "DEBUG   $1"
    fi
}


option_file=$1
analysis_name_i=$2
temp_report_file=$3
time_report_file=$4

source $option_file

misc_folder="$analysis_name_i""_Reassembly"

ech_list=$( ls *$FORWARD_SUFFIX | sed "s/$FORWARD_SUFFIX//g" )
ech_list_r=$( ls *$REVERSE_SUFFIX | sed "s/$REVERSE_SUFFIX//g" )

file_list=$( ls *$FORWARD_SUFFIX )
file_list="$file_list "$( ls *$REVERSE_SUFFIX )

if [ "$ech_list" != "$ech_list_r" ]
then
    echo "ERROR Forward files count != Reverse files count"
    echo "Problematic(s) Sample(s) List:"
    for i in $ech_list
    do
      debug_print $i
      hit=0
      for j in $ech_list_r
      do
          
          if [ «"$i"» == «"$j"» ]
          then
              #echo "$i $j"
              hit=1
              break
          fi
      done
      if [ $hit -eq 0 ]
      then
          echo -e "$i\tNo Reverse"
      fi
   done
   for i in $ech_list_r
   do
      hit=0
      for j in $ech_list
      do
          if [ «"$i"» == «"$j"» ]
          then
              #echo "$i $j"
              hit=1
              break
          fi
      done
      if [ $hit -eq 0 ]
      then
          echo -e "$i\tNo Forward"
          ech_list="$ech_list ""$i"
      fi
   done
   echo -e "------------------------\n"
   exit 1
fi

if [ "$temp_report_file" != "" ]
then 
    echo "Reading Raw samples content..."
    $INSTALLATION_PATH/Reassemblage/Reads_per_Sample_Table.sh $option_file $file_list >> $analysis_name_i.reads_per_samples.txt
    base_count=$( cat $analysis_name_i.reads_per_samples.txt | tail -n 1 | sed "s;\t; ;g" )
    echo "Raw Sequences $base_count:NONE:direct:NONE" >> $temp_report_file
fi

minoverlap_arg=""
maxoverlap_arg=""
samp_total=$( echo "$ech_list"  | wc -w )
if [ "$REASSEMBLY_METHOD" == "pandaseq" ]
then
    samp_count=0
    start_time_tot=$(date +'%s')
    if [ "$PS_MINOVERLAP" != "default" ]
    then
        minoverlap_arg="-o $PS_MINOVERLAP"
        debug_print "$minoverlap_arg"
    fi

    if [ "$PS_MAXOVERLAP" != "default" ]
    then
        maxoverlap_arg="-O $PS_MAXOVERLAP"
        debug_print "$maxoverlap_arg"
    fi
    for line in $ech_list
    do
        start_time_part=$(date +'%s')
        filename_for="$line""$FORWARD_SUFFIX"
        filename_rev="$line""$REVERSE_SUFFIX"
        output_name="$line""$FINAL_SUFFIX"
        debug_print $filename_for
        debug_print $filename_rev

        pandaseq -f $filename_for -r $filename_rev -w $output_name -k 20 -B -F -g "Reassembly_$line.log.txt" $minoverlap_arg $maxoverlap_arg
        
        debug_print $output_name
        let $[ samp_count += 1 ]
        end_time_part=$(date +'%s')
        echo "$line Reassembly DONE"
        echo "$(calculate_time $((end_time_part - start_time_part)))"
        echo -e "$samp_count / $samp_total\n"

    done

    mkdir $misc_folder
    for i in $ech_list
    do
       final_file="$i""$FINAL_SUFFIX"
       debug_print $final_file
       mv *$i.log.txt $misc_folder/
       $INSTALLATION_PATH/Reassemblage/fastq2fasta.py $final_file
       mv $final_file $misc_folder/
       mv "$i""$FORWARD_SUFFIX" $misc_folder/
       mv "$i""$REVERSE_SUFFIX" $misc_folder/
    done
    echo -e "\n"
    end_time_tot=$(date +'%s')
    echo -e "\nReassembly Total time:\t$(calculate_time $((end_time_tot - start_time_tot)))"
    if [ "$time_report_file" != "" ]
    then
        echo -e "\nReassembly Total time:\t$(calculate_time $((end_time_tot - start_time_tot)))" >> $time_report_file
    fi
fi

if [ "$REASSEMBLY_METHOD" == "pear" ]
then
    echo "pear reassembly IN PROGRESS"
fi




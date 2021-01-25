#!/bin/bash

# Reads_per_Sample_Table
# This script is part of the ADAPTI Pipeline

option_file=$1
file_list=${@:2}
source $option_file

if [ "$DEBUG_MODE" == "true" ]
then
    echo "DEBUG   Forward Suffix = $FORWARD_SUFFIX"
    echo "DEBUG   Reverse Suffix = $REVERSE_SUFFIX"
fi

ech_list=""
for line in $file_list
do
  ech_list="$ech_list ""$( echo "$line" | grep "$FORWARD_SUFFIX" | sed "s/$FORWARD_SUFFIX//g" )"
  ech_list_r="$ech_list_r ""$( echo "$line" | grep "$REVERSE_SUFFIX" | sed "s/$REVERSE_SUFFIX//g" )"
done

for_check=$( echo "$ech_list"  | wc -w )
rev_check=$( echo "$ech_list_r"  | wc -w )

if [ "$DEBUG_MODE" == "true" ]
then
    echo "DEBUG   Forward Samples Count : $for_check"
    echo "DEBUG   Reverse Samples Count : $rev_check"
fi

if [ $for_check -ne $rev_check ]
then
    echo "WARNING Forward files count != Reverse files count"
    echo "Problematics Samples count will be forced to 0"
    echo "Problematic(s) Sample(s) List:"
    for i in $ech_list
    do
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
fi
#exit

count_list=""
samp_count=0
for_count=0
rev_count=0
prob_count=0
for line in $ech_list
do
  if [ "$DEBUG_MODE" == "true" ]
  then
      echo "DEBUG   Reading sample $line"
  fi
  let $[ samp_count += 1 ]
  filename_for="$line""$FORWARD_SUFFIX"
  if [ ! -e $filename_for ]
  then
      count_list="$count_list""$( echo -e "$line*\t0\t*No forward" )\n"
      let $[ prob_count += 1 ]
      continue
  fi
  filename_rev="$line""$REVERSE_SUFFIX"
  if [ ! -e $filename_rev ]
  then
      count_list="$count_list""$( echo -e "$line*\t0\t*No Reverse" )\n"
      let $[ prob_count += 1 ]
      continue
  fi
  nb_lines=$( zcat $filename_for | wc -l )
  nb_seqs=$( echo "$((nb_lines/4))" )
  #echo $nb_seqs
  count_list="$count_list""$( echo -e "$line\t"$nb_seqs"" )\n"
done

sorted_list=$( echo -e $count_list | sort -n -k 2  | column -t )
echo -e "$sorted_list"

total=$( echo -e "$sorted_list" | awk '{ sum+=$2} END {print sum}' )
if [ $prob_count -eq 0 ]
then
    echo -e "\nTotal ($samp_count Samples)\t$total"
fi

if [ $prob_count -ne 0 ]
then
    echo -e "\nTotal ($samp_count Samples, $prob_count error)\t$total"
fi


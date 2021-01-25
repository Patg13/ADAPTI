#!/bin/bash

function calculate_time(){
    i=$1
    ((sec=i%60, i/=60, min=i%60, hrs=i/60))
    timestamp=$(printf "%d:%02d:%02d" $hrs $min $sec)
    echo $timestamp
}

custom_list=$@
file_list=""
if [ ! "$custom_list" ];
then
    file_list=$( ls *.fastq.gz )
fi

if [ "$custom_list" ];
then
   file_list=$( ls $custom_list )
fi

if [ «"$file_list"» == «""» ]
then
    echo "File list is empty"
    exit
fi

ech_list=""
for line in $file_list
do
  ech_list="$ech_list ""$( echo "$line" | grep '_R1_' | sed 's/\_R1_001.fastq.gz.*//g' )"
  ech_list_r="$ech_list_r ""$( echo "$line" | grep '_R2_' | sed 's/\_R2_001.fastq.gz.*//g' )"
done

for_check=$( echo "$ech_list"  | wc -w )
rev_check=$( echo "$ech_list_r"  | wc -w )

#echo $for_check
#echo $rev_check

if [ $for_check -ne $rev_check ]
then
    echo "ERROR Forward files count != Reverse files count"
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
   exit
fi
#exit

count_list=""
samp_count=0

start_time_tot=$(date +'%s')

for line in $ech_list
do
  start_time_part=$(date +'%s')
  filename_for="$line""_R1_001.fastq.gz"
  filename_rev="$line""_R2_001.fastq.gz"
  output_name="$line"".Complete.fastq"

  pandaseq -f $filename_for -r $filename_rev -w $output_name -k 20 -B -F -g "Reassembly_$line.log.txt"

  let $[ samp_count += 1 ]
  end_time_part=$(date +'%s')
  echo "$line Reassembly DONE"
  echo "$(calculate_time $((end_time_part - start_time_part)))"
  echo -e "$samp_count / $for_check\n"

done

echo -e "\n"
end_time_tot=$(date +'%s')

echo -e "\nReassembly Total time:\t$(calculate_time $((end_time_tot - start_time_tot)))"

mkdir Fastq
mv *.Complete.fastq Fastq/
#mv *.unpaired.fasta Fasta/
echo -e "\nREASSEMBLY DONE\n"                                             

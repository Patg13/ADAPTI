#!/bin/bash

Script_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

trap ctrl_c INT
function ctrl_c() {
    echo -e "\n\n[Ctrl-c] detected"
    echo "Verifying Processes PID"
    if [ -e .enhblast.pid_list.temp ]
    then
        echo "PIDs detected, interruption in progress..."
        while read line
        do
          #echo $line
          kill $line
       done<.enhblast.pid_list.temp
       wait
       echo -e "\nAll processes have been terminated"
    fi
    if [ ! -e .enhblast.pid_list.temp ]
    then
        echo "No PIDs detected"
    fi
    echo "Performing temporary files cleanup..."
    rm .*.temp
    echo -e "Cleanup Done\nEND OF PROGRAM"
    exit 1
}

function calculate_time(){
    i=$1
    ((sec=i%60, i/=60, min=i%60, hrs=i/60))
    timestamp=$(printf "%d:%02d:%02d" $hrs $min $sec)
    echo $timestamp
}

temp_check=$( ls .*.temp 2>/dev/null )

if [ «"$temp_check"» != «""» ]
then
    rm .*.temp
fi


HELPLINES=$(printf "\t%s\t%s\n" "-q" "Query file input (REQUIRED)" "-c" "Blast command to use  (must be between Double Quotes WITHOUT -query, -num_threads, -out ), " "-o" "Blast output filename (REQUIRED)" "-p" "Nb of processors for analysis (REQUIRED, must be > 1)")

#read -p "ATTENTE"
echo -e "\nBlast multi-thread enhancing script (V1.0)\n"
USAGE="Utilisation: 'basename $0'  [-q Query_input (REQUIRED) ] [-c Blast_command (REQUIRED) ] [ -o Blast_Output (REQUIRED) ] [-p Nb of processors (REQUIRED) ]"
blast_c_given=0
while getopts ":hq:c:p:o:" opt; do
case $opt in
h)
  echo "$USAGE"
  echo -e "\n"
  echo "$HELPLINES" 
  exit
 ;;
q)
   query_in=$OPTARG
   ;;
c)
   blast_c=$OPTARG
   blast_c_given=1
   ;;
p)
   threads=$OPTARG
   ;;
o)
   output_file=$OPTARG
   ;;
\?)
   echo "ERROR, Invalid option: -$OPTARG" >&2
   exit 1
   ;;
:)
  echo "-$OPTARG Option needs an argument." >&2
  echo "$USAGE" >&2
  exit 1
  ;;
 esac
done

if [ $# -eq 0 ];
then
   echo "ERROR, Option(s) missing" >&2
   echo "$USAGE"
   exit 1
fi

if [ ! $query_in ];
then
    echo "ERROR, query input file not specified"
    exit 1
fi

if [ ! $blast_c_given -eq 1 ];
then
    echo "ERROR, blast command not specified"
    exit 1
fi

if [ ! $threads ];
then
    echo "ERROR, no threads number specified"
    exit 1
fi

if [ ! $output_file ];
then
    echo "ERROR, no output filename specified"
    exit 1
fi

if [ ! -e $query_in ];
then
    echo "ERROR, sff file not found"
    exit 1
fi

echo -e "\nQuery input:\t$query_in"
echo -e "\nBlast Command:\t$blast_c"
echo -e "\nNumber of threads:\t$threads"
echo -e "\nOutput Filename:\t$output_file"
echo -e "\n"

echo "Veryfing Blast command..."

verif=$( echo "$blast_c" | grep -e "-query " )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -query in blast command"
    exit 1
fi
verif=$( echo "$blast_c" | grep -e "-out " )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -out in blast command"
    exit 1
fi
verif=$( echo "$blast_c" | grep -e ">" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, Redirection Char (>) in blast command"
    exit 1
fi
verif=$( echo "$blast_c" | grep -e "-num_threads " )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -num_threads in blast command option"
    exit 1
fi

command_test="$blast_c -query .verif.temp -num_threads $threads"

echo "$command_test"

touch .verif.temp
echo "#!/bin/bash" > .test.sh.temp
echo $command_test >> .test.sh.temp
chmod 700 .test.sh.temp

verif=$( ./.test.sh.temp 2>&1 >/dev/null )

rm .test.sh.temp
rm .verif.temp

#echo "$verif"

if [ «"$verif"» != «"Warning: [blastn] Query is Empty!"» ]
then
    echo "Blast internal Error, check your command"
    echo "Error Message :"
    echo $verif
    exit 1
fi

echo -e "Blast Command Valid\n"

python3 $Script_DIR/blast_filesplitter.py -fas $query_in -part $threads -list ".enhblast.filelist.temp" -suff "fasta.temp" -hide

start_time=$(date +'%s')
echo -e "\n"
count=0
while read line
do
  blast_command="$blast_c -query $line -out $line.blast.temp &"
  echo "#!/bin/bash" >> $line.thread.sh.temp
  echo $blast_command >> $line.thread.sh.temp
  echo 'task_pid=$!' >> $line.thread.sh.temp
  echo 'echo "$task_pid" >> .enhblast.pid_list.temp' >> $line.thread.sh.temp
  echo 'wait' >> $line.thread.sh.temp
  chmod 700 $line.thread.sh.temp
  echo "Running thread $count"
  let $[ count += 1 ]
  ./$line.thread.sh.temp & 
   task_pid=$!
   echo "$task_pid" >> .enhblast.pid_list.temp
done<.enhblast.filelist.temp

wait

end_time=$(date +'%s')

cat .*.blast.temp > $output_file

rm .*.temp

echo -e "Blast time:\t$(calculate_time $((end_time - start_time)))"

echo -e "\nProgram DONE"





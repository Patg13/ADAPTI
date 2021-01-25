#!/bin/bash

########################################################
#                                                      #
#       ITSx Multi-threading Enhancement Program       #
#                        (IMEP)                        #
#                                                      #
#                          by                          #
#                    Patrick Gagne                     #
#                  patg97@hotmail.com                  #
#                                                      #
########################################################

#### Description: Enhance ITSx multi-thread by calling multiple instance of ITSx on a splitted fasta file


#####################
# Functions Section #
#####################

# function ctrl_c()
# This function trap the CTRL-C kill command to clean up temporary files and still running processes before exit

trap ctrl_c INT
function ctrl_c() {
    echo -e "\n\n[Ctrl-c] detected"
    echo "Verifying Processes PID"
    if [ -e .enhITSx.pid_list.temp ]
    then
        echo "PIDs detected, interruption in progress..."
        while read line
        do
          #echo $line
          kill $line
       done<.enhITSx.pid_list.temp
       wait
       echo -e "\nAll processes have been terminated"
    fi
    if [ ! -e .enhITSx.pid_list.temp ]
    then
        echo "No PIDs detected"
    fi
    echo "Performing temporary files cleanup..."
    temp_check=$( ls .*.temp 2>/dev/null )

    if [ «"$temp_check"» != «""» ]
    then
        rm .*.temp
    fi
    echo -e "Cleanup Done\nEND OF PROGRAM"
    exit 1
}

#function calculate_time()
# This function take a number of seconds as argument and convert it in a human readable format (hrs:min:sec)
function calculate_time(){
    i=$1
    ((sec=i%60, i/=60, min=i%60, hrs=i/60))
    timestamp=$(printf "%d:%02d:%02d" $hrs $min $sec)
    echo $timestamp
}

#function check_Bool(){}
#function check_Value(){}
#function check_String(){}
#function check_S_string(){}


# Check if there is residual hidden temporary files from an old analysis
# and remove them if there is
temp_check=$( ls .*.temp 2>/dev/null )

if [ «"$temp_check"» != «""» ]
then
    rm .*.temp
fi

############################
# Options Parsing Section  #
############################

version="ITSx Multi-threading Enhancement Program Version 1.0\nBy: Patrick Gagne\n" 

HELPLINES=$(printf "\t%s\t%s\n" "-f" "Fasta file input <string> (REQUIRED)" "-c" "ITSx command to use <ITSx ...>  (must be between Double Quotes WITHOUT -i, -o, --multi_thread, --cpu, -t, --save_regions ), " "-o" "Fasta output prefix <string> (ex filename without extension) (REQUIRED)" "-t" "Nb of threads for analysis <int> (REQUIRED, must be > 1)" "-g" "List of organism group <list> (Check ITSx manual for organism list (-t) and use this syntax (a,b or fungi,oomycota !NO QUOTES!) default:all" "-s" "Region to save <list> (Check ITSx manual for region list (--save_region) and use the same syntax (NO QUOTES)  (default:ITS1)" "-v" "Display Version Informations" "-h" "Display help")

echo -e "\nITSx multi-thread enhancing script (V1.0)\n"
USAGE="Utilisation: 'basename $0'  [-f Fasta_input (REQUIRED) ] [-c ITSx_command (REQUIRED) ] [ -o ITSx_Output_prefix (REQUIRED) ] [-t Nb of threads (REQUIRED) ] [-g list of organism groups (Use the same notation as ITSx -t option) (default: all) ] [-s Regions to save (Use the same notation as ITSx --save_regions) (default:SSU,ITS1)]"
itsx_c_given=0
warning_stop=0
warning_message=""
while getopts ":hf:c:t:o:g:s:v:" opt; do
case $opt in
h)
  echo "$USAGE"
  echo -e "\n"
  echo "$HELPLINES" 
  exit
 ;;
f)
   fasta_in=$OPTARG
   ;;
c)
   itsx_c=$OPTARG
   itsx_c_given=1
   ;;
t)
   threads=$OPTARG
   ;;
o)
   output_file=$OPTARG
   ;;
g)
   species_list=$OPTARG
   ;;
s)
   save_regions=$OPTARG
   ;;
v)
   echo -e "$version"
   exit
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

############################
# Options Checking Section # 
############################

# This section is to make sure that all program options (NOT ITSx options)
# are correct and that their values are valids

if [ $# -eq 0 ];
then
   echo "ERROR: Option(s) missing" >&2
   echo "$USAGE"
   exit
fi

if [ ! $fasta_in ];
then
    echo "ERROR: Fasta input file not specified"
    exit
fi

if [ ! $itsx_c_given -eq 1 ];
then
    echo "ERROR: ITSx command not specified"
    exit
fi

if [ ! $threads ];
then
    echo "ERROR: No number of threads specified"
    exit
fi

if [ ! $species_list ];
then
    echo "No organism list specified. Using default value: (all)"
    species_list="all"
fi

if [ ! $save_regions ];
then
    echo "No regions save specified. Using default value: (all)"
    save_regions="all"
fi

if [ ! $output_file ];
then
    echo "ERROR: no output filename specified"
    exit
fi

if [ ! -e $fasta_in ];
then
    echo "ERROR: sff file not found"
    exit
fi

if [ $threads -le 0 ]
then
    echo "ERROR:Number fo threads must be an integer > 1"
    exit
fi

if [ $threads -eq 1 ]
then
    echo -e "ERROR: this program can only be used for ITSx multi-thread acceleration.\nSingle thread mode is not supported\n"
    exit
fi

if [[ $species_list == *.* ]]
then
    warning_stop=1
    warning_message="$warning_message Dot (.) used in organism selection, it will be replaced by all\n"
    species_list="all"
fi

if [ -e $output_file/ ]
then
    echo "ERROR: $output_file folder already exist"
    exit
fi

sp_check=$( echo "$species_list" | egrep -o -i -w -e "all" )
sr_check=$( echo "$save_regions" | egrep -o -i -w -e "all" )

if [[ $sp_check != "" ]]
then
     if [[ $species_list != $sp_check ]]
     then
         warning_stop=1
         warning_message="$warning_message WARNING: selected Organisms list contain all, the list will be replaced by all\n"
         species_list="all"
     fi
     species_list="all"
fi

if [[ $sr_check != "" ]]
then
     if [[ $save_regions != $sr_check ]]
     then
         warning_message="$warning_message WARNING: selected Save regions list contain all, the list will be replaced by all\n"
         save_regions="all"
     fi
     save_regions="all"
fi

echo -e "\nFasta input:\t$fasta_in"
echo -e "\nITSx Command:\t$itsx_c"
echo -e "\nNumber of threads:\t$threads"
echo -e "\nOutput Prefix:\t$output_file"
echo -e "\nOrganism List:\t$species_list"
echo -e "\nSave Region:\t$save_regions"
echo -e "\n"

Script_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

######################################
# ITSx Command Verification Section  #
######################################

# This section will check the ITSx command provided by user and will make sure that all options are valid
# All options checking routines have been made using the ITSx User's guide : http://microbiology.se/publ/itsx_users_guide.pdf 

# As this program need to control some options, this section will check the ITSx command and return an error
# if a program-controlled option is used

echo "Veryfing ITSx command..."

verif=$( echo "$itsx_c" | grep -e "-i\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -i in ITSx command"
    exit
fi

verif=$( echo "$itsx_c" | grep -e "-h\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -h in ITSx command"
    exit
fi

verif=$( echo "$itsx_c" | grep -e "--help\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, --help in ITSx command"
    exit
fi
verif=$( echo "$itsx_c" | grep -e "-o\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -o in ITSx command"
    exit
fi
verif=$( echo "$itsx_c" | grep -e ">\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, Redirection Char (>) in ITSx command"
    exit
fi
verif=$( echo "$itsx_c" | grep -e "--multi_thread\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, --multi_thread in ITSx command option"
    exit
fi

#-i, -o, --multi_thread, --cpu, -t, --save_regions
verif=$( echo "$itsx_c" | grep -e "--cpu\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, --cpu in ITSx command option"
    exit
fi

verif=$( echo "$itsx_c" | grep -e "-t\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, -t in ITSx command option"
    exit
fi

verif=$( echo "$itsx_c" | grep -e "--save_regions\s" )
if [ «"$verif"» != «» ]
then
    echo "ERROR, --save_regions in ITSx command option"
    exit
fi

# The --silent T or it's absence will not trigger an error, but will activate the warning_stop flag
# this flag will force the program to wait until the user press [RETURN]
verif=$( echo "$itsx_c" | grep -e "--silent\s" )
if [ «"$verif"» == «""» ]
then
    warning_stop=1
    warning_message="$warning_message It's recommended to have --silent F in ITSx command (less screen print from each threads)\n"
fi


# The next three variables are the corrects arguments for ITSx -t arguments, --save_regions arguments and all the others options
valid_save_regions="SSU ITS1 5.8S ITS2 LSU all none"
valid_species="alveolata a bryophyta b bacillariophyta c amoebozoa d euglenozoa e fungi f chlorophyta g rhodophyta h phaeophyceae i marchantiophyta l metazoa m oomycota o haptophyceae p raphidophyceae q rhizaria r synurophyceae s tracheophyta t eustigmatophyceae u apusozoa x parabasalia y all"
valid_options_wa="-i=string -o=string -p=string --date=bool --reset=bool -t=string -E=value -S=value -N=value --selection_priority=s_string --search_eval=value --search_score=value --allow_single_domain=s_string --allow_reorder=bool --complement=bool --cpu=value --multi_thread=bool --heuristics=bool --summary=bool --graphical=bool --fasta=bool --preserve=bool --save_regions=s_string --anchor=s_string --partial=value --concat=bool --minlen=value --table=bool --detailed_results=bool --not_found=bool --truncate=bool --silent=bool --graph_scale=value --saw_raw=bool"


# If an organism specified with the -g option is not in the valid_species list
# the program will throw an error and exit
if [[ $species_list != "all" ]]
then
    check_sp=$( echo "$species_list" | tr "," " " )
    for i in $check_sp
    do
        egrep_result=$( echo "$valid_species" | egrep -o -i -w -e "$i" )
        if [ «"$egrep_result"» == «""» ]
        then
            echo "ERROR: invalid organism ($i)"
            exit
        fi
    done
        
fi

# If a save region specified with the -s option is not in the valid_species list
# the program will throw an error and exit
if [[ $save_regions != "all" ]]
then
    check_sp=$( echo "$save_regions" | tr "," " " )
    for i in $check_sp
    do
        egrep_result=$( echo "$valid_save_regions" | egrep -o -i -w -e "$i" )
        if [ «"$egrep_result"» == «""» ]
        then
            echo "ERROR: invalid save_regions argument ($i)"
            exit
        fi
    done
        
fi
  
# This is for splitting all the options in the user-definied ITSx command
opt_split=$( echo "$command_test" | tr "-" " " | sed 's/   /;/g' | sed 's/  /:/g' | sed 's/ /=/g' | sed 's/;/ --/g' | sed 's/:/ -/g' )

arr_vsp=($valid_species)
arr_com=($opt_split)


# The following loop will iterate throught every options found in the user-defined ITSx command
# and will identify any wrong options
# For now IT DOESN'T confirm that the values types are corrects
# Il will be done in V2.0
for check_option in $opt_split
do
  if [ «"$check_option"» == «"ITSx"» ]
  then
      continue
  fi
  option_def=$( cut -d "=" -f 1 <<< $check_option )
  option_arg=$( cut -d "=" -f 2 <<< $check_option )

  # Check if the --silent option is present and that it's argument isn't F
  # in such case, the program will trigger the warning flag
  if [ «"$option_def"» == «"--silent"» ]
  then
      corr=$( echo "$option_arg" | sed 's/f/F/g' )
      if [ «"$corr"» == «"F"» ]
      then
          warning_stop=1
          warning_message="$warning_message Using --silent F option is not recommended (lot of terminal print from each threads)\n"
      fi
  fi
  #echo $option_def
  #echo $option_arg
  if [ «"$option_def"» == «"$option_arg"» ]
  then
      echo "ERROR, Option without argument ($option_def)"
      exit
  fi
  patt="$option_def""=""[^ ]+"
  #echo $patt
  option_req=$( echo "$valid_options_wa" | egrep -o -e "$patt" )
  #echo $option_req
  if [ «"$option_req"» == «""» ]
  then
      echo "ERROR: ITSx option invalid ($option_def)"
      exit
  fi  
done
  

# If one or more warning have been triggered, the program will print the warning messages
# and will wait for the user to press [RETURN] to continue or [CTRL-C] to exit the program
# for now it's not possible to bypass the Warning triggering
# An option to bypass warning stop will be implemented in the V2.0
if [ $warning_stop == 1 ]
then
    echo -e "--------WARNING----------:\n$warning_message\n"
    read -p "Press [RETURN] to continue or [CTRL-C] to quit."
fi

echo -e "\nITSx Command Valid\n"


# The program use a custom python3 script ( by: Patrick Gagne )
# to split the fasta infile into x parts ( x is the number of threads )
python3 $Script_DIR/ITSx_filesplitter.py -fas $fasta_in -part $threads -list ".enhITSx.filelist.temp" -suff "fasta.temp" -hide

start_time=$(date +'%s') # This variable is for the algorithm time comsumption calculation

echo -e "\n"
count=0
# the m_script (or master script) will be use to launch all temporary scripts in their respectives folders
touch .$output_file.m_script.sh.temp
echo "#!/bin/bash" >> .$output_file.m_script.sh.temp

# This loop will create a temporary bash script for each fasta file and will
# move all files into their respectives folders ( each thread have a folder )
# As ITSx generate temporary files, this isolation is mandatory
while read line
do
  mkdir $output_file.$count
  ITSx_command="$itsx_c -i $line -o "$output_file.$count" -t $species_list --save_regions $save_regions"" &"
  echo "#!/bin/bash" >> $line.thread.sh.temp
  echo $ITSx_command >> $line.thread.sh.temp
  echo 'task_pid=$!' >> $line.thread.sh.temp
  echo 'echo "$task_pid" >> ../.enhITSx.pid_list.temp' >> $line.thread.sh.temp
  echo 'wait' >> $line.thread.sh.temp
  chmod 700 $line.thread.sh.temp
  mv $line.thread.sh.temp $output_file.$count/
  mv $line $output_file.$count/
  echo "cd $output_file.$count" >> .$output_file.m_script.sh.temp
  echo "./$line.thread.sh.temp &" >> .$output_file.m_script.sh.temp
  echo "cd .." >> .$output_file.m_script.sh.temp
  let $[ count += 1 ]
done<.enhITSx.filelist.temp

echo "wait" >> .$output_file.m_script.sh.temp
chmod 700 .$output_file.m_script.sh.temp

./.$output_file.m_script.sh.temp

# This portion of code is for regrouping files into singles ones
if [ «"$save_regions"» == «"all"» ]
then
    recomb_list="SSU ITS1 5_8S ITS2 LSU"
fi

if [ «"$save_regions"» != «"all"» ]
then
    recomb_list=$( echo "$save_regions" | tr "," " " | tr '[a-z]' '[A-Z]' )
fi

echo $recomb_list

for i in $recomb_list
do
    touch $output_file.$i.fasta
    count=0
    while read line
    do
        cat ./$output_file.$count/$output_file.$count.$i.fasta >> $output_file.$i.fasta
        let $[ count += 1 ]
    done<.enhITSx.filelist.temp
done
   
   


mkdir $output_file

mv $output_file.*/ $output_file

mv $output_file.*.fasta $output_file

cd $output_file

find . -name ".*.temp" -type f -delete

cd ..

end_time=$(date +'%s')

#cat .*.fasta.temp > $output_file

rm .*.temp

echo -e "\nITsx analysis time:\t$(calculate_time $((end_time - start_time)))"

echo -e "\nProgram DONE"





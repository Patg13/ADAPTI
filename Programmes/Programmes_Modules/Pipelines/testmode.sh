#!/bin/bash

##################################################################
#								 #
#		     ADAPTI Option File Tester			 #
#								 #
#        	 WARNING ! EXPERIMENTAL ! WARNING         	 #
#								 #
#								 #
#                     by : Patrick Gagne			 #
#								 #
#	          Version 0.1 (october 4 2018)			 #
#								 #
##################################################################

set -a

function check_dependency(){
    echo "Checking $1"
    check_line=$( type $1 2>&1)
    if [ $? -ne 0 ]
    then
        echo "ERROR: $1 not found in PATH"
        exit 1
    fi
    echo $check_line
}

function error_watch(){
    if [ $? -ne 0 ]
    then
        echo "ERROR: Catastrophic failure detected in ADAPTI pipeline"
        echo "ADAPTI HALTED"
        exit 2
    fi
}

option_file=$1

if [ ! $option_file ]
then
    echo "ERROR: Options File not specified"
    exit 1
fi

if [ ! -e $option_file ]
then
    echo "ERROR: $option_file not found"
    exit 1
fi

echo "Verifying Options file $option_file"
echo "NOT IMPLEMENTED"

echo "Verifying Dependencies (depending on options)"
echo "NOT IMPLEMENTED"

echo -e "\n-------Verifying Dependencies (Full check)-------\n"
check_dependency pandaseq
check_dependency vsearch
check_dependency swarm
check_dependency blastn

set


# Exit code 255 = Not implemented Error
exit 255

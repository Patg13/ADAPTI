#!/bin/bash

##################################################################
#								 #
#	  Script correction des noms de sequences ADAPTI 	 #
#								 #
#                  Dependance optionelle ADAPTI	  		 #
#								 #
#								 #
#                     Par : Patrick Gagne			 #
#								 #
#		    Version 2.5 (27 mai 2017)	        	 #
#								 #
##################################################################

# Version 2

nom_analyse=$1
echo "Correcting..."
sed -i -e 's/:/_/g' -e 's/-/_/g' -e 's/ /_/g' $nom_analyse.fasta &
sed -i -e 's/:/_/g' -e 's/-/_/g' -e 's/ /_/g' $nom_analyse.group &
wait
echo "Correction DONE"

# ADAPTI (Automatic DNA Analysis and Processing Tool for Illumina) V1.0
An open-source metabarcoding pipeline designed to work with fungi ITS1 pathology datasets


ADAPTI publication reference (french) :

Gagné Patrick, Hardy, S., Bérubé Jean, & Université Laval. Faculté des sciences et de génie. (2020). 
Élaboration d’une méthodologie d’analyses bio-informatiques pour des données de séquençage illumina dans un contexte de metabarcoding (dissertation). 
Université Laval.
Link : http://hdl.handle.net/20.500.11794/66444

To install ADAPTI (no admin right needed), launch the configuration script:
./Configure_ADAPTI.sh

This will compile ADAPTI's internal programs and edit the option files to include the installation folder.

ADAPTI's dependencies:
This pipeline was designed to use less dependencies as possible, but you must install external programs that ADAPTI uses.

-**vsearch v2.14.2+ https://github.com/torognes/vsearch (if you want to use vsearch for the blast and the reassembly step without extracting the ITS1 prior to clustering, this is the only dependency you need)**

-**pandaseq v2.11 https://github.com/neufeld/pandaseq (can be replaced by pear https://github.com/xflouris/PEAR.git or vsearch)**

-**swarm v2.2.2+ https://github.com/torognes/swarm (can be replaced by vsearch) - VERSION 3.0 NOT TESTED**

-**blast+ v2.9.0+ https://ftp.ncbi.nlm.nih.gov/blast/executables/blast+/LATEST/ (can be replaced by vsearch)**

-**ITSx v1.1.2 https://microbiology.se/sw/ITSx_1.1.2.tar.gz (can be ignored if your don't want to extract ITS1 of if you don't use ITS1)**

NOTICE: Do not use anaconda3 to install vsearch, some version contains bugs that can break your analysis depending of your options

Some of my C++ programs uses the tclap library http://tclap.sourceforge.net/ for option parsing and it's included with the source codes.

To launch the pipeline copy the file /home/ubuntu/ADAPTI/Programmes/Programmes_Modules/Pipelines/ADAPTI.sh in your analysis folder (where your fastq.gz files are located)
and copy one of the option files in the same folder ex: /home/ubuntu/ADAPTI/Programmes/Programmes_Modules/Pipelines/optionfile_ADAPTI_Def.txt

Edit the option file and include your database (fasta or blast depending of your blasting method), your sequence's prefix, your forward and reverse primer your number of threads, etc.

To launch ADAPTI :
./ADAPTI.sh optionfile.txt
It's recommended to launch it with screen if you are working remotely
screen -L ./ADAPTI.sh optionfile.txt


##TO DO:

**COMMIT THE FIRST VERSION OF ADAPTI (WILL BE DONE BY JANUARY 27 2021)**

Checkpoint system (for now, you have to relaunch the entire pipeline in case of error)

Databases and dependencies Checking

Pipeline test script (to make sure everything is correctly installed)

Some python program are old and take too much RAM, I must rewrite them

Add support for trimmomatic (if you don't want to use Illumicut, which need your primers)

Add a new step to filter bad quality sequences (in pathology, we don't do it but maybe you want to do it)

Create a singularity recipe to install the dependencies and the pipeline

Write a better guide where every option in the optionfiles are described (sorry for that...)

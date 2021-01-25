#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys,os
import argparse


print("Dereplicated Sequence Size Adder")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Add the size= tag to each dereplicated sequences')

parser.add_argument("-fas", dest="fasta_file", help=("Fasta file containing dereplicated sequences [REQUIRED]"))
parser.add_argument("-sep", dest="sep_char", default=';', help=("Seperator to be use in sequence name (ex: name;size=10) [DEFAULT= ;]"))
parser.add_argument("-out", dest="output", help=("Output filename [REQUIRED]"))

#args=parser.parse_args('-fas virginieITS.sized_sort.NO_HOMOP.fasta -out virginieITS.desized.NO_HOMOP.fasta'.split())
args=parser.parse_args()                              

#args=parser.parse_args() 

if str(args.output) == 'None':
    print('ERREUR, Aucun nom de fichier de sortie specifie')

while True:
    try:
        fichier1=args.fasta_file
        fasta = open(fichier1, 'r')
    except IOError:
        print("Desole, ce fichier est introuvable")
        sys.exit(1)
    else:
        break

sep=str(args.sep_char)



fastaL=fasta.readlines()

fasta.close()


savefile=open(str(args.output), 'w')
for line in fastaL:
    corr_line=""
    if line[0] == ">":
        corr_line=line.split(sep+"size=")[0]+"\n"
        savefile.write(corr_line)
    else:
        savefile.write(line)
        
savefile.close()

##savefile=open(str(args.output), 'w')

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys,os
import argparse

print("ITS1 Sequences Completion Program (Fasta-Fasta Version)")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Retrieve the full sequences from original fasta files and replace the corresponding ITS1 sequence using new fasta file')

parser.add_argument("-fas", dest="fasta_file", help=("Fasta file containing partial sequences [REQUIRED]"))
parser.add_argument("-orifas", dest="original_fasta", help=("Original fasta file containing full sequences [REQUIRED]"))
parser.add_argument("-out", dest="output", help=("Output filename [REQUIRED]"))

#args=parser.parse_args('-fas OTU_5.fasta -orifas /data/home2/pagagne/Analyse_Myriam_aout2016/Analyse/Analyse_Myriam_ITS_15aout2016.trimmed.fasta -out OTU_5.fullseqs.fasta'.split())
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

while True:
    try:
        fichier3=args.original_fasta
        ori_fasta = open(fichier3, 'r')
    except IOError:
        print("Desole, ce fichier est introuvable")
        sys.exit(1)
    else:
        break

print("Reading fasta file...")
fastaR=fasta.read()
print("Reading Original Fasta File")
ori_fastaR=ori_fasta.read()


ori_fasta.close()
fasta.close()


print("Mapping original fasta sequences")
ori_fastaS=ori_fastaR.split(">")
ori_fastaS.pop(0)
ori_fastaR=None

seq_dict={}

for i in ori_fastaS:
    sep=i.split("\n",1)
    seqname=sep[0].split(";size=")[0]
    seq=sep[1].replace("\n","")
    seq_dict[seqname]=seq

ori_fastaS=""


fastaS=fastaR.split(">")
fastaR=None
fastaS.pop(0)

print("Extracting Full sequences and writing in output file")

savefile=open(args.output,"w")
for i in fastaS:
    sep=i.split("\n",1)
    seqname=sep[0]
    nosize=seqname.split(";size=")[0]
    full_seq=seq_dict[nosize]
    savefile.write(">"+seqname+"\n")
    savefile.write(full_seq+"\n")

savefile.close()

print("Program DONE")

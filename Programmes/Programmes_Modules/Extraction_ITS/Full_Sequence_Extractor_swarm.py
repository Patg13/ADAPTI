#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys,os
import argparse

print("ITS1 Sequences Completion Program (for swarm based clustering)")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Retrieve the full sequences from original fasta files and replace the corresponding ITS1 sequence using uparse table')

parser.add_argument("-otus", dest="otus_file", help=("Fasta file containing OTUs [REQUIRED]"))
parser.add_argument("-uparse", dest="uparse_file", help=("Uparse table containing OTUs --- Sequences correspondances in vsearch format [REQUIRED]"))
parser.add_argument("-orifas", dest="original_fasta", help=("Original fasta file containing full sequences [REQUIRED]"))
parser.add_argument("-out", dest="output", help=("Output filename [REQUIRED]"))

#args=parser.parse_args('-otus Analysis7709_SoilExtITS1_05Oct2018_10h14.otus.ITS1.fasta -uparse Analysis7709_SoilExtITS1_05Oct2018_10h14.otu.up -orifas Analysis7709_SoilExtITS1_05Oct2018_10h14.trimmed.unique.sized_sort.NO_HOMOP.desized.fasta -out fullseqs.fasta'.split())
args=parser.parse_args()                              

#args=parser.parse_args() 

if str(args.output) == 'None':
    print('ERREUR, Aucun nom de fichier de sortie specifie')

while True:
    try:
        fichier1=args.otus_file
        otus = open(fichier1, 'r')
    except IOError:
        print("Desole, ce fichier est introuvable")
        sys.exit(1)
    else:
        break

while True:
    try:
        fichier2=args.uparse_file
        uparse = open(fichier2, 'r')
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


otusR=otus.read()


uparseL=uparse.readlines()

ori_fastaR=ori_fasta.read()

uparse.close()
ori_fasta.close()
otus.close()

uparse_otu_lines=[]

print("Extracting otu lines from uparse table")
for i in uparseL:
    spl = i.split("\t")
    if spl[0] == "S":
        uparse_otu_lines.append(i.replace("\n",""))
uparseL=None

print("Mapping original fasta sequences")
ori_fastaS=ori_fastaR.split(">")
ori_fastaS.pop(0)
ori_fastaR=None

seq_dict={}

for i in ori_fastaS:
    sep=i.split("\n",1)
    seqname=sep[0]
    seq=sep[1].replace("\n","")
    seq_dict[seqname]=seq


otus_dict={}
print("Mapping Otus correspondance")
otu_nb=0
for i in uparse_otu_lines:
    spl=i.split("\t")
    otu_nb+=1
    upseqname=spl[8].split(";size=")[0]
    ori_seq=seq_dict[upseqname]
    otus_dict[upseqname]=ori_seq

otusS=otusR.split(">")
otusR=None
otusS.pop(0)

print("Extracting Full sequences and writing in output file")

savefile=open(args.output,"w")
for i in otusS:
    sep=i.split("\n",1)
    seqname=sep[0]
    nosize=seqname.split(";size=")[0]
    full_seq=otus_dict[nosize]
    savefile.write(">"+seqname+"\n")
    savefile.write(full_seq+"\n")

savefile.close()


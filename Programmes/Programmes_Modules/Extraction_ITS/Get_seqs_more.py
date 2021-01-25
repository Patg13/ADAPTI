#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("Full sequences finder & extractor")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Full sequences finder & extractor')

parser.add_argument("-fasta", dest="fasta_file", help=("Fasta file containing sequences parts (ex: ITS1)[REQUIS]"))
parser.add_argument("-full", dest="full_file", help=("Fasta file containing full sequences [REQUIS]"))
parser.add_argument("-out", dest="output", help=("Output filename [REQUIS]"))


#args=parser.parse_args('-fasta Multitest.ITS1.centroids.phylip.an.0.03.pick.0.03.rep.fasta -full Multitest.fasta -out result.fasta'.split())
args=parser.parse_args()

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
        fichier1=args.full_file
        full = open(fichier1, 'r')
    except IOError:
        print("Desole, ce fichier est introuvable")
        sys.exit(1)
    else:
        break

fastaL=fasta.readlines()
fullL=full.readlines()

iden_list=[]

for line in fastaL:
    if line[0] == ">":
        if "\n" in line:
            iden_list.append(line[1:-1])
        else:
            iden_list.append(line[1:])
    else:
        continue

full_dict={}
x=0
seq=""
for line in fullL:
    if line[0] == ">" and x == 0:
        x=1
        seqname=line[1:-1].split("\t")[0]
        continue
    if line[0] == ">" and x != 0:
        full_dict[seqname]=seq
        seqname=line[1:-1].split("\t")[0]
        seq=""
    else:
        if "\n" in line:
            seq=seq+line[:-1]
        else:
            seq=seq+line

full_dict[seqname]=seq
seqname=line[1:-1].split("\t")[0]

line_list=[]

for seqs in iden_list:
    info=""
    seq=full_dict[seqs.split("\t")[0]]
    info=">"+str(seqs)+"\n"+str(seq)+"\n"
    line_list.append(info)

savefile=open(str(args.output), 'w')
for i in line_list:
    savefile.write(i)

savefile.close()

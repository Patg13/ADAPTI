#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("\nBlast Missing Sequences Extractor")
print("By: Patrick Gagne")


parser=argparse.ArgumentParser(description='This program extract every sequences missing in a blast and create a new fasta file')

parser.add_argument("-blast", dest="blast_file", required=True, help=("Blast file using format outfmt 6 [REQUIRED]"))
parser.add_argument("-fasta", dest="fasta_file", required=True, help=("fasta file used for blasting [REQUIRED]"))
parser.add_argument("-out", dest="output", required=True, help=("Fasta Output filename [REQUIRED]"))

#args=parser.parse_args("-blast result.blast -fasta test.fasta -out badseq.fasta".split())
args=parser.parse_args()

blast_dict={}
with open(args.blast_file,'r') as blast:
    for line in blast:
        iden=line.split("\t",1)[0]
        blast_dict[iden]=1

fasta=open(args.fasta_file,'r')
fastaS=fasta.read().split(">")[1:]
fasta.close()


savefile=open(args.output,'w')
for i in fastaS:
    seqname=i.split("\n",1)[0]
    try:
        veri=blast_dict[seqname]
    except KeyError:
        savefile.write(">"+i)
savefile.close()

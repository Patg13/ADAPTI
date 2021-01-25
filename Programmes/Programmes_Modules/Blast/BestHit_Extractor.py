#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("\nBlast Best hit Extractor")
print("By: Patrick Gagne")


parser=argparse.ArgumentParser(description='This program extract the first line (best hit) of each blast sequences')

parser.add_argument("-blast", dest="blast_file", required=True, help=("Blast file using format outfmt 6 [REQUIS]"))
parser.add_argument("-header", action='store_true', help=("First line of blast file is a header line"))
parser.add_argument("-out", dest="output", required=True, help=("Blast Output filename [REQUIS]"))


#args=parser.parse_args('-blast virginieITS.EXP_METHOD_ITS.freq.blast -out virginieITS.besthits.blast'.split())
args=parser.parse_args()


while True:
    try:
        fichier1=str(args.blast_file)
        blast = open(fichier1, 'r')
    except IOError:
        print("Error, %s cannot be found"%(fichier1))
        sys.exit(1)
    else:
        break

blastL=blast.readlines()

print("Regrouping Blast hits")
x=0
blast_temp=[]
blast_regroup=[]
for line in blastL:
    if x == 0:
        x=1
        blast_temp.append(line)
        leading=line.split("\t")[0]
        continue
    if line.split("\t")[0] != leading:
        blast_regroup.append(blast_temp)
        blast_temp=[]
        leading=line.split("\t")[0]
        blast_temp.append(line)
        continue
    else:
        blast_temp.append(line)

blast_regroup.append(blast_temp)
blast_temp=[]



savefile=open(args.output,"w")
if args.header :
    infoline=blast_regroup.pop(0)
    savefile.write(infoline[0])
for i in blast_regroup:
    lines=i[0].split("\n")[0]
    lines+="\n"
    savefile.write(lines)

savefile.close()

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("\nBlast --- OTUs frequencies table hybrider V2")
print("By: Patrick Gagne")


parser=argparse.ArgumentParser(description='This program mix first line of each hits with their sample frequencies ( from freq table )')

parser.add_argument("-blast", dest="blast_file", required=True, help=("Blast file using format outfmt 6 [REQUIS]"))
parser.add_argument("-freq", dest="freq_file", required=True, help=("Fastq file [REQUIS]"))
parser.add_argument("-header", dest="header_line",default='Otu,Identification,#Accession,%identite,Longueur,BitScore', help=("Column names in blast (using this format: 'col1,col2,col3') [Default: 'Otu,Identification,#Accession,Identite,Longueur,BitScore']"))
parser.add_argument("-out", dest="output", required=True, help=("Blast Output filename [REQUIS]"))


#args=parser.parse_args('-blast virginieITS.EXP_METHOD_ITS.blast -header "Otu,Iden,Sim,Len,Score" -freq virginieITS.new.freq -out hybrid.blast'.split())
args=parser.parse_args()                              


while True:
    try:
        fichier1=args.blast_file
        blast = open(fichier1, 'r')
    except IOError:
        print("Desole, le fichier %s est introuvable"%(fichier1))
        sys.exit(1)
    else:
        break

while True:
    try:
        fichier2=args.freq_file
        freq = open(fichier2, 'r')
    except IOError:
        print("Desole, le fichier %s est introuvable"%(fichier2))
        sys.exit(1)
    else:
        break

blastL=blast.readlines()
freqL=freq.readlines()

blast.close()
freq.close()

dir_line=freqL.pop(0)
freq_dict={}
for i in freqL:
    spl=i.split("\t",1)
    freq_dict[spl[0]]=spl[1].strip("\n")



savefile=open(args.output,'w')

dir_line=dir_line.split("\t",1)[1]

header_line=str(args.header_line).replace(",","\t").replace("'","").replace('"','')+"\t"

if len(header_line[:-1].split("\t")) != len(blastL[0].split("\t")):
    print("\nWARNING: The header line used is not the same length as in blast file\n")
    
dir_line=header_line+dir_line

savefile.write(dir_line)

x=0
leading_otu=""
for line in blastL:
    otu_iden=line.split("\t")[0].split(";size=")[0]
    if x == 0:
        leading_otu=otu_iden
        newline=line.strip("\n")+"\t"+freq_dict[otu_iden]+"\n"
        x=1
        savefile.write(newline)
        continue
    if otu_iden != leading_otu:
        leading_otu=otu_iden
        newline=line.strip("\n")+"\t"+freq_dict[otu_iden]+"\n"
        savefile.write(newline)
    else:
        savefile.write(line)

savefile.close()

print("Hybridation DONE")

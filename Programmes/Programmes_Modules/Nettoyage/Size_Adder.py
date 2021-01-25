#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys,os
import argparse


print("Dereplicated Sequence Size Adder V2")
print("By: Patrick Gagne\n")



parser=argparse.ArgumentParser(description='Add the size= tag to each dereplicated sequences')

parser.add_argument("-fas", dest="fasta_file", help=("Fasta file containing dereplicated sequences [REQUIRED]"))
parser.add_argument("-name", dest="name_file", help=("Name file containing Sequence Dereplication [REQUIRED]"))
parser.add_argument("-sort", action='store_true', help=("Sort sequences by decreasing size order"))
parser.add_argument("-sep", dest="sep_char", default=';', help=("Seperator to be use in sequence name (ex: name;size=10) [DEFAULT= ;]"))
parser.add_argument("-out", dest="output", help=("Output filename [REQUIRED]"))

#args=parser.parse_args('-fas test.fasta -name test.names -out test.out.fasta'.split())
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
        fichier2=args.name_file
        name = open(fichier2, 'r')
    except IOError:
        print("Desole, ce fichier est introuvable")
        sys.exit(1)
    else:
        break

sep=str(args.sep_char)


print("Reading '%s'"%(fichier1))
fastaR=fasta.read()
print("Reading '%s'"%(fichier2))
nameL=name.readlines()

fasta.close()
name.close()

redun_dict={}

print("Mapping '%s'"%(fichier2))
for i in nameL:
    spl=i.split("\t")
    seqname=spl[0]
    seqs=spl[1][:-1].split(",")
    redun_dict[seqname]=seqs

fastaS=fastaR.split(">")
fastaR=None
fastaS.pop(0)

fastaS2=[]

print("Regrouping Sequences...")
for i in fastaS:
    spl=i.split("\n",1)
    seqname=spl[0]
    seq=spl[1].replace("\n","")
    fastaS2.append([seqname,seq,0])
fastaS=None


if ( not args.sort ):
    print("Adding size and writing to '%s'"%(str(args.output)))
    savefile=open(str(args.output), 'w')
    for liste in fastaS2:
        corr_line=""
        corr_line=liste[0]+sep+"size="+str(len(redun_dict[liste[0]]))+sep+"\n"
        savefile.write(">"+corr_line)
        savefile.write(liste[1]+"\n")
    savefile.close()
    
if ( args.sort ):
    print("Adding Size and sorting by decreasing order...")
    for liste in fastaS2:
        size=len(redun_dict[liste[0]])
        corr_line=liste[0]+sep+"size="+str(size)+sep+"\n"
        liste[0]=corr_line
        liste[2]=size
    fastaS2.sort(key=lambda x: x[2]*-1)
    savefile=open(str(args.output), 'w')
    print("Writing results to '%s'"%(str(args.output)))
    for liste in fastaS2:
        savefile.write(">"+liste[0])
        savefile.write(liste[1]+"\n")
    savefile.close()

print("\nProgram DONE\n")

##savefile=open(str(args.output), 'w')

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys,os
import argparse


print("Fasta Sequence Splitter V1.0")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Take a fasta file and split it into X files')

parser.add_argument("-fas", dest="fasta_file",required=True, help=("Fasta file to split [REQUIRED]"))
parser.add_argument("-part", dest="part_int",required=True,type=int, help=("Number of part to generate (Integer > 2) [REQUIRED]"))
parser.add_argument("-hide", action='store_true', help=("Generate hidden files instead of normal"))
parser.add_argument("-list", dest='list_name', help=("Generate a list containing all filenames"))
parser.add_argument("-suff", dest="output_suffix", help=("Output filename suffix (filename.partno.suffix) NO DOT AT START [default = fasta]"))

#args=parser.parse_args('-fas test.blastable.fasta -part 8 -suff fasta.temp'.split())
args=parser.parse_args() 

gen_list=1
suffix=str(args.output_suffix)
if str(args.output_suffix) == 'None':
    suffix="fasta"
if str(args.list_name) == 'None':
    gen_list=0
if int(args.part_int) <= 1:
    print("ERROR, invalid number of parts (%d) (must be an integer > 1)"%(args.part_nb))
    sys.exit(1)
while True:
    try:
        fichier1=args.fasta_file
        fasta = open(fichier1, 'r')
    except IOError:
        print("ERROR %s not found"%(fichier1))
        sys.exit(1)
    else:
        break

nb_part=int(args.part_int)

def CalculateRepartition(nbparts,nbseqs):
    rep_list=[]
    for i in range(0,nbparts):
        rep_list.append(0)
    ite=0
    start_range=0
    end_range=nbseqs-1
    direction=1
    while True:
        rep_list[ite]+=1
        start_range+=1
        if start_range > end_range:
            break
        if (direction > 0):
            ite+=1
        if (direction < 0):
            ite-=1
        if ((direction > 0) and (ite == nbparts)):
            direction=-1
            ite-=1
            continue
        if ((direction < 0) and (ite < 0)):
            direction=1
            ite+=1
            continue

    return rep_list

print("\nReading %s"%(fichier1))
fastaR=fasta.read()
fasta.close()
fastaS=fastaR.split(">")
fastaR=None
fastaS.pop(0)


print("Calculating split for %d threads"%(nb_part))
replist=CalculateRepartition(nb_part,len(fastaS))

generic_filename=fichier1[::-1].split(".",1)[1][::-1]

#if args.hide
file_num=-1
realcount=0
filename_list=[]
print("Splitting in progress...")
for i in replist:
    file_num+=1
    savefile_name=generic_filename+"."+str(file_num)+"."+suffix
    if args.hide:
        savefile_name="."+savefile_name
    seqcount=0
    savefile=open(savefile_name,'w')
    if gen_list==1:
        filename_list.append(savefile_name)
    while seqcount != i:
        savefile.write(">"+fastaS[realcount])
        seqcount+=1
        realcount+=1
    savefile.close()
    print("%s\tCreated \t(%d)"%(savefile_name,seqcount))
        
if gen_list==1:
    listfile=open(str(args.list_name),'w')
    for i in filename_list:
        listfile.write(i+"\n")
    listfile.close()

print("Splitting DONE")

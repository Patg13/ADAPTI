#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse




print("Fasta Size Table Generator V2")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Fasta Size Table Generator V2')

parser.add_argument("-in", dest="fasta_file", help=("OTUs file (fasta with ;size= tags) [REQUIS]"))
parser.add_argument("-d", action='store_true', help=("Histogram in decreasing order"))
parser.add_argument("-head", action='store_true', help=("Print Table Header"))

#args=parser.parse_args('-in O001_S49_L001_R1_001.fastq'.split())
args=parser.parse_args()



while True:
    try:
        fichier1=args.fasta_file
        fasta = open(fichier1, 'r')
    except IOError:
        print("ERROR, %s doesn't exist or cannot be found"%(fichier1))
        sys.exit(1)
    except TypeError:
        print("ERROR, No input file specified")
        sys.exit(1)
    else:
        break

#input()
#fastqL=fastq.readlines()

point=0

#seqlist=[]

size_list=[]
size_dict={}

nb_seqs=0

nb_reads=0
while True:
    seqname=fasta.readline()
    if seqname == "":
        break
    nb_seqs+=1
    try:
        size=seqname.split(";size=")[1].split(";")[0]
    except IndexError:
        print("ERROR, Size tags not found")
        sys.exit(1)
    fasta.readline()
    nb_reads+=int(size)
    if int(size) not in size_list:
        size_list.append(int(size))
        size_dict[int(size)]=1
    else:
        size_dict[int(size)]+=1

fasta.close()

if args.d:
    size_list.sort(reverse=True)
else:
    size_list.sort()

if args.head:
    #print("File : %s"%(str(args.fastq_file)))
    print("Nb Reads\tNb OTUs\tOtus %\tCumulative Otus %\tNb Cumulative Otus\tReads %\tCumulative Reads %\tNb Cumulative Reads")
cumul_reads=0
cumul_otu=0
cumul_reads_nb=0
cumul_otu_pour=0
for i in size_list:
    curr_size=size_dict[i]
    cumul_otu+=curr_size
    cumul_reads_nb+=curr_size*i
    cumul_reads+=((curr_size*i)/nb_reads)*100
    cumul_otu_pour+=(curr_size/nb_seqs)*100
    print(str(i)+"\t"+str(curr_size)+"\t"+str(round(curr_size/nb_seqs*100,4))+"\t"+str(round(cumul_otu_pour,4))+"\t"+str(cumul_otu)+"\t"+str(round(((curr_size*i)/nb_reads)*100,4))+"\t"+str(round(cumul_reads,4))+"\t"+str(cumul_reads_nb))

print("Program DONE")

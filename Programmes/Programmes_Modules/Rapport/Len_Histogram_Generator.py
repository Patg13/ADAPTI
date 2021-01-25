#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse




#print("Low Quality Sequence ends Trimmer")
#print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Fastq Length Table Generator V3')

parser.add_argument("-in", dest="fastx_file", help=("Fastx file (fasta or fastq) [REQUIS]"))
parser.add_argument("-d", action='store_true', help=("Histogram in decreasing order"))
parser.add_argument("-head", action='store_true', help=("Print Table Header"))
parser.add_argument("-total", action='store_true', help=("Print Totals at the end of the histogram"))

#args=parser.parse_args('-in O001_S49_L001_R1_001.fastq'.split())
args=parser.parse_args()



while True:
    try:
        fichier1=args.fastx_file
        fastx = open(fichier1, 'r')
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

len_list=[]
len_dict={}
mode=''
verif=fastx.readline()




if verif[0] == ">":
    mode='fasta'
    fastx.readline()
    if fastx.readline()[0] != '>':
        print('ERROR, this fasta file contain multiple lines sequences')
        sys.exit(1)
elif verif[0] == "@":
    mode="fastq"
else:
    print("ERROR, file format unknown (must be a fasta or fastq)")
    sys.exit(1)

fastx.close()
fastx = open(fichier1, 'r')

nb_seqs=0
if mode == 'fastq':
    while True:
        seqname=fastx.readline()
        #print(seqname)
        if seqname == "":
            break
        nb_seqs+=1
        seq=fastx.readline().replace("\n","")
        fastx.readline()
        fastx.readline()
        if len(seq) not in len_list:
            len_list.append(len(seq))
            len_dict[len(seq)]=1
        else:
            len_dict[len(seq)]+=1

if mode == 'fasta':
    while True:
        seqname=fastx.readline()
        if seqname == "":
            break
        nb_seqs+=1
        seq=fastx.readline().replace("\n","")
        if len(seq) not in len_list:
            len_list.append(len(seq))
            len_dict[len(seq)]=1
        else:
            len_dict[len(seq)]+=1

fastx.close()

if args.d:
    len_list.sort(reverse=True)
else:
    len_list.sort()

if args.head:
    #print("File : %s"%(str(args.fastq_file)))
    print("Length\tReads\tPercent")
for i in len_list:
    print(str(i)+"\t"+str(len_dict[i])+"\t"+str(round(len_dict[i]/nb_seqs*100,3)))

if args.total:
    print("--------------------------")
    print("Total\t"+str(nb_seqs)+"\t"+"100")

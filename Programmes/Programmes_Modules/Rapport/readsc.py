#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse




#print("Low Quality Sequence ends Trimmer")
#print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Fasta reads counter program V1', epilog="WARNING: If -size and -name options are used at the same time, their read count WILL additionate !")

parser.add_argument("-in", dest="fasta_file", help=("Fasta file [REQUIRED]"))
parser.add_argument("-size", action='store_true', help=("Fasta file contain ;size= tags"))
parser.add_argument("-name", dest='name_file', default="", help=("Use names file to obtain reads count"))


#args=parser.parse_args('-in Analyse_Myriam_ITS_15aout2016.desized.NO_HOMOP.ITS3A_FAILED.fasta -name Analyse_Myriam_ITS_15aout2016.trimmed.names'.split())
args=parser.parse_args()


use_name=False
use_size=False

if args.size == True:
    use_size=True

if args.name_file != "":
    use_name=True

if use_name==False and use_size == False:
    print("ERROR: Reads count method must be specified\n( use option -size or -name or both )")
    sys.exit(1)

try:
    fasta=open(args.fasta_file,'r')
except IOError:
    print("ERROR: %s not found or doesn't exist"%(args.fasta_file))
    

if use_name == True:
    try:
        name=open(args.name_file,'r')
    except IOError:
        print("ERROR: %s not found or doesn't exist"%(args.name_file))


fastaR=fasta.read()
fasta.close()
fastaS=fastaR.split(">")
fastaR=""
fastaS.pop(0)

if use_name == True:
    name_dict={}
    nameL=name.readlines()
    name.close()
    for i in nameL:
        seqname=i.split("\t",1)[0]
        seqs=i.split("\t",1)[1]
        if ";size=" in seqname:
            seqname=seqname.split(";size=")[0]
        cnt=seqs.count(",")+1
        name_dict[seqname]=cnt
    nameL=""

total_count=0
for i in fastaS:
    seqname=i.split("\n",1)[0]
    size=0
    if use_size == True:
        try:
            size+=int(seqname.split(";size=")[1].replace(";",""))
        except IndexError:
            print("ERROR: ';size=' tag not found")
            sys.exit(1)
        except ValueError:
            print("ERROR: invalid size tag (cannot convert to integer)")
            sys.exit(1)
    if use_name == True:
        if ";size=" in seqname:
            search_seqname=seqname.split(";size=")[0]
        else:
            search_seqname=seqname
        try:
            size+=name_dict[search_seqname]
        except KeyError:
            print("ERROR: %s not in names file"%(search_seqname))
            sys.exit(1)
    total_count+=size

print(total_count)


#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("\nVSEARCH Blast Converter")
print("By: Patrick Gagne")


parser=argparse.ArgumentParser(description='This Program convert the vsearch blast output to BLASTN output compatible with ADAPTI Pipeline')

parser.add_argument("-blast", dest="blast_file", required=True, help=("Blast file using format outfmt 6 [REQUIS]"))
parser.add_argument("-header", action='store_true', help=("First line of blast file is a header line"))
parser.add_argument("-out", dest="output", required=True, help=("Blast Output filename [REQUIS]"))


#args=parser.parse_args('-blast result.blast -out test.txt'.split())
args=parser.parse_args()


def Concat_List(liste,sep="\t"):
    new_str=""
    ind=0
    for i in liste:
        if ind == len(liste)-1:
            new_str+=str(i)
        else:
            new_str+=str(i)+sep
        ind+=1
    return new_str


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
blast.close()

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
blast_regroup.sort(key=lambda x: int(x[0].split("OTU_")[1].split(";")[0]),reverse=False)
for i in blast_regroup:
    for j in i:
        infos=j.split("\t")
        corr=infos[1].split(" ",1)
        acc=corr[0]
        new_iden=corr[1]
        infos.pop(1)
        infos.pop(1)
        infos.insert(1,new_iden)
        infos.insert(2,acc)
        corr_line=Concat_List(infos)
        savefile.write(corr_line)

savefile.close()

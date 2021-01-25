#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse
import math

def ConcatLists(list1,list2):
    complete_list=[]
    for i in list1:
        complete_list.append(i)
    for i in list2:
        complete_list.append(i)
    return complete_list

print("usearch otu table ---> frequencies table maker V2 for vsearch clustering")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='usearch otu table information extractor V2 for vsearch clustering')

parser.add_argument("-up", dest="vsearch_uc_file", required=True, help=("uparse table produced by option -uparseout [REQUIRED]"))
parser.add_argument("-group", dest="group_file", required=True, help=("Group file (sequence \t sample) [REQUIRED]"))
parser.add_argument("-name", dest="name_file", required=False, help=("Name file containing sequences redundancy [OPTIONAL]"))
parser.add_argument("-out", dest="table_output", required=True, help=("frequencies table Output filename [REQUIRED]"))

#args=parser.parse_args('-up otu.up -group Analysis14182_ITSSoil_30May2017_15h46.group -name Analysis14182_ITSSoil_30May2017_15h46.trimmed.names -out result.table'.split())
args=parser.parse_args() 

file=open(str(args.vsearch_uc_file),'r')

fileL=file.readlines()
file.close()

group=open(str(args.group_file),'r')

groupL=group.readlines()

group.close()

oldname=open(str(args.name_file),'r')
oldnameL=oldname.readlines()
oldname.close()



otu_corresp={}

otu_count=0
otu_list=[]
otu_conv_dict={}

seq_list=[]

for i in fileL:
    if i.split("\t")[0]=="S":
        seq_list=[]
        otu_count+=1
        otu_name="OTU_"+str(otu_count)
        otu_list.append(otu_name)
        seq_list.append(i.split("\t")[8].split(";size=")[0])
        otu_corresp[otu_name]=seq_list
        otu_conv_dict[i.split("\t")[8].split(";size=")[0]]=otu_name

vis=0

searchL=[]

for i in fileL:
    if i.split("\t")[0]=="S":
        continue
    elif i.split("\t")[0]=="C":
        break
    else:
        searchL.append(i)
   
searchL2=[]
for i in searchL:
    seqname=i.split("\t")[8].split(";size=")[0]
    otu=otu_conv_dict[i.split("\t")[9].split(";size=")[0]]
    otu_corresp[otu].append(seqname)
    
        
group_dict={}

print("making group dict")
ech_list=[]
for i in groupL:
    spl=i.strip("\n").split("\t")
    group_dict[spl[0]]=spl[1]
    if spl[1] not in ech_list:
        ech_list.append(spl[1])

redun_otu_seqs=[]
temp_redun_os=[]

print("making name dict")
redun_dict={}
for i in oldnameL:
    seqname=i.split("\t")[0]
    seqs=i.split("\t")[1].strip("\n").split(",")
    redun_dict[seqname]=seqs

print("adding redundancy")
for i in otu_list:
    for j in otu_corresp[i]:
        try:
            red=redun_dict[j]
        except KeyError:
            red=j
            temp_redun_os.append(red)
        else:
            for k in red:
                temp_redun_os.append(k)
    redun_otu_seqs.append(temp_redun_os)
    temp_redun_os=[]

temp_freq=[]
freqs=[]

print("counting reads / sample")
for i in redun_otu_seqs:
    count=0
    for j in ech_list:
        for k in i:
            if group_dict[k] == j:
                count+=1
        temp_freq.append(count)
        count=0
    freqs.append(temp_freq)
    temp_freq=[]

savefile=open(str(args.table_output),'w')

compt=0

x=0
result=""
for i in ech_list:
    if x == 0:
        result=result+"\t"
        x=1
    result=result+i+"\t"

result=result[:-1]

savefile.write(result+"\n")

print("writing results")
result=""
while compt != len(freqs):
    result=otu_list[compt]+"\t"
    for fr in freqs[compt]:
        result=result+str(fr)+"\t"
    result=result[:-1]
    result+="\n"
    savefile.write(result)
    compt+=1

savefile.close()
    
    
    


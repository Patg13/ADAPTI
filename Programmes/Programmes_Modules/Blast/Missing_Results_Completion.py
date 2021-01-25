#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

def JoinList(liste):
    strin=""
    for i in liste:
        strin+=str(i)+"\t"
    strin=strin[:-1]
    return strin

print("Besthits Table Missing OTUs Completion Program V2.0")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='This program add missing OTUs from best hits blast file')

parser.add_argument("-table",required=True, dest="table_file", help=("Best hits Blast file with frequencies [REQUIRED]"))
parser.add_argument("-freq",required=True, dest="freq_table", help=("Samples frequencies by otu table [REQUIRED]"))
parser.add_argument("-cutoff",default=-9999,type=int, dest="cutoff_value", help=("OTUs cut-off value (use this in case not all OTUs have been used for blast) [OPTIONAL]"))
parser.add_argument("-column",default=6,type=int, dest="column_value", help=("Number of column before samples frequencies [Default: 6]"))
parser.add_argument("-out",required=True, dest="output", help=("Output filename [REQUIRED]"))


#args=parser.parse_args('-table virginieITS.EXP_METHOD_ITS.besthits.blast -freq virginieITS.new.freq -out test.blast'.split())
args=parser.parse_args()


if str(args.output) == 'None':
    print('ERROR, No output filename specified')
    sys.exit(1)

while True:
    try:
        fichier1=args.table_file
        table = open(fichier1, 'r')
    except IOError:
        print("Error, %s cannot be found"%(fichier1))
        sys.exit(1)
    else:
        break

while True:
    try:
        fichier2=args.freq_table
        freq = open(fichier2, 'r')
    except IOError:
        print("Error, %s cannot be found"%(fichier2))
        sys.exit(1)
    else:
        break

if args.cutoff_value != -9999:
    if args.cutoff_value <= 0:
        print("ERROR, cutoff value must be an integer > 0")
        sys.exit(1)

print("Reading Blast and Frequency tables")

tableL=table.readlines()
freqL=freq.readlines()

table.close()
freq.close()

info_line=""

table_infoline=tableL.pop(0)
freq_infoline=freqL.pop(0)

tab_to_add=(args.column_value-2)+1

miss_toadd="\t-----INCONNU-----"

for i in range(0,tab_to_add):
    miss_toadd+="\t"


print("Generating OTUs dictionnary from Frequencies Table")

freq_dict={}
full_otu_list=[]

size_dict={}

size_c=0
count=0
for i in freqL:
    if count == args.cutoff_value:
        break
    size_c=0
    otu_name=i.split("\t")[0]
    freq_dict[otu_name]=i
    full_otu_list.append(otu_name)
    size_i=i.split("\t")[1:]
    for j in size_i:
        size_c+=int(j)
    size_dict[otu_name]=";size="+str(size_c)
    count+=1

print("Generating OTUs dictionnary from Results Table")

res_dict={}

for i in tableL:
    otu_name=i.split("\t")[0].split(";size=")[0]
    res_dict[otu_name]=i

print("Identifying missing OTUs and adding information")

write_buffer=[]
for i in full_otu_list:
    try:
        write_buffer.append(res_dict[i])
    except KeyError:
        newline=i+size_dict[i]+miss_toadd+JoinList(freq_dict[i].split("\t")[1:])
        write_buffer.append(newline)


savefile=open(str(args.output),'w')
    
print("Writing buffer in file")

write_buffer.insert(0,table_infoline)
for i in write_buffer:
    savefile.write(i)

savefile.close()



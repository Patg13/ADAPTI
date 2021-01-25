#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse




print("Uparse Table OTU-Seqs Extractor V1 for usearch based clustering")
print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Extract Every Sequences from a specific OTU (or all OTUs) clustered by usearch')

parser.add_argument("-up", dest="uparse_table",required=True, help=("Uparse Table with clustering orders"))
parser.add_argument("-otu", dest="otu_number", required=False, default="all", help=("OTU number to extract (if otus has been renamed, use the NEW name) [default=all]"))
parser.add_argument("-clist", dest="corrlist_file", required=False, help=("List of correspondance between old and new OTU name with this format NEW_NAME TAB OLD_NAME (if not specified, no correspondance will be made)"))
parser.add_argument("-fasta", dest="fasta_file", required=True, help=("Fasta File used for clustering ( fasta prior clustering used as usearch input)"))

#args=parser.parse_args('-up otu.up -otu 5,6,7,8,9 -clist BERJ011_Patho_18nov2016.corrlist.txt -fasta BERJ011_Patho_18nov2016.sized_sort.NO_HOMOP.fasta'.split())
args=parser.parse_args()

if args.otu_number == "all":
    otu_nb=[-1]
else:
    otu_nb=[]
    try:
        if len(args.otu_number.split(",")) > 1:
            for i in args.otu_number.split(","):
                if int(i) < 0:
                    print("ERROR: OTU number must be an integer > -1")
                    sys.exit()
                otu_nb.append(int(i))
        else:
            if int(args.otu_number) < 0:
                print("ERROR: OTU number must be an integer > -1")
                sys.exit(1)
            otu_nb.append(int(args.otu_number))
    except ValueError:
        print("ERROR: Otu number must be an integer ( to extract all otus, don't use -otus)")
        sys.exit(1)


correction=True

if args.corrlist_file == None:
    correction=False

try:
    uptab=open(args.uparse_table,'r')
except IOError:
    print("ERROR: %s not found or doesn't exist"%(args.uparse_table))
    sys.exit(1)

try:
    fasta=open(args.fasta_file,'r')
except IOError:
    print("ERROR: %s not found or doesn't exist"%(args.fasta_file))
    sys.exit(1)


if correction == True:
    try:
        corlist=open(args.corrlist_file,'r')
    except IOError:
        print("ERROR: %s not found or doesn't exist"%(args.corrlist_file))
        sys.exit(1)

##################################################
corr_dict_no={}
corr_dict_on={}

if correction == True:
    print("Reading Correction List")
    corlistL=corlist.readlines()
    corlist.close()
    for i in corlistL:
        inf=i.replace("\n","").split("\t")
        corr_dict_on[inf[1]]=inf[0]
        corr_dict_no[inf[0]]=inf[1]

corlistL=""

##################################################
print("Reading Fasta File")
fastaR=fasta.read()
fasta.close()
fastaS=fastaR.split(">")
fasta_dict={}
fastaS.pop(0)
for i in fastaS:
    seqname=i.split("\n",1)[0]
    seq=i.split("\n",1)[1].replace("\n","")
    fasta_dict[seqname]=seq
fastaR=""
fastaS=""
##################################################
print("Reading Uparse Table")
uptabL=uptab.readlines()
uptab.close()

otu_lines=[]
for i in uptabL:
    #if i.split("\t")[1].upper() == "MATCH":
    otu_lines.append(i.replace("\n",""))

uptabL=[]
##################################################

#sys.exit()
ext_otus_count=0
if otu_nb[0] != -1:
    for i in otu_nb:
        print("Extracting OTU_%d Sequences"%(i))
        name_search="OTU_"+str(i)
        if correction == True:
            name_search_c=corr_dict_no[name_search]
        else:
            name_search_c=name_search

        savefile_name="OTU_"+str(i)+".fasta"
        savefile=open(savefile_name,'w')
        for i in otu_lines:
            spl=i.split("\t")
            seqname=spl[0]
            typ=spl[1]
            if typ.upper() == "OTU":
                if spl[-1] == name_search_c:
                    savefile.write(">"+seqname+"\n"+fasta_dict[seqname]+"\n")
                else:
                    continue
            if typ.upper() == "MATCH":
                if spl[-1] == name_search_c:
                    savefile.write(">"+seqname+"\n"+fasta_dict[seqname]+"\n")
                else:
                    continue
        savefile.close()
        ext_otus_count+=1

    

if otu_nb[0] == -1:
    print("Extracting All OTUs sequences")
    otu_list=[]
    search_dict={}
    for i in otu_lines:
        spl=i.split("\t")
        if spl[1].upper() == "OTU":
            otu_list.append(spl[-1])
            search_dict[spl[-1]]=[spl[0]]
        elif spl[1].upper() == "MATCH":
            search_dict[spl[-1]].append(spl[0])
    for i in otu_list:
        if correction == True:
            savefile_name=corr_dict_on[i]+".fasta"
        else:
            savefile_name=i+".fasta"
        savefile=open(savefile_name,'w')
        for j in search_dict[i]:
            savefile.write(">"+j+"\n"+fasta_dict[j]+"\n")
        ext_otus_count+=1
        savefile.close()


print("Number of extracted OTUs : %d"%(ext_otus_count))
print("Program DONE")
             
    
        

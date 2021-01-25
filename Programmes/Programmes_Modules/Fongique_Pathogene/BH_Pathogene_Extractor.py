#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse


#print("Low Quality Sequence ends Trimmer")
#print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Besthit Pathogene Extractor V1')

parser.add_argument("-in", dest="besthit_file", help=("Best Hits Blast file with identification at column x (x defined by option -idcol) [REQUIRED]"))
parser.add_argument("-list", dest="Pathogene_file", help=("Pathogene List (one entry per line, NO HEADER) [REQUIRED]"))
parser.add_argument("-silent", action='store_true', help=("Prevent WARNING messages"))
parser.add_argument("-bhead", action='store_true', help=("First Blast Line is a Header line"))
parser.add_argument("-newcol", dest="AddColumn_Val",type=int,default=4, help=("Column Number in Besthit file where Sequence Identification is [default=4 (before AccNB)]"))
parser.add_argument("-idcol", dest="IdenColumn_Val",type=int,default=3, help=("Column number to assign to Pathogenic information (will be add after the column preceding this number ( [default=4]"))
parser.add_argument("-out", dest="output_filename", help=("Output Filename [REQUIRED]"))

#args=parser.parse_args('-in Rods_filtre_2014_2015.blast -bhead -list Pathogene_List_3juin2016.txt -out result.blast'.split())
args=parser.parse_args()


while True:
    try:
        fichier1=args.besthit_file
        blast = open(fichier1, 'r')
    except IOError:
        print("ERROR, %s doesn't exist or cannot be found"%(fichier1))
        sys.exit(1)
    except TypeError:
        print("ERROR, No input file specified")
        sys.exit(1)
    else:
        break
    
while True:
    try:
        fichier2=args.Pathogene_file
        plist = open(fichier2, 'r')
    except IOError:
        print("ERROR, %s doesn't exist or cannot be found"%(fichier1))
        sys.exit(1)
    except TypeError:
        print("ERROR, No input file specified")
        sys.exit(1)
    else:
        break


print("Reading and indexing Trophic State List")


plistL=plist.readlines()
plist.close()

if plistL[0] == "\n":
    plistL.pop(0)

header_check1=plistL[0]
header_check2=plistL[1]
header_check3=plistL[2]

#if "GENRE/" in header_check[0].upper()  "TROPHIC" in header_check[1].upper() or "LIFESTYLE" in header_check[3].upper():

if header_check3.replace("\t","")=="\n" and "GENRE" in header_check[0].upper():
    plistL.pop(0)
    plistL.pop(0)
    plistL.pop(0)


ts_dict={}
for i in plistL:
    spl=i.replace("\n","").split(" ")
    Genre=""
    Espece=""
    if len(spl) < 2:
        Genre=spl[0]
    else:
        Genre=spl[0]
        Espece=spl[1]
    if Genre == "":
        print("ERROR: Empty Gender Detected, Invalid Entry in Pathogene List line %d"%(tlistL.index(i)))
        sys.exit(1)
    try:
        ts_dict[Genre.upper()]
    except KeyError:
        ts_dict[Genre.upper()]=1
    if Espece != "":
        try:
            ts_dict[Espece.upper()]
        except KeyError:
            ts_dict[Espece.upper()]=2

x=0
long=0
count=0
print("Screening Besthit file for Pathogene...")
savefile=open(args.output_filename,'w')
while True:
    code=0
    count+=1
    blast_line=blast.readline()
    if args.bhead and x == 0:
        x=1
        header=blast_line.split("\t")
        header.insert(args.AddColumn_Val-1,"Genre_Patho")
        header.insert(args.AddColumn_Val,"Espece_Patho")
        long=len(header)
        header="\t".join(header)
        savefile.write(header)
        continue
    if x == 0:
        x=1
        long=len(blast_line.split("\t"))+2
    if blast_line== "":
        break
    linespl=blast_line.split("\t")
    latin_name=linespl[args.IdenColumn_Val-1]
    latin_spl=latin_name.split(" ")
    Genre=latin_spl[0]
    Espece=""
    if ";" in Genre:
        Genre=Genre.split(";")[0]
    try:
        Espece=latin_spl[1]
    except IndexError:
        Espece=""
    if ";" in Espece:
        Espece=Espece.split(";")[0]
    try:
        ts_dict[Genre.upper()]
    except KeyError:
        continue
    else:
        code=1
    if Espece != "":
        try:
            code=ts_dict[Espece.upper()]
        except KeyError:
            pass
        else:
            code=2
    #print(Genre)
    #print(Espece)
    if code == 1:
        linespl.insert(args.AddColumn_Val-1,Genre)
        linespl.insert(args.AddColumn_Val,"")
        if len(linespl) != long:
            print("ERROR: Inconsistent Line Length (check line: %d)"%(count))
            sys.exit(1)
        savefile.write("\t".join(linespl))
    else:
        linespl.insert(args.AddColumn_Val-1,Genre)
        linespl.insert(args.AddColumn_Val,Espece)
        if len(linespl) != long:
            print("ERROR: Inconsistent Line Length (check line: %d)"%(count))
            sys.exit(1)
        savefile.write("\t".join(linespl))
    
savefile.close()

print("Program DONE")
        
        

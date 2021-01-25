#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

#Incertae sedis
def FindBestInfo(tupl1,tupl2):
    ind=0
    long=len(tupl1)
    score1=long
    score2=long
    while ind != long:
        if tupl1[ind] == "":
            score1-=1
        if tupl1[ind] == "Unknown Function":
            score1-=1
        if tupl1[ind] == "Incertae sedis":
            score1-=1
        if tupl2[ind] == "":
            score2-=1
        if tupl2[ind] == "Unknown Function":
            score2-=1
        if tupl2[ind] == "Incertae sedis":
            score2-=1
        ind+=1
    if score1 >= score2:
        return tupl1
    else:
        return tupl2

#print("Low Quality Sequence ends Trimmer")
#print("By: Patrick Gagne")



parser=argparse.ArgumentParser(description='Besthit Blast Trophic State Column Adder V1')

parser.add_argument("-in", dest="besthit_file", help=("Best Hits Blast file with identification at column 3 [REQUIRED]"))
parser.add_argument("-list", dest="Trophic_file", help=("Tab Separated Text file containing Organism Class and Order + Trophic State + Lifestyle ( 5 columns ) [REQUIRED]"))
parser.add_argument("-silent", action='store_true', help=("Prevent WARNING messages"))
parser.add_argument("-bhead", action='store_true', help=("First Blast Line is a Header line"))
parser.add_argument("-col", dest="Column_Value",type=int,default=4, help=("Column number for Trophical Information [default=4 (before AccNB)]"))
parser.add_argument("-out", dest="output_filename", help=("Output Filename [REQUIRED]"))

#args=parser.parse_args('-in Donnee_Anticosti_MO.new.besthits_with_missing.blast -bhead -list Fungi_Trophic_State_Type_3juin2016.txt -out result.blast'.split())
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
        fichier2=args.Trophic_file
        tlist = open(fichier2, 'r')
    except IOError:
        print("ERROR, %s doesn't exist or cannot be found"%(fichier2))
        sys.exit(1)
    except TypeError:
        print("ERROR, No input file specified")
        sys.exit(1)
    else:
        break


print("Reading and indexing Trophic State List")


tlistL=tlist.readlines()
tlist.close()

if tlistL[0] == "\n":
    tlistL.pop(0)

header_check=tlistL[0].split("\t")

if "TAXON" in header_check[0].upper() or "TROPHIC" in header_check[1].upper() or "LIFESTYLE" in header_check[3].upper():
    tlistL.pop(0)

#sys.exit()
err_count=0
ts_dict={}
for i in tlistL:
    spl=i.split("\t")
    taxon=spl[0]
    type1=spl[1]
    type2=spl[2]
    troph=spl[3]
    error=""
    conserve=""
    try:
        error=ts_dict[taxon.upper()]
    except KeyError:
        ts_dict[taxon.upper()]=(taxon,type1,type2,troph)
    else:
        err_count+=1
        best=FindBestInfo(error,(taxon,type1,type2,troph))
        #print(error[3])
        #print("unknown functions" not in error[3].lower())
        if best != error:
            ts_dict.pop(taxon.upper())
            ts_dict[taxon.upper()]=best
            conserve="Redundant"
        else:
            conserve="First Find"
        if args.silent == False:
            print("\nWARNING: Taxon Redundancy Detected in Trophic State List")
            print("Redundant Taxon Name: '%s'  |  First Find Taxon Name: '%s'"%(taxon,error[0]))
            print("Redundant Class: '%s'  |  First Find Class: '%s'"%(type1,error[1]))
            print("Redundant Order: '%s'  |  First Order: '%s'"%(type2,error[2]))
            print("Redundant Trophic State: '%s'  |  First Find Trophic State: '%s'"%(troph,error[3]))
            print("Entry Kept: %s"%(conserve))
            #print(best)
            continue
        
print("\nRedundancy Error Corrected: %d\n"%(err_count))

x=0
print("Creating New Blast File...")
savefile=open(args.output_filename,'w')
while True:
    blast_line=blast.readline()
    if args.bhead and x == 0:
        x=1
        header=blast_line.split("\t")
        header.insert(args.Column_Value-1,"Organism_Class")
        header.insert(args.Column_Value,"Organism_Order")
        header.insert(args.Column_Value+1,"Trophic_Statut")
        header="\t".join(header)
        savefile.write(header)
        continue
    if blast_line== "":
        break
    linespl=blast_line.split("\t")
    latin_name=linespl[2]
    latin_spl=latin_name.split(" ")
    Genre=latin_spl[0]
    Espece=""
    try:
        Espece=latin_spl[1]
    except IndexError:
        Espece=""
    try:
        troph_tupl=ts_dict[Genre.upper()]
    except KeyError:
        troph_tupl=("","","","","")
    linespl.insert(args.Column_Value-1,troph_tupl[1])
    linespl.insert(args.Column_Value,troph_tupl[2])
    linespl.insert(args.Column_Value+1,troph_tupl[3])
    savefile.write("\t".join(linespl))
    
savefile.close()

print("Program DONE")
        
        

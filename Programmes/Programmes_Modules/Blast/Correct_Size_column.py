#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("\n;Size=; Tags Column Correction Program V1")
print("By: Patrick Gagne\n")


parser=argparse.ArgumentParser(description='This program replace a ;size=XXXX tags in query name by query_name XXXX')

parser.add_argument("-in", dest="blast_file", required=True, help=("Blast file using format outfmt 6 [REQUIS]"))
parser.add_argument("-header", action='store_true', help=("First line of blast file is a header line"))
parser.add_argument("-out", dest="output", required=True, help=("Blast Output filename [REQUIS]"))


#args=parser.parse_args('-in error.blast -header -out result.blast'.split())
args=parser.parse_args()


while True:
    try:
        fichier1=str(args.blast_file)
        blast = open(fichier1, 'r')
    except IOError:
        print("Error: %s cannot be found"%(fichier1))
        sys.exit(1)
    else:
        break

info_str=""
blastL=blast.readlines()
blast.close()
if args.header:
    info_line=blastL.pop(0)
    try:
        info_line.split(";size=")[1]
    except IndexError:
        pass
    else:
        print("ERROR: first line doesn't seem to be a header line, the ;size= tag is present.")
        sys.exit(1)
    info_line=info_line.split("\t")
    info_line.insert(1,"Size")
    info_str="\t".join(info_line)

try:
    blastL[0].split("\t")[0].split(";size=")[1].split(";")[0]
except IndexError:
    print("ERROR: ;size=XXXX; tags not detected, or first line is header, and -header option was not specified")
    sys.exit(1)

savefile=open(str(args.output),'w')
if args.header:
    savefile.write(info_str)

for i in blastL:
    spl=i.split("\t",1)
    query=spl[0]
    line=spl[1]
    size=query.split(";size=")[1].split(";")[0]
    query=spl[0].split(";size=")[0]
    savefile.write(query+"\t"+size+"\t"+line)

savefile.close()


#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

def join_list(liste,sep="\t"):
    new_string=""
    maxi=len(liste)
    count=0
    for i in liste:
        if count != maxi-1:
            new_string=new_string+i+sep
        else:
            new_string=new_string+i
        count+=1
    return new_string




print("\nBlast Identification reformatting program")
print("By: Patrick Gagne")


parser=argparse.ArgumentParser(description='This program take each blast line and reduce the sequence identification line to X chars')

parser.add_argument("-blast", dest="blast_file", required=True, help=("Blast file using format outfmt 6 [REQUIRED]"))
parser.add_argument("-column", dest="col_number", required=True,type=int, help=("Column number of the sequence identification line (1st column = 1) [REQUIRED]"))
parser.add_argument("-max_chars", dest="chars_max", default=40,type=int, help=("Max number of chars in identification line (default=40)"))
parser.add_argument("-out", dest="output", required=True, help=("Blast Output filename [REQUIRED]"))

#args=parser.parse_args('-blast test.new.otus.blast -column 2 -out result.blast'.split())
args=parser.parse_args()


input_file=str(args.blast_file)
output_file=str(args.output)
nb_col=int(args.col_number)-1
max_char=int(args.chars_max)

if input_file == output_file:
    print("ERROR: Input file cannot be the same than the output file")
    sys.exit(1)

if nb_col < 1 :
    print("ERROR: Column number must be an integer > 0")
    sys.exit(1)

if max_char < 1:
    print("ERROR: Maximum chars must be an integer > 0")
    sys.exit(1)

savefile=open(output_file,'w')
with open(input_file,'r') as blast:
    for line in blast:
        spl=line.split("\t")
        iden_line=spl[nb_col]
        iden_line=iden_line[0:max_char]
        spl[nb_col]=iden_line
        savefile.write(join_list(spl))

savefile.close()

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

#print("Par: Patrick Gagne")



file_ind=1

filelist=[]

while True:
    try:
        filelist.append(sys.argv[file_ind])
    except IndexError:
        break
    if (sys.argv[file_ind] == "-h" or sys.argv[file_ind] == "--help"):
        print("\nFasta Sequences Counting Program V2.0")
        print("By: Patrick Gagne")
        print("USAGE: fastac [-h] file1.fasta [file2.fasta] [...]\n")
        sys.exit(1)
    else:
        file_ind+=1

nbseqs=0
for i in filelist:
    nbseqs=0
    with open(i,'r') as fasta:
        for line in fasta:
            if line[0] == '>':
                nbseqs+=1
            else:
                continue
    print(i+"\t"+str(nbseqs))

#print(nbseqs)

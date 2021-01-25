#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

filename=str(sys.argv[1])
ech_name=str(sys.argv[2])

fasta=open(filename, 'r')


fastaL=fasta.readlines()

fasta.close()

corr=""
for line in fastaL:
    if line[0] == ">":
        corr=line.strip("\n").strip(">")
        print(corr+"\t"+ech_name)

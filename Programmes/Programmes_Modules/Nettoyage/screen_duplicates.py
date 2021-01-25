#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

filename=str(sys.argv[1])
#filename="test.group"


dup_dict={}
#dup_list=[]
with open(filename,"r") as groupfile:
    for line in groupfile:
        seqname=line.split("\t")[0]
#        seqlist.append(seqname)
        try:
            check=dup_dict[seqname]
        except KeyError:
            dup_dict[seqname]=1
        else:
            print("ERROR: duplicate found (%s)"%(line))
            sys.exit(1)

print("No duplicates found")

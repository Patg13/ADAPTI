#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse


class FastqSeq:
        def __init__(self, seqname, seq, qual):
                self.seqname = seqname.strip("\n")[1:]
                self.seq = seq.strip("\n")
                self.qual = qual.strip("\n")
        def __repr__(self):
                return repr((self.seqname, self.seq, self.qual, self.length))
        def fasta_write(self,file):
                file.write(">"+self.seqname+"\n")
                file.write(self.seq+"\n")

print("fastq -----> fasta converter")
print("By: Patrick Gagne")

file_ind=1

filelist=[]

while True:
    try:
        filelist.append(sys.argv[file_ind])
    except IndexError:
        break
    if (sys.argv[file_ind] == "-h" or sys.argv[file_ind] == "--help"):
        print("\nFastq files -----> Fasta Converting Program V2.0")
        print("By: Patrick Gagne")
        print("USAGE: fastq2fasta [-h] file1.fastq [file2.fastq] [...]\n")
        sys.exit()
    else:
        file_ind+=1

if len(filelist) < 1:
        print("ERROR, no file specified")
        sys.exit(1)

for i in filelist:
        if "*" in i:
                print("ERROR, wildcard char found in file names")
                print("Please check wildcard syntax")
                sys.exit(1)


for file in filelist:
        print("Reading %s"%(file))
        fastq=open(file,'r')
        fastqL=fastq.readlines()
        fastq.close()
        i=0
        if (len(file.split(".fastq")) != 2):
                print ("ERROR, Invalid file type %s"%(file))
                sys.exit(1)
        savefile_name=file.split(".fastq")[0]+".fasta"
        savefile=open(savefile_name,'w')

        print("Converting %s to fasta..."%(file))

        while (i < len(fastqL)):
                seqname=fastqL[i]
                seq=fastqL[i+1]
                qualseq=fastqL[i+3]
                fastqS=FastqSeq(seqname,seq,qualseq)
                fastqS.fasta_write(savefile)
                i=i+4

        print("Conversion of %s done"%(file))
        print("\n")
        savefile.close()

print("\nProgram DONE")

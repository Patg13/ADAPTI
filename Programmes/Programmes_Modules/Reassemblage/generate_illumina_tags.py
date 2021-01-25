#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os,random
import argparse

parser=argparse.ArgumentParser(description='Illumina Fastq Sequence Name Randomizer/Creator')

parser.add_argument('-l','--list', nargs='+', dest="ech_list", help='<Required> Set flag', required=True)
parser.add_argument('-f','--forward_pattern', dest="for_patt", required=True, help=("forward pattern used [REQUIRED]"))
parser.add_argument('-r','--reverse_pattern', dest="rev_patt", required=True, help=("reverse pattern used [REQUIRED]"))

parser.add_argument("-o",'--out_suffix', dest="output_suffix", default="M", help=("fasta output filename [REQUIRED]"))


#args=parser.parse_args('-f _1.fastq -r _2.fastq -l SRS2937504_1.fastq SRS2937505_1.fastq'.split())
args=parser.parse_args()


def random_tag():
    letter='abcdefghijklmnopqrstuvwxyz'.upper()
    number='1234567890'
    machine_iden=str(random.choice(letter))+str(random.choice(number))+str(random.choice(number))+str(random.choice(number))+str(random.choice(number))+str(random.choice(number))
    project_iden=str(random.randint(1,10000))
    flowcell_iden="000000000-"+str(random.choice(letter))+str(random.choice(letter))+str(random.choice(number))+str(random.choice(letter))
    return (machine_iden,project_iden,flowcell_iden)

random_param=random_tag()

forward_list=args.ech_list

flow_x=0
flow_y=1
zone=0

for i in forward_list:
    zone+=1
    flow_x=random.randint(0,10000)
    flow_y=random.randint(0,10000)
    ech_name=i.split(args.for_patt)[0]
    reverse_name=ech_name+args.rev_patt
    forfile_r=open(i)
    forfile_w=open(ech_name+args.output_suffix+args.for_patt,'w')
    revfile_r=open(reverse_name)
    revfile_w=open(ech_name+args.output_suffix+args.rev_patt,'w')
    base_flow_x=flow_x
    base_flow_y=flow_y
    while True:
        seqname=forfile_r.readline()
        if seqname == '':
            break
        seq=forfile_r.readline()
        sep_line=forfile_r.readline()
        quality_line=forfile_r.readline()
        newname="@"+random_param[0]+":"+random_param[1]+":"+random_param[2]+":1:"+str(zone)+":"+str(flow_x)+":"+str(flow_y)+" 1:N:0:1\n"
        forfile_w.write(newname)
        forfile_w.write(seq)
        forfile_w.write("+\n")
        forfile_w.write(quality_line)
        flow_x+=1
        flow_y+=1
    flow_x=base_flow_x
    flow_y=base_flow_y
    while True:
        seqname=revfile_r.readline()
        if seqname == '':
            flow_x+=10
            flow_y+=11
            break
        seq=revfile_r.readline()
        sep_line=revfile_r.readline()
        quality_line=revfile_r.readline()
        newname="@"+random_param[0]+":"+random_param[1]+":"+random_param[2]+":1:"+str(zone)+":"+str(flow_x)+":"+str(flow_y)+" 2:N:0:1\n"
        revfile_w.write(newname)
        revfile_w.write(seq)
        revfile_w.write("+\n")
        revfile_w.write(quality_line)
        flow_x+=1
        flow_y+=1
    forfile_r.close()
    forfile_w.close()
    revfile_r.close()
    revfile_w.close()

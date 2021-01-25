#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse


class FreqLine:
        def __init__(self, line, otu):
                self.line= line.strip("\n")
                self.otu=int(otu)
        def line_write(self,file):
                file.write(self.line+"\n")
    

class FastaSeq:
        def __init__(self, seqname, seq):
                self.seqname = seqname.strip("\n")
                self.seq = seq.replace("\n","")
                self.size = int(self.seqname.split(";size=")[1].split(";")[0])
                self.otu = self.seqname.split(";size=")[0]
        def __repr__(self):
                return repr((self.seqname, self.seq, self.size))
        def fasta_write(self,file):
                file.write(">"+self.seqname+"\n")
                file.write(self.seq+"\n")
        def reset(self):
                self.otu = self.seqname.split(";size=")[0]

class Blastbloc:
        def __init__(self, bloc, no_otu):
                self.bloc=bloc
                self.otu=no_otu
        def __repr__(self):
                return repr((self.bloc, self.otu))
        def bloc_write(self,file):
                for i in self.bloc:
                        file.write(i.replace("\n","")+"\n")

def Extract_Blast_Blocs(lines):
        temp_blast_bloc=[]
        blast_blocs=[]
        x=0
        leading=""
        for i in lines:
                seqname=i.split("\t")[0]
                if x==0:
                     leading=seqname
                     x=1
                     temp_blast_bloc.append(i)
                     continue
                if seqname != leading:
                        leading=seqname
                        blast_blocs.append(temp_blast_bloc)
                        temp_blast_bloc=[]
                        temp_blast_bloc.append(i)
                        continue
                else:
                        temp_blast_bloc.append(i)
        blast_blocs.append(temp_blast_bloc)
        temp_blast_bloc=[]

        return blast_blocs

def Rename_Bloc(bloc,new_name):
        newbloc=[]
        for i in bloc:
                res=""
                info=i.split("\t",1)[1]
                res=str(new_name)+"\t"+info
                newbloc.append(res)
        return newbloc


print("Otu position corrector V2")
print("By: Patrick Gagne\n")



parser=argparse.ArgumentParser(description='Otu Position and name correction program')

parser.add_argument("-fas", dest="fasta_file", required=True, help=("Fasta file with seqname= OTU_XXXX and size= information [REQUIS]"))
parser.add_argument("-freq", dest="freq_file", required=False, help=("Freq table to correct [OPTIONAL]"))
parser.add_argument("-blast", dest="blast_file", required=False, help=("Blast file to correct (quaryname must be first column [OPTIONAL]"))
parser.add_argument("-idenL", dest="idenLine_bool", default='F', help=("T or F, first line of blast is columns identifiers (default:F) [OPTIONAL]"))


parser.add_argument("-fasout", dest="fasta_output", required=True, help=("Fasta Output filename [REQUIS]"))
parser.add_argument("-corout", dest="corresp_output", required=False, help=("Otu correspondance List Output filename [OPTIONAL]"))
parser.add_argument("-freout", dest="freq_output", required=False, help=("uparse Output filename [REQUIRED IF -UP]"))
parser.add_argument("-blaout", dest="blast_output", required=False, help=("Blast Output filename [REQUIRED IF -blast]"))

#args=parser.parse_args('-fas myriam.otus.fasta -idenL F -freq myriam.freq -freout result.freq -fasout result.fasta'.split())
args=parser.parse_args()                              


if str(args.idenLine_bool).upper() == 'T':
        blast_leading=1
elif str(args.idenLine_bool).upper() == 'F':
        blast_leading=0
else:
        print("ERROR, invalid choice (%s) for -idenL [must be T or F]"% (args.idenLine_bool))
        sys.exit(1)

corlst=0
if args.corresp_output != None:
        corlst=1
        if str(args.corresp_output) == "":
                print("ERROR, Correspondance List filename not valid")
                sys.exit(1)

up=0
if args.freq_file != None:
        up=1
        if args.freq_output == None:
                print("ERROR uparse output filename must be specified if uparse input is specified")
                sys.exit(1)
        else:
                while True:
                        try:
                                fichier=args.freq_file
                                freq = open(fichier, 'r')
                        except IOError:
                                print("ERROR %s not found"%(fichier))
                                sys.exit(1)
                        else:
                                break
bl=0
if args.blast_file != None:
        bl=1
        if args.blast_output == None:
                print("ERROR uparse output filename must be specified if uparse input is specified")
                sys.exit(1)
        else:
                while True:
                        try:
                                fichier=args.blast_file
                                blast = open(fichier, 'r')
                        except IOError:
                                print("ERROR %s not found"%(fichier))
                                sys.exit(1)
                        else:
                                break

while True:
    try:
        fichier1=args.fasta_file
        fasta = open(fichier1, 'r')
    except IOError:
        print("ERROR  %s not found"%(fichier1))
        sys.exit(1)
    else:
        break

print("Reading fasta file")
fastaL=fasta.read()
fasta.close()

fastaL=fastaL.split(">")

fastaL.pop(0)


fastaSeqs=[]
for i in fastaL:
        seqname=i.split("\n")[0]
        if ";size=" in seqname:
                seqname2=seqname.split(";size=")[0]
        seq=i.split("\n",1)[1].strip("\n")
        fastaS=FastaSeq(seqname,seq)
        fastaSeqs.append(fastaS)
#fastaL=None
#test2, key=lambda SPatterns: SPatterns.calc
print("Sorting by size")
fastaSeqs.sort(key = lambda FastaSeq: FastaSeq.size, reverse=True)

otu_corr={}
newotu_size={}

otu_num=0
old_otu_list=[]
print("Renaming OTUs")
for i in fastaSeqs:
        otu_num+=1
        otu_corr[i.otu]="OTU_"+str(otu_num)
        old_otu_list.append(i.otu)
        newotu_size["OTU_"+str(otu_num)]=i.size
        i.seqname="OTU_"+str(otu_num)+";size="+str(i.size)+";"
        i.reset()

print("Writing new fasta file")
fas_savefile=open(str(args.fasta_output),'w')

for i in fastaSeqs:
        i.fasta_write(fas_savefile)

fas_savefile.close()
        

if up == 1:
        freqlines=[]
        print("Reading frequencies table")
        freqL=freq.readlines()
        freq.close()
        freq_savefile=open(str(args.freq_output),'w')
        leading=freqL.pop(0)
        freq_savefile.write(leading)
        for i in freqL:
                oldname=i.split("\t",1)[0]
                freqs=i.split("\t",1)[1]
                newname=otu_corr[oldname]
                line_nw=newname+"\t"+freqs
                otu=int(newname.split("_")[1])
                FrLi=FreqLine(line_nw,otu)
                freqlines.append(FrLi)
        freqlines.sort(key = lambda FreqLine: FreqLine.otu, reverse=False)
        for i in freqlines:
                i.line_write(freq_savefile)
        freq_savefile.close()
        print("frequencies table corrected")


if bl == 1:
        print("Reading Blast file")
        blastL=blast.readlines()
        blast.close()
        bl_savefile=open(str(args.blast_output),'w')
        print("Correcting Blast file")
        leading_line=""
        if blast_leading==1:
                leading_line=blastL.pop(0)
        bl_blocs=Extract_Blast_Blocs(blastL)
        blastL=None
        bl_obj=[]
        for i in bl_blocs:
                oldname=i[0].split("\t")[0].split(";size=")[0]
                newname=otu_corr[oldname]+";size="+str(newotu_size[otu_corr[oldname]])
                num_otu=int(newname.split("OTU_")[1].split(";size=")[0])
                cor_bloc=Rename_Bloc(i,newname)
                bloc_o=Blastbloc(cor_bloc,num_otu)
                bl_obj.append(bloc_o)
        bl_obj.sort(key = lambda Blastbloc: Blastbloc.otu, reverse=False)
        if leading_line != "":
                bl_savefile.write(leading_line)
        for i in bl_obj:
                i.bloc_write(bl_savefile)
        bl_savefile.close()
        print("Blast file corrected")
                            
                
if corlst == 1:
        print("Writing Correspondance List")
        corl_savefile=open(str(args.corresp_output),'w')
        corl_savefile.write("New_Otu_Name\tOld_Otu_Name\n")
        for i in old_otu_list:
                write_line=otu_corr[i]+"\t"+i+"\n"
                corl_savefile.write(write_line)
        corl_savefile.close()
        print("Correspondance List created")



print("Corrections DONE")
                        
                        
        

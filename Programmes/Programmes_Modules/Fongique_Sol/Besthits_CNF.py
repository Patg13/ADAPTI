#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys,os
import argparse

print("\nBesthit Blast Filter & Compressor V1")
print("By: Patrick Gagne\n")

print("--------------------------------------------")
print("\nNOTICE:")
print("This program regroup besthits lines using Accession Number ONLY")
print("It doesn't consider % Identity or BitScore\n")
print("The Alignment length is used for filtration purpose\nand HAVE PRIORITY over regrouping\n")
print("-------------------------------------------\n")

parser=argparse.ArgumentParser(description='Besthit Blast Filter & Compressor V1')

parser.add_argument("-in", dest="besthit_file",required=True, help=("Best Hits Blast file using this format (OTU_id Size Iden Acc Piden Aln_Len Bitscore Frequencies) [REQUIRED]"))
parser.add_argument("-aln_thr", dest="AThres_Value",type=int,default=100, help=("Minimum Alignment Length for hit [default=100]"))
parser.add_argument("-report", dest="Report_filename",default="", help=("Program Report filename (Contain informations about rejected OTUs) [OPTIONAL]"))
parser.add_argument("-checkall", action='store_true', help=("Check validity for the whole file (This will take more time)"))
parser.add_argument("-out", dest="output_filename",required=True, help=("Output Filename [REQUIRED]"))




#args=parser.parse_args('-in Donnee_Anticosti_MO.new.besthits_with_missing.blast -checkall -report report.txt -out result.blast'.split())
#args=parser.parse_args('-in test.blast -checkall -report report.txt -out result.blast'.split())
args=parser.parse_args()


class BH_Blastline:
    def __init__(self,line):
        spl=line.replace("\n","").split("\t")
        self.id=spl[0]
        self.real_id=int(spl[0].split("OTU_")[1])
        self.size=int(spl[1])
        self.iden=spl[2]
        self.acc=spl[3]
        self.piden=spl[4]
        self.aln=int(spl[5])
        self.bscore=spl[6]
        spl[7]
        freq_list=[]
        for i in spl[7:]:
            freq_list.append(int(i))
        self.freqs=freq_list
    def __lt__(self,other):
        return self.real_id < other.real_id
    def __gt__(self,other):
        return self.real_id > other.real_id
    def increment(self,other):
        self.size=self.size+other.size
        self.freqs=list(map(lambda x: x[0] + x[1], zip(self.freqs,other.freqs)))
    def Convert_freqs_2_str(self):
        res=""
        for i in self.freqs:
            res+=str(i)+"\t"
        res=res[:-1]
        return res
    def str_repr(self):
        line=self.id+"\t"+str(self.size)+"\t"+self.iden+"\t"+self.acc+"\t"+self.piden+"\t"+str(self.aln)+"\t"+self.bscore+"\t"+self.Convert_freqs_2_str()
        return line
    def __repr__(self):
        line=self.id+"\t"+str(self.size)+"\t"+self.iden+"\t"+self.acc+"\t"+self.piden+"\t"+str(self.aln)+"\t"+self.bscore+"\t"+self.Convert_freqs_2_str()
        return line
    

#def Compress_BH_obj(BH_obj_list):
#    BH_obj_list.sort()
#    lead=BH_obj_list[0]
#    for i in BH_obj_list[1:]:
#        lead.increment(i)
#    return lead

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

report_flag=False

if args.Report_filename != "":
    report_flag=True
    reportfile=open(args.Report_filename,'w')
    reportfile.write("OTUs_ID\tRejection_Cause\n")


print("Reading Besthit input file...")

blastL=blast.readlines()
blast.close()

print("Verifing file Format...")
if args.checkall:
    print("Exhaustive File Check option detected...")
    print("Processing...")
if args.checkall != True:
    incase=1
    while True:
        if "INCONNU" in blastL[incase]:
            incase+=1
        else:
            break
    try:
        BH_Blastline(blastL[incase])
    except ValueError:
            print("ValueError")
            print("ERROR: Invalid Besthit Blast format")
            print("EXITING PROGRAM")
            sys.exit(1)
    except IndexError:
            print("IndexError")
            print("ERROR: Invalid Besthit Blast format")
            print("EXITING PROGRAM")
            sys.exit(1)
else:
    long=len(blastL[1].split("\t"))
    count=0
    if blastL[1].split("\t")[0] == blastL[2].split("\t")[0]:
        print("ERROR: The input file is a Blast file, not Besthits Blast (OTU_id redundant)")
        sys.exit(1)
    for i in blastL[1:]:
        count+=1
        if "INCONNU" in i:
            continue
        if len(i.split("\t")) != long:
            print("ERROR: Inconsistent Number of elements (line: %d)"%(count))
            sys.exit(1)
        try:
            BH_Blastline(i)
        except ValueError:
            print("ValueError")
            print("ERROR: Invalid Besthit Blast format (line: %d)"%(count))
            print("EXITING PROGRAM")
            sys.exit(1)
        except IndexError:
            print("IndexError")
            print("ERROR: Invalid Besthit Blast format ( No frequencies found ) (line: %d)"%(count))
            print("EXITING PROGRAM")
            sys.exit(1)
print("File Check Complete\n")
#sys.exit()
Acc_dict={}

savefile=open(args.output_filename,'w')
try:
    int(blastL[0].split("\t")[1])
except ValueError:
    savefile.write(blastL.pop(0))
else:
    pass

key_log=[]




print("Constructing and indexing #Accession Lists...") 
for i in blastL:
    #print(i)
    if "INCONNU" in i:
        if report_flag:
            reportfile.write(i.split("\t")[0]+"\t"+"Unknown Identification\n")
        continue
    bl_obj=BH_Blastline(i)
    if bl_obj.aln < args.AThres_Value:
        if report_flag:
            reportfile.write(bl_obj.id+"\t"+"Alignment Length too short ( < "+str(args.AThres_Value)+")\n")
        continue
    try:
        Acc_dict[bl_obj.acc].append(bl_obj)
    except KeyError:
        Acc_dict[bl_obj.acc]=[bl_obj]
        key_log.append(bl_obj.acc)

blastL=""

corres_check=[]
#key_log=Acc_dict.keys()
print("Compressing OTUs and writing results to file...")
for i in key_log:
    obj_list=Acc_dict[i]
    
    if len(obj_list) == 1:
        savefile.write(obj_list[0].str_repr()+"\n")
    else:
        temp=[]
        for j in obj_list[1:]:
            obj_list[0].increment(j)
            temp.append(j.id)
        corres_check.append([obj_list[0].id,temp])
        savefile.write(obj_list[0].str_repr()+"\n")

savefile.close()
if report_flag:
    reportfile.close()

print("PROGRAM DONE")
        

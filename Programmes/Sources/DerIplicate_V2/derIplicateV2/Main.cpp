/*
 * Main.cpp
 *
 *  Created on: 18 Jun 2015
 *      Author: pagagne
 */

#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <deque>
#include "SimpleFastaSequence.h"
#include "FastaSequence.h"
#include "FastqSequence.h"
#include <algorithm>
#include <numeric>
#include <pthread.h>
#include <thread>
#include <cstdlib>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <set>
#include <list>
#include <map>
#include <tclap/CmdLine.h>

using namespace PHF;
using namespace std;
using namespace PHC;
using namespace DNAF;
using namespace cutlib;



struct ResultSeq
{
	SimpleFastaSequence* p_Seq;
	string s_Redundancy;
	//int ele_count=0;
};



bool line_sort_Tab2(ResultSeq s1, ResultSeq s2)
{
	return ( s1.p_Seq->reqLine() < s2.p_Seq->reqLine());
}

string FindRedundancy(const map<string,string> &redu_map, const string& seqname){
	map<string,string>::const_iterator itera;
	itera=redu_map.find(seqname);
	if (redu_map.find(seqname) != redu_map.end()){
		return itera->second;
	}else{
		return seqname;
	}
}


bool Seq_unique(SimpleFastaSequence* s1, SimpleFastaSequence* s2)
{
	return ( s1->reqSeq() == s2->reqSeq());
}

struct ltstr
{
	bool operator()(SimpleFastaSequence* s1, SimpleFastaSequence* s2)
	{
		return ( s1->reqSeq() < s2->reqSeq());
	}
};


void *TrimSequences_Thread(void *threadarg)
{

	pthread_exit(NULL);
}

void print(const std::string& item)
{
	cout << item << endl;
	cin.ignore();
}

//bool operator<(SimpleFastaSequence const & s1, SimpleFastaSequence const & s2){return s1.reqSeq() < s2.reqSeq();}

int main(int argc, char** argv)
{
	try{

			TCLAP::CmdLine cmd("This program remove sequences redundancy from fasta file and produce a mothur-type name file to keep track of redundancy", ' ', "2.0");

			TCLAP::SwitchArg debug_messagesArg("d","debug","Display debug messages",cmd,false);

			TCLAP::ValueArg<std::string> name_redun_inArg("r","name_redundancy","mothur-like names containing redundancy (to update the name output) (OPTIONAL)", false,"","name file");
			cmd.add(name_redun_inArg);

			TCLAP::ValueArg<std::string> name_outArg("n","name_out","mothur-like names output name (REQUIRED)", true,"", "filename");
			cmd.add(name_outArg);

			TCLAP::ValueArg<std::string> fasta_outArg("o","fasta_out","Fasta output name (REQUIRED)", true,"","filename");
			cmd.add(fasta_outArg);

			TCLAP::ValueArg<std::string> fasta_inArg("f","fasta_in","Fasta file input (REQUIRED)", true,"","fasta");
			cmd.add(fasta_inArg);

			cmd.parse(argc, argv);




		// Option Parsing


		    string filename=fasta_inArg.getValue();
			string fasta_savefile=fasta_outArg.getValue();
			string name_savefile=name_outArg.getValue();
			string redundancy_namefile=name_redun_inArg.getValue();

			bool add_redun_flag=false;
			if (redundancy_namefile != ""){
				add_redun_flag=true;
			}
			bool display_debug=debug_messagesArg.getValue();

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
	string filetype="fasta";
	list<SimpleFastaSequence> Work_Container;
		ifstream ifs_count(filename,ios::in);

		if(!ifs_count)
		{
			cerr << "Ouverture de " << filename << " impossible" << endl;
			return -1;
		}

		cout << "LECTURE DU FICHIER" << endl;
		int nb_lines=0;
		for (string lines; getline( ifs_count, lines );)
		{
			nb_lines++;
		}
		ifs_count.close();


		if(nb_lines % 2 != 0){
			cout << "ERROR, Invalid fasta file" << endl;
			return -1;
		}


		cout << nb_lines << endl;
		cout << ((nb_lines/2)) << endl;

		ifstream ifs(filename,ios::in);
		int nb_seqs=0;
		int line_pos=0;
		for (string line; getline( ifs, line );)
		{
			string seqname=line;
			line_pos++;
			getline(ifs, line);
			string seq=line;
			SimpleFastaSequence FSequence(seqname, seq);
			FSequence.asgLine(line_pos);
			Work_Container.push_back( FSequence );
			nb_seqs++;
		}
		ifs.close();


		//cout << "Multi_Thread Prêt (APPUYER SUR RETURN)" << endl;
		//cin.ignore();

		cout << "Sorting by Sequences Alpha" << endl;

		Work_Container.sort();

		map <string,string> redundancy_add;
				if (redundancy_namefile != ""){
					cout << "Reading and mapping Redundancy Name file" << endl;
					ifstream name_redun_stream(redundancy_namefile,ios::in);
					string leading_seq;
					string seqlist;
					for (string line; getline( name_redun_stream, line );)
					{
						leading_seq=line.substr(0,line.find("\t"));
						seqlist=line.substr(line.find("\t")+1);
						redundancy_add.insert(pair<string,string>(leading_seq,seqlist));
					}
				}

		//cout << FindRedundancy(redundancy_add,"M02842_12_000000000AMUMV_1_1107_10953_5938_8") << endl;
///////////////////////////////////////////////////////////////////////////////
		set<SimpleFastaSequence*, ltstr> Seqset; //Set des séquences uniques (se basant sur l'alpahbet de la séquence pour la similarité)
		set<SimpleFastaSequence*>::iterator it;
		pair<set<SimpleFastaSequence*>::iterator,bool> ret; //Itérateur permettant de savoir si une valeur à été ajouté ou non
		vector<ResultSeq> v_Redundancy;


		list<SimpleFastaSequence>::iterator iter;

	cout << "Writing Vectors Pointers" << endl;
	int first = 0;
	int count = 0;
	int treatedseq = 0;
	string Redundancy_line="";
	ResultSeq s_Seq;
	for (iter = Work_Container.begin(); iter != Work_Container.end(); ++iter) {



		SimpleFastaSequence * myp;
		myp = &(*iter);

		treatedseq++;

		ret = Seqset.insert(myp);
		if (first == 0) {
			first = 1;
			s_Seq.p_Seq=myp;
			if (add_redun_flag == true){
				Redundancy_line+=FindRedundancy(redundancy_add,myp->reqNomSeq())+",";
			}else{
				Redundancy_line+=myp->reqNomSeq()+",";
			}
			//s_Seq.ele_count+=1;
			continue;
		}
		if (ret.second == false) {
			if (add_redun_flag == true){
				Redundancy_line+=FindRedundancy(redundancy_add,myp->reqNomSeq())+",";
			}else{
				Redundancy_line+=myp->reqNomSeq()+",";
			}
			//s_Seq.ele_count+=1;
		} else {
			count++;
			s_Seq.s_Redundancy=Redundancy_line.substr(0,Redundancy_line.size()-1);
			v_Redundancy.push_back(s_Seq);
			Redundancy_line="";
			s_Seq.p_Seq=myp;
			if (add_redun_flag == true){
				Redundancy_line+=FindRedundancy(redundancy_add,myp->reqNomSeq())+",";
			}else{
				Redundancy_line+=myp->reqNomSeq()+",";
			}
			//s_Seq.ele_count+=1;

			if (treatedseq % 25000 == 0) {
				std::cout<< "\r";
				cout << ((nb_lines / 2) - treatedseq) << "\t" << count << "             " << std::flush;
			}

		}

	}
	if (Redundancy_line.size() != 0) {
		s_Seq.s_Redundancy=Redundancy_line.substr(0,Redundancy_line.size()-1);
		v_Redundancy.push_back(s_Seq);
	}
	std::cout<< "\r";
	cout << "0\t" << v_Redundancy.size() << "             " << std::flush;
	cout << endl;
//////////////////////////////////////////////////////////////////////////

		cout << "Sorting Writing Matrix" << endl;


		sort(v_Redundancy.begin(),v_Redundancy.end(),line_sort_Tab2);


		cout << "Writing Names and Fasta files" << endl;



//////////////////////////////////////////////////////////////////////////
		ofstream savefile_fasta;
		ofstream savefile_names;


		savefile_fasta.open(fasta_savefile.c_str(),ios::out);

		cout << "Writing Fasta file..." << endl;

		for (int i=0; i < v_Redundancy.size(); i++)
		{
			savefile_fasta << v_Redundancy[i].p_Seq->reqFastaFormat();
		}

		savefile_fasta.close();

		savefile_names.open(name_savefile.c_str(),ios::out);

		cout << "Writing Names file..." << endl;

		for (int i=0; i < v_Redundancy.size(); i++)
		{
			savefile_names << v_Redundancy[i].p_Seq->reqNomSeq() << "\t" << v_Redundancy[i].s_Redundancy << "\n";
		}


		savefile_names.close();

		cout << "FIN DE PROGRAMME" << endl;


	}catch(TCLAP::ArgException &e)
			{
				cerr << "ERROR: " << e.error() << std::endl;
			}
	return 0;
}

/*
 * if (redundancy_namefile == ""){
		for (int i=0; i < v_Redundancy.size(); i++)
		{
			savefile_names << v_Redundancy[i].p_Seq->reqNomSeq() << "\t" << v_Redundancy[i].s_Redundancy << "\n";
		}
		}
 * if (redundancy_namefile != ""){
			cout << "Adding additionnal name file redundancy" << endl;
			map<string,string>::iterator itera;
			string sub_test;
			int ele_count;
			string sep=",";
			size_t sep_pos;
			string new_redundancy_str;
			for (int i=0; i < v_Redundancy.size(); i++){
				sub_test=v_Redundancy[i].s_Redundancy;
				ele_count=v_Redundancy[i].ele_count;
				sep_pos=-1;
				new_redundancy_str="";
				for (int i=0; i<ele_count;i++){
					sub_test=sub_test.substr(sep_pos+1);
					if (i == ele_count-1){
						//cout << sub_test << endl;
						itera=redundancy_add.find(sub_test);
						if (itera != redundancy_add.end()){
							new_redundancy_str+=itera->second+",";
						}else{
							new_redundancy_str+=sub_test+",";
						}
					}
					else{
						sep_pos=sub_test.find(sep);
						//cout << sub_test.substr(0,sep_pos) << endl;
						itera=redundancy_add.find(sub_test.substr(0,sep_pos));
						if (itera != redundancy_add.end()){
							new_redundancy_str+=itera->second+",";
						}else{
							new_redundancy_str+=sub_test.substr(0,sep_pos)+",";
						}
					}
				}
				new_redundancy_str=new_redundancy_str.substr(0,new_redundancy_str.size()-1);
				savefile_names << v_Redundancy[i].p_Seq->reqNomSeq() << "\t" << new_redundancy_str << "\n";
			}
		}
*/


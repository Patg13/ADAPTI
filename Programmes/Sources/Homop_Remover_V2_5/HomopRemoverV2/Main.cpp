/*
 * Main.cpp
 *
 *  Created on: 18 Jun 2015
 *      Author: pagagne
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <pthread.h>
#include <thread>
#include <cstdlib>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include "Repartition.h"
#include <map>
#include <ctime>
#include <chrono>
#include <tclap/CmdLine.h>


using namespace std;



// Progression Bars Code
void printProgBar( int percent ){
  std::string bar;

  for(int i = 0; i < 50; i++){
    if( i < (percent/2)){
      bar.replace(i,1,"=");
    }else if( i == (percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }

  std::cout<< "\r" "[" << bar << "] ";
  std::cout.width( 3 );
  std::cout<< percent << "%     " << std::flush;
}

void printProgBarPro( int percent, const string& nom_pro ){
  std::string bar;

  for(int i = 0; i < 50; i++){
    if( i < (percent/2)){
      bar.replace(i,1,"=");
    }else if( i == (percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }

  std::cout<< "\r" "[" << bar << "] ";
  std::cout.width( 3 );
  std::cout<< percent << "%     "<< nom_pro << " " << std::flush;
}

bool Homop_Too_Long(const string& seq, std::vector<string>* homop_list){
	for ( int i = 0; i != homop_list->size(); i++){
		if (seq.find((*homop_list)[i]) != string::npos){
			return true;
		}
	}
	return false;
}

bool Check_Size_Tag(const string& seqname){
	size_t s_tag = seqname.find(";size=");
	if (s_tag == string::npos){
		return false;
	}
	size_t e_tag = seqname.find(";",s_tag+6);
	if (e_tag == string::npos){
		return false;
	}
	if (atoi(seqname.substr(s_tag+6,e_tag).c_str()) == 0){
		return false;
	}
	return true;

}
int Extract_Size(const string& seqname){
	size_t s_tag = seqname.find(";size=")+6;
	size_t e_tag = seqname.find(";",s_tag);
	return atoi(seqname.substr(s_tag,e_tag).c_str());
}

// END



struct thread_data
{
	int homop_thre;
	int size_thre;
	int thread_id;
	string readfasta_filename;
	string writefasta_filename_good;
	string writefasta_filename_scrap;
	int file_start_index;
	int file_end_index;
};

// Main multithread Function
void *Homop_Sequences_Thread(void *threadarg)
{

	struct thread_data *my_data;

	my_data = (struct thread_data *) threadarg;

	ifstream Seqfile(my_data->readfasta_filename,ios::in);
	ofstream Goodtemp;
	ofstream Scraptemp;

	Goodtemp.open(my_data->writefasta_filename_good,ios::out);
	if (my_data->writefasta_filename_scrap != ""){
		Scraptemp.open(my_data->writefasta_filename_scrap,ios::out);
	}

	int num_line=0;
	int nb_seqs=0;

	string seqname;
	string seq;

	int totalseq=((my_data->file_end_index-my_data->file_start_index)+1)/2;
	string pro_name="Thread: "+to_string(my_data->thread_id);

	vector<string> v_homoplist;
	string A_homop (my_data->homop_thre + 1,'A');
	string T_homop (my_data->homop_thre + 1,'T');
	string C_homop (my_data->homop_thre + 1,'C');
	string G_homop (my_data->homop_thre + 1,'G');
	string U_homop (my_data->homop_thre + 1,'U');

	v_homoplist.push_back(A_homop);
	v_homoplist.push_back(T_homop);
	v_homoplist.push_back(C_homop);
	v_homoplist.push_back(G_homop);
	v_homoplist.push_back(U_homop);

	int seq_size=-1;
	bool seq_status=true;

	for (string lines; getline( Seqfile, lines );)
			{
				num_line++;
				if (num_line == my_data->file_start_index)
				{
					//cout << my_data->thread_id << " " << lines << " " << num_line << endl;
					seqname=lines;
					getline( Seqfile, seq);
					num_line++;
					break;
				}
			}

	// First Sequence Treatment

	if (my_data->size_thre != 0){
		seq_size=Extract_Size(seqname);
	}
	if (Homop_Too_Long(seq,&v_homoplist) == true)
	{
		seq_status=false;
		if (seq_size >= my_data->size_thre){
			seq_status=true;
		}

	}
	if (seq_status == true)
	{
		Goodtemp << seqname << "\n";
		Goodtemp << seq << "\n";
	}
	else if((seq_status == false) && (my_data->writefasta_filename_scrap != ""))
	{
		Scraptemp << seqname << "\n";
		Scraptemp << seq << "\n";
	}


	while(true)
	{
		getline (Seqfile, seqname);
		num_line++;
		getline (Seqfile, seq);
		num_line++;
		if (num_line > my_data->file_end_index)
		{
			break;
		}
		if (nb_seqs % 25001 == 0){
			printProgBarPro((((nb_seqs*100))/totalseq), pro_name);

		}
		seq_status=true;
		if (my_data->size_thre != 0){
			seq_size=Extract_Size(seqname);
		}
		if (Homop_Too_Long(seq,&v_homoplist) == true)
		{
			seq_status=false;
			if (seq_size >= my_data->size_thre){
				seq_status=true;
			}

		}
		if (seq_status == true)
		{
			Goodtemp << seqname << "\n";
			Goodtemp << seq << "\n";
		}
		else if((seq_status == false) && (my_data->writefasta_filename_scrap != ""))
		{
			Scraptemp << seqname << "\n";
			Scraptemp << seq << "\n";
		}

		nb_seqs+=1;

	}

	//string Pro_name="0";


	printProgBarPro(100,pro_name);
	//cout << seq_count << endl;

	Goodtemp.close();
	if (my_data->writefasta_filename_scrap != ""){
		Scraptemp.close();
	}

	pthread_exit(NULL);
}


// Section de configuration des options



int main(int argc, char** argv)
{
	//Option List
	/*
	 * Fasta infile REQ
	 * Homop threshold REQ
	 * Size threshold OPT
	 * Out filename REQ
	 * Number of Threads def:1 OPT
	 * Scrap filename OPT
	 */

	try{

		TCLAP::CmdLine cmd("This program check nucleotides sequences and remove those containing homopolymers > threshold, but keep those where size >= threshold", ' ', "2.5");

		TCLAP::SwitchArg debug_messagesArg("d","debug","Display debug messages",cmd,false);

		TCLAP::ValueArg<int> nb_threadArg("t","num_threads","Number of threads to use (Default: 1)",false,1," [integer > 0] ");
		cmd.add(nb_threadArg);

		TCLAP::ValueArg<int> size_thrArg("i","size_thr","Minimal sequence size threshold ( everything greater or equal will bypass rejection, must be greater then 0 and sequences must have ;size=xxxx; tags ) (OPTIONAL)",false,-9999," [integer > 0] ");
		cmd.add(size_thrArg);

		TCLAP::ValueArg<int> homop_thrArg("l","homop_thr","Homopolymers length threshold ( everything greater is rejected, must be greater then 1 ) (REQUIRED)", true,0, " [integer > 1] ");
		cmd.add(homop_thrArg);

		TCLAP::ValueArg<std::string> sfasta_outArg("s","scrap_out","Scrap fasta output name (Sequences rejected) (OPTIONAL)", false,"","filename");
		cmd.add(sfasta_outArg);

		TCLAP::ValueArg<std::string> fasta_outArg("o","fasta_out","Fasta output name (REQUIRED)", true,"","filename");
		cmd.add(fasta_outArg);

		TCLAP::ValueArg<std::string> fasta_inArg("f","fasta_in","Fasta file input (REQUIRED)", true,"","fasta");
		cmd.add(fasta_inArg);

		cmd.parse(argc, argv);




	// Option Parsing


	    string filename=fasta_inArg.getValue();
		string fasta_savefile=fasta_outArg.getValue();
		string scrap_savefile=sfasta_outArg.getValue();
		int homop_thr=homop_thrArg.getValue();
		int size_thr=size_thrArg.getValue();
		int num_threads=nb_threadArg.getValue();

		bool display_debug=debug_messagesArg.getValue();

		bool scrap_flag;
		if (scrap_savefile.empty()){
			scrap_flag=false;
		}else{
			scrap_flag=true;
		}


		if ( size_thr != -9999 ){
			if (size_thr  < 1)
			{
				throw TCLAP::ArgException("Invalid Size threshold (must be an integer > 0) ");
			}
		}else{
			size_thr=0;
		}

		if (homop_thr < 2){
			throw TCLAP::ArgException("Invalid Homopolymers length threshold (must be an integer > 1)");
		}

		if (num_threads < 1){
			throw TCLAP::ArgException("Invalid number of threads (must be an integer > 0)");
		}

		if (display_debug)
		{
			cout << "fasta in: " << filename << endl;
			cout << "fasta savename: " << fasta_savefile << endl;
			cout << "Scrap fasta savename: " << scrap_savefile << endl;
			cout << "Scrap flag: " << scrap_flag << endl;
			cout << "Homopolymers Threshold: " << homop_thr << endl;
			cout << "Size Threshold: " << size_thr << endl;
			cout << "Number of threads: " << num_threads << endl << endl;
			string A_homop (homop_thr + 1,'A');
			string T_homop (homop_thr + 1,'T');
			string C_homop (homop_thr + 1,'C');
			string G_homop (homop_thr + 1,'G');
			string U_homop (homop_thr + 1,'U');
			cout << A_homop << endl;
			cout << T_homop << endl;
			cout << C_homop << endl;
			cout << G_homop << endl;
			cout << U_homop << endl << endl;
		}

		auto t1 = chrono::high_resolution_clock::now();
//##########################################################################################################
			//TEST ZONE

//##########################################################################################################
		// Program Start

		ifstream ifs_count(filename,ios::in);


		if(!ifs_count)
		{
			cerr << "Ouverture de " << filename << " impossible" << endl;
			return -1;
		}

		string Size_verif;
		cout << "Reading Fasta File" << endl;
		int nb_lines=0;
		for (string lines; getline( ifs_count, lines );)
		{
			if (nb_lines==0){
				Size_verif=lines;
				if (lines[0] != '>'){
					throw TCLAP::ArgException("Invalid fasta file format (first char not >)");
				};
			}
			if (nb_lines==2){
				if (lines[0] != '>'){
					throw TCLAP::ArgException("Invalid fasta file format (Nucleotide sequence on multiple line)");
				};
			}
			nb_lines++;
		}
		ifs_count.close();

		if(nb_lines % 2 != 0){
			throw TCLAP::ArgException("Invalid fasta file (odd number of lines)");
		}


		cout << nb_lines << " lines" <<  endl;
		cout << ((nb_lines/2)) << " sequences" << endl << endl;





		if (size_thr != 0){
			if (display_debug){
				cout << Size_verif << endl;
				cout << Check_Size_Tag(Size_verif) << endl;
				cout << Extract_Size(Size_verif) << endl << endl;
			}
			if (Check_Size_Tag(Size_verif) == false){
				throw TCLAP::ArgException(";size=xxxx; tag not found");
			}
		}



		vector<int> v_repartition_num;

		cout << "File splitting Calculation for " <<  num_threads << " threads..." << endl;
		RepartitionChargeNum(&v_repartition_num,num_threads,nb_lines/2);

		if (display_debug)
		{
			int sum=0;

			for(int i=0;i != v_repartition_num.size(); i++){
				cout << v_repartition_num[i] << endl;
				sum=sum+v_repartition_num[i];
		}
		cout << "Sum = " << sum << endl;

		}
		cout << "Creating File processing map..." << endl;

		map<int,pair<int,int>> proc_map;

		int start_index=1;
		int end_index=0;

		for( int i =0; i < v_repartition_num.size(); i++){

			if (i != v_repartition_num.size()-1 )
			{
				end_index=v_repartition_num[i]*2*(i+1);
				proc_map.insert(pair<int,pair<int,int>>(i,make_pair(start_index,end_index)));
				start_index=end_index+1;
			}else
			{
				end_index=nb_lines;
				proc_map.insert(pair<int,pair<int,int>>(i,make_pair(start_index,end_index)));
			}

		}

		map<int,pair<int,int>>::iterator itera;

		if (display_debug){


		int summe=0;
		for( itera=proc_map.begin(); itera != proc_map.end();++itera)
		{
			cout << "Thread " << (*itera).first << " [" << (*itera).second.first << "," << (*itera).second.second << "] " << ((*itera).second.second - (*itera).second.first + 1)/2  << endl;
			summe=summe+(((*itera).second.second - (*itera).second.first + 1)/2);
		}
		cout << summe << endl;
		}

		//cout << result << endl;


	vector<string> v_tempfiles_good;
	for (int i =0; i< num_threads; i++)
	{
		v_tempfiles_good.push_back("."+filename+"."+to_string(i)+".good"+".htemp");
		if (display_debug){
			cout << v_tempfiles_good[i] << endl;
		}
	}
	vector<string> v_tempfiles_scrap;
	if (scrap_savefile != ""){
		for (int i =0; i< num_threads; i++)
		{
			v_tempfiles_scrap.push_back("."+filename+"."+to_string(i)+".scrap"+".htemp");
			if (display_debug){
				cout << v_tempfiles_scrap[i] << endl;
			}
		}
	}else{
		for (int i =0; i< num_threads; i++)
		{
			v_tempfiles_scrap.push_back("");
			if (display_debug){
				cout << v_tempfiles_scrap[i] << endl;
			}
		}
	}


	    cout << "Launching Threads..." << endl;
		pthread_t threads[num_threads];
		struct thread_data td[num_threads];
		int rc;
		int i;
		pthread_attr_t attr;
		void *status;

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);



		for (i=0; i < num_threads; i++)
		{
			if (display_debug){
				cout << "main() : creating thread, " << i << endl;
			}
			td[i].thread_id=i;
			td[i].readfasta_filename=filename;
			td[i].writefasta_filename_good=v_tempfiles_good[i];
			td[i].writefasta_filename_scrap=v_tempfiles_scrap[i];
			td[i].homop_thre=homop_thr;
			td[i].size_thre=size_thr;
			td[i].file_start_index=proc_map.find(i)->second.first;
			td[i].file_end_index=proc_map.find(i)->second.second;
			rc = pthread_create(&threads[i], NULL, Homop_Sequences_Thread, (void *)&td[i]);
			if (rc)
			{
				cout << "ERREUR, cannot create thread " << rc << endl;
				exit(-1);
			}
		}

		pthread_attr_destroy(&attr);
		for( i=0; i< num_threads; i++)
		{
			rc = pthread_join(threads[i], &status);
			if (rc){
				cout << "ERROR, Cannot join" << rc <<endl;
				exit(-1);
			}
		}


	cout << "Threads DONE..." << endl;
	string Regroup_command_linux_good;
	string Clean_command_linux_good;
	string Regroup_command_linux_scrap;
	string Clean_command_linux_scrap;

	cout << endl << "Executing system commands..." << endl;
        Regroup_command_linux_good="cat ";
        for (i=0; i< num_threads; i++)
        {
            Regroup_command_linux_good=Regroup_command_linux_good+v_tempfiles_good[i]+" ";
        }
        Regroup_command_linux_good=Regroup_command_linux_good+"> "+fasta_savefile;
	//Regroup_command_linux_good="cat ."+filename+".*"+".good.htemp > "+fasta_savefile;
	Clean_command_linux_good="rm -f ."+filename+".*"+".good.htemp";

	if (scrap_flag == true){
                Regroup_command_linux_scrap="cat ";
                for (i=0; i< num_threads; i++)
                {
                    Regroup_command_linux_scrap=Regroup_command_linux_scrap+v_tempfiles_scrap[i]+" ";
                }
                Regroup_command_linux_scrap=Regroup_command_linux_scrap+"> "+scrap_savefile;
		//Regroup_command_linux_scrap="cat ."+filename+".*"+".scrap.htemp > "+scrap_savefile;
		Clean_command_linux_scrap="rm -f ."+filename+".*"+".scrap.htemp";
	}

	cout << endl;
	cout << "Regrouping temp files..."  << endl;
        //cout << Regroup_command_linux_good << endl;
	system(Regroup_command_linux_good.c_str());
	if (scrap_flag){
                //cout << Regroup_command_linux_scrap << endl;
		system(Regroup_command_linux_scrap.c_str());
	}
	cout << "Cleaning temp files..."  << endl << endl;
	system(Clean_command_linux_good.c_str());
	if (scrap_flag){
		system(Clean_command_linux_scrap.c_str());
	}

	auto t2 = chrono::high_resolution_clock::now();
	cout << endl << "Time: " << chrono::duration_cast<chrono::seconds>(t2-t1).count();
	cout << endl << "Time: " << chrono::duration_cast<chrono::minutes>(t2-t1).count();
	cout << endl << "Program DONE" << endl;


	}catch(TCLAP::ArgException &e)
		{
			cerr << "ERROR: " << e.error() << std::endl;
		}
		return 0;


}

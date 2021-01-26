/*
 * Main.cpp
 *
 *  Created on: 18 Jun 2015
 *      Author: pagagne
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <pthread.h>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <cstdlib>
#include "Repartition.h"
#include <map>
#include <ctime>
#include <chrono>
#include <tclap/CmdLine.h>


using namespace std;

int CountIntLen(int integer){
	int lengthcount=0;
	for(; integer!=0; integer/=10, lengthcount++);
	if (lengthcount==0){
		lengthcount=1;
	}
	return lengthcount;
}

string PrintTime(const int& r_seconds){
	ostringstream os;
	if (r_seconds == 0)
	{
		return "00:00:00";
	}
	int seconds=(r_seconds % 60);
	int minutes=(r_seconds/60)%60;
	int hours=(r_seconds/60/60)%24;

	if (CountIntLen(hours) == 1){
		os << "0" << hours;
	}else{
		os << hours;
	}
	os << ":" ;

	if (CountIntLen(minutes) == 1){
		os << "0" << minutes;
	}else{
		os << minutes;
	}
	os << ":" ;
	if (CountIntLen(seconds) == 1){
		os << "0" << seconds;
	}else{
		os << seconds;
	}
	return os.str();
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
  std::cout<< percent << "%     "<< nom_pro << "    " << std::flush;
}

int CountMinStopCodon(const string& r_seq, const int& r_limit){
	int count;
	string subseq;
	string reverse;
	int lowest_count=999;
	for (int offset=0; offset < 3; offset++){
		count=0;
		//cout << "Offset: " << 1+offset << endl;
		for (int i=offset;i < r_seq.length(); i=i+3){
			subseq=(r_seq.substr(i,3));
			//cout << subseq << endl;
			if (subseq == "TAA" or subseq == "TAG"){
				count++;
				//cout << count << endl;
				if (count > r_limit){
					break;
				}
			}

		}
		//cout << "--------------" << endl;
	if (count < lowest_count){
		lowest_count=count;
		if (lowest_count <= r_limit ){
			//cout << "Offset: " << 1+offset << endl;
			return lowest_count;
		}
	}
	}
	reverse = string (r_seq.rbegin(), r_seq.rend());
	for (int offset=0; offset < 3; offset++){
			count=0;
			//cout << "Offset: " << (1+offset)*-1 << endl;
			for (int i=offset;i < reverse.length(); i=i+3){
				subseq=(reverse.substr(i,3));
				//cout << subseq << endl;
				if (subseq == "ATT" or subseq == "ATC"){
					count++;
					//cout << count << endl;
					if (count > r_limit){
						break;
					}
				}

			}
			//cout << "--------------" << endl;
		if (count < lowest_count){
			lowest_count=count;
			if (lowest_count <= r_limit ){
				//cout << "Offset: " << (1+offset)*-1 << endl;
				return lowest_count;
			}
		}
	}
	return lowest_count;
}


struct thread_data
{
	int thread_id;
	int stop_thre;
	int file_start_index;
	int file_end_index;
	string readfasta_filename;
	string writefasta_filename_good;
	string writefasta_filename_scrap;

};

// Main multithread Function
void *CStop_Sequences_Thread(void *threadarg)
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

	bool seq_status=true;

	for (string lines; getline( Seqfile, lines );)
			{
				num_line++;
				if (num_line == my_data->file_start_index)
				{
					seqname=lines;
					getline( Seqfile, seq);
					num_line++;
					break;
				}
			}

	// First Sequence Treatment

	if (CountMinStopCodon(seq,my_data->stop_thre) > my_data->stop_thre)
	{
		seq_status=false;
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

		if (CountMinStopCodon(seq,my_data->stop_thre) > my_data->stop_thre)
		{
			seq_status=false;
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

		TCLAP::CmdLine cmd("This program filter Cox sequences and reject those where the protein translation contain multiple stop codon", ' ', "2.0");

		TCLAP::SwitchArg debug_messagesArg("d","debug","Display debug messages",cmd,false);

		TCLAP::ValueArg<int> nb_threadArg("t","num_threads","Number of threads to use (Default: 1)",false,1," [integer > 0] ");
		cmd.add(nb_threadArg);

		TCLAP::ValueArg<int> stop_thrArg("l","stop_thr","Stop codon count threshold ( everything greater is rejected, must be greater or equal then 0  ) (REQUIRED)", true,0, " [integer >= 0] ");
		cmd.add(stop_thrArg);

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
		int stop_thr=stop_thrArg.getValue();
		//int size_thr=size_thrArg.getValue();
		int num_threads=nb_threadArg.getValue();

		bool display_debug=debug_messagesArg.getValue();

		bool scrap_flag;
		if (scrap_savefile.empty()){
			scrap_flag=false;
		}else{
			scrap_flag=true;
		}


		if (stop_thr < 0){
			throw TCLAP::ArgException("Invalid Stop codon count threshold (must be an integer > 1)");
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
			cout << "Stop Codon count Threshold: " << stop_thr << endl;
			cout << "Number of threads: " << num_threads << endl << endl;
		}

		auto t1 = chrono::high_resolution_clock::now();
//##########################################################################################################
			//TEST ZONE

		//string seq="ACCCTCCCTTACATTCTTATTAATAAGAAGAATTGTAGATAAAGGAGCTGGCACAGGCTGAACAGTGTATCCTCCCTTAGCAAGAAATATTGCACATGAGGGCCCTTCTGTAGATTTGGCAATTTTCAGCCTTCATATAGCAGGAATCTCCTCTATTTTAGGAGCAATAAATTTTATTGCAACTACAATCAATATACACCCCCTAGGATGTAAAACAGAACAACTTACCCTTTTACATGAGCTGTAAAAATTACAGCAGTTTTATTACTTTTATCTCTCCCAGTACTTGCTGGAGCAATTACAATACTTTTGACAGAT";

		//cout << CountMinStopCodon(seq,0) << endl;

		//return 0;

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
			td[i].stop_thre=stop_thr;
			//td[i].size_thre=size_thr;
			td[i].file_start_index=proc_map.find(i)->second.first;
			td[i].file_end_index=proc_map.find(i)->second.second;
			rc = pthread_create(&threads[i], NULL, CStop_Sequences_Thread, (void *)&td[i]);
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


	cout << endl << "Threads DONE..." << endl;
	string Regroup_command_linux_good;
	string Clean_command_linux_good;
	string Regroup_command_linux_scrap;
	string Clean_command_linux_scrap;

	cout << endl << "Executing system commands..." << endl;
	Regroup_command_linux_good="cat ."+filename+".*"+".good.htemp > "+fasta_savefile;
	Clean_command_linux_good="rm -f ."+filename+".*"+".good.htemp";

	if (scrap_flag == true){
		Regroup_command_linux_scrap="cat ."+filename+".*"+".scrap.htemp > "+scrap_savefile;
		Clean_command_linux_scrap="rm -f ."+filename+".*"+".scrap.htemp";
	}

	cout << endl;
	cout << "Regrouping temp files..."  << endl;
	system(Regroup_command_linux_good.c_str());
	if (scrap_flag){
		system(Regroup_command_linux_scrap.c_str());
	}
	cout << "Cleaning temp files..."  << endl << endl;
	system(Clean_command_linux_good.c_str());
	if (scrap_flag){
		system(Clean_command_linux_scrap.c_str());
	}

	auto t2 = chrono::high_resolution_clock::now();
	int total=chrono::duration_cast<chrono::seconds>(t2-t1).count();

	cout << "Time: " << PrintTime(total) << endl;
	cout << endl << "Program DONE" << endl;


	}catch(TCLAP::ArgException &e)
		{
			cerr << "ERROR: " << e.error() << std::endl;
		}
		return 0;


}

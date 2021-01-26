/*
 * Repartition.cpp
 *
 *  Created on: 13 Jul 2015
 *      Author: pagagne
 */

#include "Repartition.h"
#include "SimpleFastaSequence.h"



void RepartitionCharge(
		std::vector<std::vector<cutlib::SimpleFastaSequence *>>* p_repartition,
		std::vector<cutlib::SimpleFastaSequence>* p_seqpool,int nb_threads) {

	std::vector<cutlib::SimpleFastaSequence *> processor_vec;

	for (int i =0; i < nb_threads; i++)
	{
		p_repartition->push_back(processor_vec);
	}



	signed int ite=0;
	signed int direction=1;

	int start_range=0;
	int end_range=p_seqpool->size()-1;

	std::cout << start_range << std::endl;
	std::cout << end_range << std::endl;


	while (true)
	{
		//cout << ite << endl;

		cutlib::SimpleFastaSequence * mypp;
		mypp = &(*p_seqpool)[start_range];
		(*p_repartition)[ite].push_back(mypp);

		start_range++;
		if (start_range > end_range)
		{
			break;
		}

		if (direction > 0){
			ite++;
		}
		if (direction < 0)
		{
			ite--;
		}
		if ((direction > 0) && (ite == p_repartition->size()))
		{
			direction=-1;
			ite --;
			continue;
		}
		if ((direction < 0) && (ite < 0))
		{
			direction=1;
			ite ++;
			continue;
		}

	}
}

void RepartitionChargeNum(std::vector<int>* p_repartition, int nb_threads,
		int nb_seqs) {
	for (int i =0; i < nb_threads; i++)
		{
			p_repartition->push_back(0);
		}



		signed int ite=0;
		signed int direction=1;

		int start_range=0;

		while (true)
		{
			//cout << ite << endl;

			(*p_repartition)[ite]+=1;

			start_range++;
			if (start_range == nb_seqs)
			{
				break;
			}

			if (direction > 0){
				ite++;
			}
			if (direction < 0)
			{
				ite--;
			}
			if ((direction > 0) && (ite == p_repartition->size()))
			{
				direction=-1;
				ite --;
				continue;
			}
			if ((direction < 0) && (ite < 0))
			{
				direction=1;
				ite ++;
				continue;
			}

		}
}



/*
 * Repartition.h
 *
 *  Created on: 13 Jul 2015
 *      Author: pagagne
 */

#ifndef REPARTITION_H_
#define REPARTITION_H_

#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <map>
#include "SimpleFastaSequence.h"



void RepartitionCharge(
		std::vector<std::vector<cutlib::SimpleFastaSequence *> >* p_repartition,
		std::vector<cutlib::SimpleFastaSequence>* p_seqpool,int nb_threads);


#endif /* REPARTITION_H_ */

/*
 * NewAlignmentFcts.h
 *
 *  Created on: 17 Jul 2015
 *      Author: pagagne
 */

#ifndef NEWALIGNMENTFCTS_H_
#define NEWALIGNMENTFCTS_H_

#include "AlignmentFunctions.h"

void InitMatrix_NEW(std::vector<signed int>* p_matrix, int p_maxlength, signed int p_GapPenalty);

void makeMatrix_NEW(std::vector<signed int>* p_matrix, int p_dimension,  const std::string& p_topseq, const std::string& p_leftseq, signed int p_matchScore, signed int p_mismatchScore, signed int p_gapPenalty);

std::string getAlignCode_NEW(std::vector<signed int>* p_matrix, int p_dimension, const std::string& p_topseq, const std::string& p_leftseq, signed int p_matchScore, signed int p_mismatchScore, signed int p_gapPenalty);

//std::string getAlignCode(std::vector<std::vector<signed int>>* p_matrix);



#endif /* NEWALIGNMENTFCTS_H_ */

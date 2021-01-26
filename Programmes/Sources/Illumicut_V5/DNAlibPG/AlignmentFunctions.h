/*
 * AlignmentFunctions.h
 *
 *  Created on: 30 Jun 2015
 *      Author: pagagne
 */

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#ifndef ALIGNMENTFUNCTIONS_H_
#define ALIGNMENTFUNCTIONS_H_

namespace ALNF {


void makeMatrix(std::vector<std::vector<signed int> >* p_matrix, const std::string& p_topseq, const std::string& p_leftseq, signed int p_matchScore, signed int p_mismatchScore, signed int p_gapPenalty);

int maxScore(int p_int1, int p_int2, int p_int3);

std::string getAlignCode(std::vector<std::vector<signed int> >* p_matrix, const std::string& p_topseq2, const std::string& p_leftseq2, signed int p_matchScore, signed int p_mismatchScore, signed int p_gapPenalty);

std::string TranslateAlignCode(const std::string& p_AlignCode, const std::string& p_topseq, const std::string& p_leftseq);

int CalculateAlignmentScore(const std::string& p_Alignment, signed int p_matchScore, signed int p_mismatchScore, signed int p_gapPenalty);

double CalculateSimilarity(const std::string& p_Alignment);

int GiveScore(const char p_base1, const char p_base2, signed int p_matchScore, signed int p_mismatchPenalty);

//std::string getAlignCode(std::vector<std::vector<signed int>>* p_matrix);

};

#endif /* ALIGNMENTFUNCTIONS_H_ */

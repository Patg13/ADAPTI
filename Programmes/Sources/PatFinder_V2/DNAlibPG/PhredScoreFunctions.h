/*
 * PhredScoreFunctions.h
 *
 *  Created on: 1 Jun 2015
 *      Author: pagagne
 */

#ifndef PHREDSCOREFUNCTIONS_H_
#define PHREDSCOREFUNCTIONS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>

// CF == Conversion Factor ( Phred score + Conversion Factor = Ascii Char Int)
// LL == Lower Limit
// UL == Upper Limit
const int SCORE_ASCII_CF=33;
const int PHRED_SCORE_LL=0;
const int PHRED_SCORE_UL=93;

namespace PHF
{

// Conversion Functions
int convertPhredScore_AsciiInt(int p_score);
const char convertPhredScore_AsciiChar(int p_score);
int convertAsciiInt_PhredScore(int p_intAscii);
int convertAsciiChar_PhredScore(const char p_ascii);
const char convertInt_Ascii(int p_int);
int convertAscii_int(const char p_ascii);

// Count Functions
int countBases(const std::string& p_seq);
int countBase(const std::string& p_seq, char p_base);

// Correction Functions
std::string Correct_LineBreak(const std::string& p_str);
std::string Correct_Seqname(const std::string& p_seqName);

};


#endif /* PHREDSCOREFUNCTIONS_H_ */

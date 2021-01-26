/*
 * DNAfunctions.h
 *
 *  Created on: 15 Jun 2015
 *      Author: pagagne
 */

#ifndef DNAFUNCTIONS_H_
#define DNAFUNCTIONS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>

namespace DNAF
{

// Conversion Functions

std::string ReverseComplementarySeq(const std::string& p_seq);
std::string ReverseSeq(const std::string& p_seq);
std::string ComplementarySeq(const std::string& p_seq);

// Verification Functions

bool IsDNAsequence(const std::string& p_seq);

// Comparison Functions

//bool CheckMatchML(const std::string& p_seq1, const std::string& p_seq2, int mismatch);

bool CheckMatchSL(const std::string& p_seq1, const std::string& p_seq2, int mismatch);

// Browsing Functions

std::string BrowseSeq(const std::string& p_seq, int p_len, int p_offset);

// Trimming Functions

// p_side doit prendre -1 ou 1 (-1 pour retirer le primer et ce qu'il y a AVANT, 1 pour retirer APRÈS) et p_limit est pour limiter la recherche du primer (Index Maximal)
std::string TrimSeq(const std::string& p_seq, const std::string& p_primer, int p_mismatch, int p_limit, int p_side);

}

#endif /* DNAFUNCTIONS_H_ */

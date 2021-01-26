/*
 * NewDNAFunctions.h
 *
 *  Created on: 4 Jan 2016
 *      Author: pagagne
 */

#ifndef NEWDNAFUNCTIONS_H_
#define NEWDNAFUNCTIONS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>
#include "DNAfunctions.h"

int FindTrimPosition(const std::string& p_seq, const std::string& p_primer, int p_mismatch, int p_limit);

std::string TrimSeq_FirstMatch(const std::string& p_seq, const std::string& p_pattern, int p_mismatch, int p_side, bool p_keepPattern);
std::string TrimSeq_BestMatch(const std::string& p_seq, const std::string& p_pattern, int p_mismatch, int p_side);
std::string TrimSeq_HybridMatch(const std::string& p_seq, const std::string& p_pattern, int p_mismatch, int p_side);

#endif /* NEWDNAFUNCTIONS_H_ */

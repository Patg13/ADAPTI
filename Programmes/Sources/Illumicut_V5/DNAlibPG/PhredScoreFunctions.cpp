/*
 * PhredScoreFunctions.cpp
 *
 *  Created on: 1 Jun 2015
 *      Author: pagagne
 */

#include "PhredScoreFunctions.h"


const char PHF::convertInt_Ascii(int p_int_ascii) {

	char asciiChar = p_int_ascii;
	return asciiChar;

}

int PHF::countBases(const std::string& p_seq) {
	std::string Correct_seq=p_seq;
	if (Correct_seq.find("\n") != -1) {
		return (Correct_seq.erase(p_seq.find("\n"), 1)).length();

	}else {
		return p_seq.length();
	}

}

int PHF::countBase(const std::string& p_seq, char p_base) {
	int count=0;
	for (int i = 0; i < p_seq.length(); i++){
		if (p_seq[i] == p_base)
		{
			count += 1;
		}
	}
	return count;
}

std::string PHF::Correct_LineBreak(const std::string& p_str) {
	std::string Correct_str=p_str;
		if (Correct_str.find("\n") != -1) {
			return Correct_str.erase(p_str.find("\n"), 1);

		}else {
			return p_str;
		}

}

int PHF::convertAscii_int(const char p_ascii) {
	int asciiInt = p_ascii;
	return asciiInt;
}

int PHF::convertPhredScore_AsciiInt(int p_score) {
	return p_score + SCORE_ASCII_CF;
}

const char PHF::convertPhredScore_AsciiChar(int p_score) {
	char asciiChar = p_score + SCORE_ASCII_CF;
	return asciiChar;
}

int PHF::convertAsciiInt_PhredScore(int p_intAscii) {
	return p_intAscii - SCORE_ASCII_CF;
}

int PHF::convertAsciiChar_PhredScore(const char p_ascii) {
	int asciiInt = p_ascii;
	return asciiInt - SCORE_ASCII_CF;
}

std::string PHF::Correct_Seqname(const std::string& p_seqName) {
	std::string Correct_name=p_seqName;
	if (p_seqName[0] == '>'){
		return Correct_name.erase(0,1);
	}
	if (p_seqName[0] == '@'){
		return Correct_name.erase(0,1);
	}
	else {
		return p_seqName;
	}

}

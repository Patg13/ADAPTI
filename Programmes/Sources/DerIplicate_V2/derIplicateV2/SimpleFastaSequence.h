/*
 * SimpleFastaSequence.h
 *
 *  Created on: 18 Jun 2015
 *      Author: pagagne
 */

#ifndef SIMPLEFASTASEQUENCE_H_
#define SIMPLEFASTASEQUENCE_H_

#include <string>
#include <vector>
#include <sstream>
#include "ComponentEmptyException.h"
#include "DNAfunctions.h"
#include "PhredScoreFunctions.h"

namespace cutlib {

class SimpleFastaSequence {
public:
	SimpleFastaSequence(const std::string& p_nomSeq, const std::string& p_seq);
	virtual ~SimpleFastaSequence();


	int reqNbBase() const;
	void asgNbBase();


	std::string reqNomSeq() const;
	std::string reqSeq() const;

	int reqLine() const;
	void asgLine(int p_numline);

	bool SeqSortCriterion(const SimpleFastaSequence& s1, const SimpleFastaSequence& s2);
	bool SeqSortCriterionAlpha(const SimpleFastaSequence& s1, const SimpleFastaSequence& s2);
    bool SeqCompCriterion(const SimpleFastaSequence& s1, const SimpleFastaSequence& s2);

    bool operator<(const SimpleFastaSequence& p_sfs);
    bool operator==(const SimpleFastaSequence& p_sfs);
	std::string reqFastaFormat() const;



private:
	std::string m_nomSeq;
	std::string m_seq;
	int m_nbBase;
	int m_line;
};

} /* namespace cutlib */

#endif /* SIMPLEFASTASEQUENCE_H_ */

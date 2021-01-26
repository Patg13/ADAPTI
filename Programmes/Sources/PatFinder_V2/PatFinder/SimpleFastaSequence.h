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
#include "DNAfunctions.h"
#include "PhredScoreFunctions.h"

namespace cutlib {

class SimpleFastaSequence {
public:
	SimpleFastaSequence(const std::string& p_nomSeq, const std::string& p_seq, int p_numline=0);
	virtual ~SimpleFastaSequence();

	int reqNbN() const;
	void asgNbN();

	int reqNbBase() const;
	void asgNbBase();

	int reqLine() const;
	void asgLine(int p_numline);

	std::string reqNomSeq() const;
	std::string reqSeq() const;
	void asgSeq(const std::string& p_newSeq);

	std::string reqFastaFormat() const;
	std::string reqScrapFastaFormat() const;

	std::string reqTrimFastaFormat() const;
	std::string reqTrimScrapFastaFormat() const;

	std::string reqScrapReason() const;
	void asgScrapReason(const std::string& p_reason);

	int reqGoodInd() const;
	void asgGoodInd(int p_good);

	void ClearSeq();

	int reqCutPosFor() const;
	int reqCutPosRevC() const;

	void asgCutPosFor(int p_pos);
	void asgCutPosRevC(int p_pos);




private:
	std::string m_nomSeq;
	std::string m_seq;
	std::string m_Fprimer;
	std::string m_Rprimer;
	int m_nbN;
	int m_nbBase;
	int m_line;
	int m_good;
	std::string m_reason;
	int m_cut_pos_for;
	int m_cut_pos_revc;
};

} /* namespace cutlib */

#endif /* SIMPLEFASTASEQUENCE_H_ */

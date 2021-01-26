/*
 * FastaSequence.h
 *
 *  Created on: 2 Jun 2015
 *      Author: pagagne
 */

#ifndef FASTASEQUENCE_H_
#define FASTASEQUENCE_H_

#include <string>
#include <vector>
#include <sstream>
#include "ComponentEmptyException.h"

namespace PHC {

class FastaSequence {
public:
	FastaSequence(const std::string& p_nomSeq, const std::string& p_seq);

	virtual ~FastaSequence();

	std::string reqNomSeq() const;
	std::string reqSeq() const;
	std::string reqRevCompSeq() const;

	void asgRevCompSeq();

	int reqNbA() const;
	void asgNbA();

	int reqNbBase() const;
	void asgNbBase();

	int reqNbC() const;
	void asgNbC();

	int reqNbG() const;
	void asgNbG();

	int reqNbN() const;
	void asgNbN();

	int reqNbT() const;
	void asgNbT();

	double reqPourGc() const;
	void asgPourGc();

	std::string reqFastaFormat() const;

// Surcharge des opérateurs pour permettre le classement des séquences et leur comparaison
	virtual bool operator==(const FastaSequence& p_sequenceFasta) const;
	virtual bool operator<(const FastaSequence& p_sequenceFasta) const;
	virtual bool operator>(const FastaSequence& p_sequenceFasta) const;


private:
	std::string m_nomSeq;
	std::string m_Seq;
	std::string m_RevCompSeq;
	int m_nbBase;
	int m_nbA;
	int m_nbC;
	int m_nbT;
	int m_nbG;
	int m_nbN;
	double m_pourGC;
};

} /* namespace PHC */

#endif /* FASTASEQUENCE_H_ */

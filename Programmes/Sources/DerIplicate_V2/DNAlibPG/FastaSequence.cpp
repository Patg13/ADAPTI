/*
 * FastaSequence.cpp
 *
 *  Created on: 2 Jun 2015
 *      Author: pagagne
 */

#include "FastaSequence.h"
#include "PhredScoreFunctions.h"
#include "DNAfunctions.h"

namespace PHC {


FastaSequence::FastaSequence(const std::string& p_nomSeq, const std::string& p_seq): m_nomSeq(PHF::Correct_Seqname(p_nomSeq)),m_Seq(PHF::Correct_LineBreak(p_seq)){

	if (m_nomSeq.empty()){
			std::ostringstream error;
			error << "\nERROR: Sequence name cannot be empty\n";
			throw PHException::ComponentEmptyException(error.str());
	}
	if (m_Seq.empty()){
			std::ostringstream error;
			error << "\nERROR: DNA sequence cannot be empty\n";
			throw PHException::ComponentEmptyException(error.str());
	}
	asgNbA();
	asgNbC();
	asgNbT();
	asgNbG();
	asgNbN();
	asgNbBase();
	asgPourGc();
	asgRevCompSeq();


}

FastaSequence::~FastaSequence() {
	// TODO Auto-generated destructor stub
}

int FastaSequence::reqNbA() const {
	return m_nbA;
}

void FastaSequence::asgNbA() {
	m_nbA = PHF::countBase(m_Seq, 'A');
}

int FastaSequence::reqNbBase() const {
	return m_nbBase;
}

void FastaSequence::asgNbBase() {
	m_nbBase = PHF::countBases(m_Seq);
}

int FastaSequence::reqNbC() const {
	return m_nbC;
}

void FastaSequence::asgNbC() {
	m_nbC = PHF::countBase(m_Seq, 'C');
}

int FastaSequence::reqNbG() const {
	return m_nbG;
}

void FastaSequence::asgNbG() {
	m_nbG = PHF::countBase(m_Seq, 'G');
}

int FastaSequence::reqNbN() const {
	return m_nbN;
}

void FastaSequence::asgNbN() {
	m_nbN = PHF::countBase(m_Seq, 'N');
}

int FastaSequence::reqNbT() const {
	return m_nbT;
}

void FastaSequence::asgNbT() {
	m_nbT = PHF::countBase(m_Seq, 'T');
}

std::string FastaSequence::reqNomSeq() const {
	return m_nomSeq;
}

std::string FastaSequence::reqSeq() const {
	return m_Seq;
}

std::string PHC::FastaSequence::reqRevCompSeq() const {
	return m_RevCompSeq;
}

void PHC::FastaSequence::asgRevCompSeq() {
	m_RevCompSeq=DNAF::ReverseComplementarySeq(m_Seq);
}

double FastaSequence::reqPourGc() const {
	return m_pourGC;
}


void FastaSequence::asgPourGc() {
	double nbGC = double(m_nbG) + double(m_nbC);
	m_pourGC = (nbGC / double(m_nbBase)) * 100;
}

std::string FastaSequence::reqFastaFormat() const {
	std::ostringstream os;

	os << ">" << m_nomSeq << "\n";
	os << m_Seq << "\n";
	return os.str();
}

bool FastaSequence::operator==(const FastaSequence& p_sequenceFasta) const {
	return this->reqNbBase() == p_sequenceFasta.reqNbBase();
}

bool FastaSequence::operator<(const FastaSequence& p_sequenceFasta) const {
	return this->reqNbBase() < p_sequenceFasta.reqNbBase();
}

bool FastaSequence::operator>(const FastaSequence& p_sequenceFasta) const {
	return this->reqNbBase() > p_sequenceFasta.reqNbBase();
}




} /* namespace PHC */

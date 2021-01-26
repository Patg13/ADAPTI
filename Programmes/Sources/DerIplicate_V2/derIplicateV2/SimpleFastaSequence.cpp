/*
 * SimpleFastaSequence.cpp
 *
 *  Created on: 18 Jun 2015
 *      Author: pagagne
 */

#include "SimpleFastaSequence.h"
#include "ComponentEmptyException.h"
#include "PhredScoreFunctions.h"

namespace cutlib {

SimpleFastaSequence::SimpleFastaSequence(const std::string& p_nomSeq, const std::string& p_seq): m_nomSeq(PHF::Correct_Seqname(p_nomSeq)),m_seq(PHF::Correct_LineBreak(p_seq)){

	if (m_nomSeq.empty()){
				std::ostringstream error;
				error << "\nERROR: Sequence name cannot be empty\n";
				throw PHException::ComponentEmptyException(error.str());
		}
		if (m_seq.empty()){
				std::ostringstream error;
				error << "\nERROR: DNA sequence cannot be empty\n";
				throw PHException::ComponentEmptyException(error.str());
		}
		asgNbBase();
}

SimpleFastaSequence::~SimpleFastaSequence() {

}





std::string SimpleFastaSequence::reqNomSeq() const {
	return m_nomSeq;
}

std::string SimpleFastaSequence::reqSeq() const {
	return m_seq;
}


int cutlib::SimpleFastaSequence::reqNbBase() const {
	return m_nbBase;
}

void cutlib::SimpleFastaSequence::asgNbBase() {
	m_nbBase = PHF::countBases(m_seq);
}

bool cutlib::SimpleFastaSequence::SeqSortCriterion(
		const SimpleFastaSequence& s1, const SimpleFastaSequence& s2)
{
	return s1.reqNbBase() < s2.reqNbBase() || (s1.reqNbBase()==s2.reqNbBase() && s1.reqNbBase() < s2.reqNbBase());
}

bool cutlib::SimpleFastaSequence::SeqSortCriterionAlpha(
		const SimpleFastaSequence& s1, const SimpleFastaSequence& s2)
{
	return s1.reqSeq() < s2.reqSeq() || (s1.reqSeq()==s2.reqSeq() && s1.reqSeq() < s2.reqSeq());
}

bool SimpleFastaSequence::SeqCompCriterion(const SimpleFastaSequence& s1,
		const SimpleFastaSequence& s2) {
	return (s1.reqSeq() == s2.reqSeq());
}

bool SimpleFastaSequence::operator ==(const SimpleFastaSequence& p_sfs) {
	return this->reqSeq() == p_sfs.reqSeq();
}

std::string SimpleFastaSequence::reqFastaFormat() const {
	std::ostringstream os;

	os << ">" << m_nomSeq << "\n";
	os << m_seq << "\n";
	return os.str();
}

int cutlib::SimpleFastaSequence::reqLine() const {
	return m_line;
}

void cutlib::SimpleFastaSequence::asgLine(int p_numline) {
	m_line = p_numline;
}

} /* namespace cutlib */

bool cutlib::SimpleFastaSequence::operator <(const SimpleFastaSequence& p_sfs) {
	return this->reqSeq() < p_sfs.reqSeq();
}

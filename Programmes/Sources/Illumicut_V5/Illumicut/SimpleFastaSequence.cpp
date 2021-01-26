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

SimpleFastaSequence::SimpleFastaSequence(const std::string& p_nomSeq, const std::string& p_seq, int p_numline, const std::string& pqual): m_nomSeq(PHF::Correct_Seqname(p_nomSeq)),m_seq(PHF::Correct_LineBreak(p_seq)),m_qual(PHF::Correct_LineBreak(pqual)) {

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
		if (! pqual.empty()){
			asgQualSeq(pqual);
			asgSeqType(2);
		}else{
			asgQualSeq(pqual);
			asgSeqType(1);
		}
		asgNbN();
		asgNbBase();
		asgGoodInd(0);
		asgScrapReason("");


}

SimpleFastaSequence::~SimpleFastaSequence() {

}

int SimpleFastaSequence::reqNbN() const {
	return m_nbN;
}

void SimpleFastaSequence::asgNbN() {
	m_nbN = PHF::countBase(m_seq, 'N');
}

std::string SimpleFastaSequence::reqNomSeq() const {
	return m_nomSeq;
}

std::string SimpleFastaSequence::reqSeq() const {
	return m_seq;
}

std::string SimpleFastaSequence::reqFastaFormat() const {
	std::ostringstream os;

	os << ">" << m_nomSeq << "\n";
	os << m_seq << "\n";
	return os.str();
}

void SimpleFastaSequence::ClearSeq() {
	m_seq.clear();
}

} /* namespace cutlib */

int cutlib::SimpleFastaSequence::reqNbBase() const {
	return m_nbBase;
}

void cutlib::SimpleFastaSequence::asgNbBase() {
	m_nbBase = PHF::countBases(m_seq);
}

int cutlib::SimpleFastaSequence::reqLine() const {
	return m_line;
}

void cutlib::SimpleFastaSequence::asgLine(int p_numline) {
	m_line = p_numline;
}

std::string cutlib::SimpleFastaSequence::reqScrapReason() const {
	return m_reason;
}

void cutlib::SimpleFastaSequence::asgScrapReason(const std::string& p_reason) {
	m_reason = p_reason;
}

int cutlib::SimpleFastaSequence::reqGoodInd() const {
	return m_good;
}


void cutlib::SimpleFastaSequence::asgGoodInd(int p_good) {
	m_good=p_good;
}

void cutlib::SimpleFastaSequence::asgSeq(const std::string& p_newSeq) {
	m_seq.replace(m_seq.begin(),m_seq.end(),p_newSeq);
}

std::string cutlib::SimpleFastaSequence::reqScrapFastaFormat() const {
	std::ostringstream os;
	os << ">" << m_nomSeq << "|" << m_reason << "\n";
	os << m_seq << "\n";
	return os.str();
}

std::string cutlib::SimpleFastaSequence::reqTrimFastaFormat() const {
	std::ostringstream os;
	os << ">" << m_nomSeq << "\n";
	os << m_seq.substr(m_cut_pos_for,m_nbBase-m_cut_pos_for-m_cut_pos_revc) << "\n";
	return os.str();
}

std::string cutlib::SimpleFastaSequence::reqTrimScrapFastaFormat() const {
	std::ostringstream os;
	os << ">" << m_nomSeq << "|" << m_reason << "\n";
	os << m_seq.substr(m_cut_pos_for,m_nbBase-m_cut_pos_for-m_cut_pos_revc) << "\n";
	return os.str();
}

std::string cutlib::SimpleFastaSequence::reqTrimFastqFormat() const {
	std::ostringstream os;
	os << "@" << m_nomSeq << "\n";
	os << m_seq.substr(m_cut_pos_for,m_nbBase-m_cut_pos_for-m_cut_pos_revc) << "\n";
	os << "+\n";
	os << m_qual.substr(m_cut_pos_for,m_nbBase-m_cut_pos_for-m_cut_pos_revc) << "\n";
	return os.str();
}

std::string cutlib::SimpleFastaSequence::reqTrimScrapFastqFormat() const {
	std::ostringstream os;
	os << "@" << m_nomSeq << "|" << m_reason << "\n";
	os << m_seq.substr(m_cut_pos_for,m_nbBase-m_cut_pos_for-m_cut_pos_revc) << "\n";
	os << "+\n";
	os << m_qual.substr(m_cut_pos_for,m_nbBase-m_cut_pos_for-m_cut_pos_revc) << "\n";
	return os.str();
}

std::string cutlib::SimpleFastaSequence::reqScrapFastqFormat() const {
	std::ostringstream os;
	os << "@" << m_nomSeq << "|" << m_reason << "\n";
	os << m_seq << "\n";
	os << "+\n";
	os << m_qual << "\n";
	return os.str();
}

int cutlib::SimpleFastaSequence::reqCutPosFor() const {
	return m_cut_pos_for;
}

int cutlib::SimpleFastaSequence::reqCutPosRevC() const {
	return m_cut_pos_revc;
}

void cutlib::SimpleFastaSequence::asgCutPosFor(int p_pos) {
	m_cut_pos_for=p_pos;
}

void cutlib::SimpleFastaSequence::asgCutPosRevC(int p_pos) {
	m_cut_pos_revc=p_pos;
}

int cutlib::SimpleFastaSequence::reqSeqType() const {
	return m_seqType;
}

void cutlib::SimpleFastaSequence::asgSeqType(int p_seqType) {
	m_seqType=p_seqType;
}

std::string cutlib::SimpleFastaSequence::reqQualSeq() const {
	return m_qual;
}

void cutlib::SimpleFastaSequence::asgQualSeq(const std::string& p_newQual) {
	m_qual=p_newQual;
}

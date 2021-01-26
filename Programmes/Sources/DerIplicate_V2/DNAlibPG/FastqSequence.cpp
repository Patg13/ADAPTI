/*
 * FastqSequence.cpp
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#include "FastqSequence.h"

namespace PHC {

FastqSequence::FastqSequence(const std::string& p_nomSeq, const std::string& p_seq, const std::string& p_qualitySeq): FastaSequence(p_nomSeq, p_seq), m_qualitySeq(PHF::Correct_LineBreak(p_qualitySeq)) {

	if (m_qualitySeq.empty()){
			std::ostringstream error;
			error << "\nERROR, Quality sequence cannot be empty\n";
			throw PHException::ComponentEmptyException(error.str());
		}
	if (this->reqNbBase() != m_qualitySeq.length()){
		std::ostringstream error;
		error << "\nERROR, DNA sequence " << this->reqNomSeq() << " doesn't match Quality Sequence\n";
		error << "( " << this->reqNbBase() << " pb Not Equal " << m_qualitySeq.length() << "pb )\n";
		throw PHException::QualityNotEqualSequenceException(error.str());
	}
	asgIntScoreVector();
	asgAvgQuality();
	asgHighestQualityScoreIndex();
	asgLowestQualityScoreIndex();
	asgWrongSequenceProbability();

}

FastqSequence::~FastqSequence() {
}

double FastqSequence::reqAvgQuality() const {
	return m_AvgQuality;
}

void FastqSequence::asgAvgQuality() {
	double Total=std::accumulate(m_vIntScore.begin(), m_vIntScore.end(), 0);
	m_AvgQuality = (Total / double(this->reqNbBase()));
}

int FastqSequence::reqHighestQualityScoreIndex() const {
	return m_HighestQualityScoreIndex;
}

void FastqSequence::asgHighestQualityScoreIndex() {
	std::vector<int>::iterator result;
	result = std::max_element(m_vIntScore.begin(), m_vIntScore.end());
	m_HighestQualityScoreIndex = std::distance(m_vIntScore.begin(), result);
}

int FastqSequence::reqLowestQualityScoreIndex() const {
	return m_LowestQualityScoreIndex;
}

void FastqSequence::asgLowestQualityScoreIndex() {
	std::vector<int>::iterator result;
	result = std::min_element(m_vIntScore.begin(), m_vIntScore.end());
	m_LowestQualityScoreIndex = std::distance(m_vIntScore.begin(), result);
}

const std::string& FastqSequence::reqQualitySeq() const {
	return m_qualitySeq;
}

double FastqSequence::reqWrongSequenceProbability() const {
	return m_WrongSequenceProbability;
}

void FastqSequence::asgWrongSequenceProbability() {
	double part = (m_AvgQuality * -1)/10;
	m_WrongSequenceProbability = pow(10,part);

}

double FastqSequence::reqGoodSequenceProbability() const {
	return 1 - m_WrongSequenceProbability;
}

int FastqSequence::reqHighestQualityScore() const {
	return m_vIntScore[m_HighestQualityScoreIndex];
}

int FastqSequence::reqLowestQualityScore() const {
	return m_vIntScore[m_LowestQualityScoreIndex];
}

void FastqSequence::asgIntScoreVector() {

	for (int i = 0; i < this->reqNbBase(); i++)
	{
		m_vIntScore.push_back((PHF::convertAscii_int(m_qualitySeq[i]))-SCORE_ASCII_CF);
	}
}

std::vector<int> FastqSequence::reqIntScoreVector() const {
	return m_vIntScore;
}

double FastqSequence::reqScoreWrongProbability(int p_score) {
	if (p_score < PHRED_SCORE_LL || p_score > PHRED_SCORE_UL)
	{
		return -1;
	}
	double part = double((double(p_score) * -1)/10);
	return double(pow(10,part));
}

double FastqSequence::reqScoreGoodProbability(int p_score) {
	if (p_score < PHRED_SCORE_LL || p_score > PHRED_SCORE_UL)
		{
			return -1;
		}
	return double(1 - (reqScoreWrongProbability(p_score)));
}

int FastqSequence::reqBaseScore(int p_index) {
	return m_vIntScore[p_index];
}

} /* namespace PHC */

/*
 * FastqSequence.h
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#ifndef FASTQSEQUENCE_H_
#define FASTQSEQUENCE_H_

#include "FastaSequence.h"
#include "PhredScoreFunctions.h"
#include <vector>
#include <numeric>
#include "ComponentEmptyException.h"
#include "QualityNotEqualSequenceException.h"
#include <sstream>
#include <algorithm>
#include <cmath>


namespace PHC {


class FastqSequence: public FastaSequence {
public:
	FastqSequence(const std::string& p_nomSeq, const std::string& p_seq, const std::string& p_qualitySeq);
	
	virtual ~FastqSequence();
	double reqAvgQuality() const;
	void asgAvgQuality();
	int reqHighestQualityScoreIndex() const;
	void asgHighestQualityScoreIndex();
	int reqLowestQualityScoreIndex() const;
	void asgLowestQualityScoreIndex();
	const std::string& reqQualitySeq() const;
	double reqWrongSequenceProbability() const;
	void asgWrongSequenceProbability();
	void asgIntScoreVector();
	std::vector<int> reqIntScoreVector() const;

	double reqGoodSequenceProbability() const;
	int reqHighestQualityScore() const;
	int reqLowestQualityScore() const;

	double reqScoreWrongProbability(int p_score);
	double reqScoreGoodProbability(int p_score);
	int reqBaseScore(int p_index);

private:
	std::string m_qualitySeq;
	double m_AvgQuality;
	int m_LowestQualityScoreIndex;
	int m_HighestQualityScoreIndex;
	double m_WrongSequenceProbability;
	std::vector<int> m_vIntScore;

};

} /* namespace PHC */

#endif /* FASTQSEQUENCE_H_ */

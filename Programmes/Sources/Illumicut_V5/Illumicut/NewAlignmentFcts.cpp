

#include "NewAlignmentFcts.h"

/*
 * NewAlignmentFcts.cpp
 *
 *  Created on: 17 Jul 2015
 *      Author: pagagne
 */


void InitMatrix_NEW(std::vector<signed int>* p_matrix,
		int p_maxlength_b, signed int p_GapPenalty) {

		int dimension=p_maxlength_b;
		std::vector<signed int> tableau(dimension*dimension);
		int poos=0;
				for (int j=0; j < dimension; ++j)
				{
					for (int i = 0; i < dimension; ++i)
					{
						if (j==0)
						{
							tableau[j * dimension + i]=i*p_GapPenalty;
						}else{
							if (i == 0)
							{
								tableau[j * dimension + i]=j*p_GapPenalty;
							}else{
								tableau[j * dimension + i]=poos;
								poos++;
							}

						}
					}
				}
				tableau.swap((*p_matrix));

}

void makeMatrix_NEW(std::vector<signed int>* p_matrix, int p_dimension,
		const std::string& p_topseq, const std::string& p_leftseq,
		signed int p_matchScore, signed int p_mismatchScore,
		signed int p_gapPenalty) {

	int toplength=p_topseq.length();
	int leftlength=p_leftseq.length();
	for (int j=1; j <= leftlength; j++)
	{
		for (int i=1; i <= toplength; i++)
		{
			(*p_matrix)[j * p_dimension + i]=ALNF::maxScore((*p_matrix)[j * p_dimension + (i-1)]+p_gapPenalty, (*p_matrix)[(j-1) * p_dimension + i]+p_gapPenalty, (*p_matrix)[(j-1) * p_dimension + (i-1)]+ALNF::GiveScore(p_topseq[i-1] , p_leftseq[j-1] , p_matchScore, p_mismatchScore));
		}
	}
}

std::string getAlignCode_NEW(std::vector<signed int>* p_matrix, int p_dimension,
		const std::string& p_topseq, const std::string& p_leftseq,
		signed int p_matchScore, signed int p_mismatchScore,
		signed int p_gapPenalty) {

	std::string p_topseqC="0"+p_topseq; // Add 0 character to align Sequence with matrix
	std::string p_leftseqC="0"+p_leftseq; // Add 0 character to align Sequence with matrix
	int j = p_leftseqC.length()-1; // p_matrix.size()-1
	int i = p_topseqC.length()-1; //(*p_matrix)[j].size()-1;
	std::string AlignCode="";
	//std::cout << (*p_matrix)[j][i] << std::endl;
	while (true)
	{
		if ((j == 0) && (i == 0))
		{
			break;
		}else
		{

			if ((j > 0) && ((*p_matrix)[(j-1) * p_dimension + i]+p_gapPenalty == (*p_matrix)[j * p_dimension + i]))
			{
				j=j-1;
				AlignCode="U"+AlignCode;
				continue;
			}
			if ((j > 0) && (i > 0) && ((*p_matrix)[(j-1) * p_dimension + (i-1)]+ALNF::GiveScore(p_topseqC[i], p_leftseqC[j],p_matchScore, p_mismatchScore) == (*p_matrix)[j * p_dimension + i]))
			{
				j=j-1;
				i=i-1;
				AlignCode="D"+AlignCode;
				continue;
			}
			if ((i > 0) && ((*p_matrix)[j * p_dimension + (i-1)]+p_gapPenalty == (*p_matrix)[j * p_dimension + i]))
			{
				i=i-1;
				AlignCode="L"+AlignCode;
				continue;
			}

		}

	}

	return AlignCode;



}



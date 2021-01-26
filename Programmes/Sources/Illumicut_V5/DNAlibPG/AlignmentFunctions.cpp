/*
 * AlignmentFunctions.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: pagagne
 */


#include "AlignmentFunctions.h"





int ALNF::maxScore(int p_int1, int p_int2, int p_int3) {


	if ((p_int1 >= p_int2) && (p_int1 >= p_int3))
	{
		return p_int1;
	}
	if ((p_int2 >= p_int1) && (p_int2 >= p_int3))
		{
			return p_int2;
		}
	if ((p_int3 >= p_int1) && (p_int3 >= p_int2))
		{
			return p_int3;
		}
	//return std::max(std::max(p_int1, p_int2),p_int3);

}

void ALNF::makeMatrix(std::vector<std::vector<signed int> >* p_matrix,
		const std::string& p_topseq, const std::string& p_leftseq,
		signed int p_matchScore, signed int p_mismatchScore,
		signed int p_gapPenalty) {


	    std::vector<signed int> p_ligne;
		int toplength=p_topseq.length();
		int leftlength=p_leftseq.length();

		// Matrix initialisation
		for (int element=0; element <= p_topseq.length(); element++)
		{
			p_ligne.push_back(0);
		}
		for (int colonne=0; colonne <= p_leftseq.length(); colonne++)
		{

			p_matrix->push_back(p_ligne);
		}

		// Prepare Matrix Calculation

		for (int i=0; i <= toplength; i++)
		{
			(*p_matrix)[0][i]=(i*p_gapPenalty);
		}
		for (int j=0; j <= leftlength; j++)
		{
				(*p_matrix)[j][0]=(j*p_gapPenalty);
		}

		// Matrix Resolution

		for (int j=1; j <= leftlength; j++)
		{
			for (int i=1; i <= toplength; i++)
			{
				(*p_matrix)[j][i]=maxScore((*p_matrix)[j][i-1]+p_gapPenalty, (*p_matrix)[j-1][i]+p_gapPenalty, (*p_matrix)[j-1][i-1]+GiveScore(p_topseq[i-1] , p_leftseq[j-1] , p_matchScore, p_mismatchScore));

			}
		}
}

std::string ALNF::TranslateAlignCode(const std::string& p_AlignCode,
		const std::string& p_topseq, const std::string& p_leftseq) {

	std::string topseqResult="";
	std::string leftseqResult="";
	std::string concat;
	int top=0;
	int left=0;
	int ori=0;

	while (ori < p_AlignCode.length())
	{
		if (p_AlignCode[ori] == 'D')
		{
			topseqResult+=p_topseq[top];
			leftseqResult+=p_leftseq[left];
			top++;
			left++;
			ori++;
			continue;
		}
		if (p_AlignCode[ori]== 'L')
		{
			topseqResult+=p_topseq[top];
			leftseqResult+="-";
			top++;
			ori++;
			continue;
		}
		if (p_AlignCode[ori]== 'U')
		{
			topseqResult+="-";
			leftseqResult+=p_leftseq[left];
			left++;
			ori++;
			continue;
		}



	}
	concat=topseqResult+" "+leftseqResult;
	return concat;

}

double ALNF::CalculateSimilarity(const std::string& p_Alignment) {
	std::string topseq=p_Alignment.substr(0, p_Alignment.find(' '));
	std::string leftseq=p_Alignment.substr(p_Alignment.find(' ')+1);

	double match=0;

	if (topseq.length() != leftseq.length())
	{
		return 0.0;
	}

	for (int i=0; i < topseq.length(); i++)
	{
		if (topseq[i] == leftseq[i])
		{
			match+=1.0;
		}
	}

	return ((match / double(topseq.length())));


}

int ALNF::CalculateAlignmentScore(const std::string& p_Alignment,
		signed int p_matchScore, signed int p_mismatchScore,
		signed int p_gapPenalty)
{
	std::string topseq=p_Alignment.substr(0, p_Alignment.find(' '));
	std::string leftseq=p_Alignment.substr(p_Alignment.find(' ')+1);
	signed int score=0;

	if (topseq.length() != leftseq.length())
		{
			return 0.0;
		}

		for (int i=0; i < topseq.length(); i++)
		{
			if (topseq[i] == leftseq[i])
			{
				score+=p_matchScore;
				continue;
			}
			if (topseq[i] == '-' || leftseq[i] == '-')
			{
				score+=p_gapPenalty;
				continue;
			}
			if (topseq[i] != leftseq[i])
			{
				score+=p_mismatchScore;
				continue;
			}

		}
		return score;


}

std::string ALNF::getAlignCode(
		std::vector<std::vector<signed int> >* p_matrix,
		const std::string& p_topseq, const std::string& p_leftseq,
		signed int p_matchScore, signed int p_mismatchScore,
		signed int p_gapPenalty)

{
	std::string p_topseqC="0"+p_topseq; // Add 0 character to align Sequence with matrix
	std::string p_leftseqC="0"+p_leftseq; // Add 0 character to align Sequence with matrix
	int j = p_matrix->size()-1;
		int i = (*p_matrix)[j].size()-1;
		std::string AlignCode="";

		while (true)
		{
			if ((j == 0) && (i == 0))
			{
				break;
			}else
			{

				if ((j > 0) && ((*p_matrix)[j-1][i]+p_gapPenalty == (*p_matrix)[j][i]))
				{
					j=j-1;
					AlignCode="U"+AlignCode;
					continue;
				}
				if ((j > 0) && (i > 0) && ((*p_matrix)[j-1][i-1]+GiveScore(p_topseqC[i], p_leftseqC[j],p_matchScore, p_mismatchScore) == (*p_matrix)[j][i]))
				{
					j=j-1;
					i=i-1;
					AlignCode="D"+AlignCode;
					continue;
				}
				if ((i > 0) && ((*p_matrix)[j][i-1]+p_gapPenalty == (*p_matrix)[j][i]))
				{
					i=i-1;
					AlignCode="L"+AlignCode;
					continue;
				}

			}

		}

		return AlignCode;


}

int ALNF::GiveScore(const char p_base1, const char p_base2,
		signed int p_matchScore, signed int p_mismatchPenalty) {

	if (p_base1 == p_base2)
	{
		return p_matchScore;
	}else
	{
		return p_mismatchPenalty;
	}
}

// Old AlignCode Algorithm (using max score recursively) ERROR, Can possibily return NON optimum Alignment
/*
std::string ALNF::getAlignCode(std::vector<std::vector<signed int>>* p_matrix)
{
	int j = p_matrix->size()-1;
	int i = (*p_matrix)[j].size()-1;
	std::string AlignCode="";

	while (true)
	{
		if ((j == 0) && (i == 0))
		{
			break;
		}else
		{

			if (((*p_matrix)[j-1][i] >= (*p_matrix)[j-1][i-1]) && ((*p_matrix)[j-1][i] >= (*p_matrix)[j][i-1]))
			{
				j=j-1;
				AlignCode="U"+AlignCode;
				continue;
			}
			if (((*p_matrix)[j-1][i-1] >= (*p_matrix)[j-1][i]) && ((*p_matrix)[j-1][i-1] >= (*p_matrix)[j][i-1]))
			{
				j=j-1;
				i=i-1;
				AlignCode="D"+AlignCode;
				continue;
			}
			if (((*p_matrix)[j][i-1] >= (*p_matrix)[j-1][i-1]) && ((*p_matrix)[j][i-1] >= (*p_matrix)[j-1][i]))
			{
				i=i-1;
				AlignCode="L"+AlignCode;
				continue;
			}

		}

	}

	return AlignCode;
}
*/

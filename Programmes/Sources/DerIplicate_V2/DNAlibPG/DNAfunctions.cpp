/*
 * DNAfunctions.cpp
 *
 *  Created on: 15 Jun 2015
 *      Author: pagagne
 */


#include "DNAfunctions.h"



std::string DNAF::ReverseComplementarySeq(const std::string& p_seq) {

	return ComplementarySeq(ReverseSeq(p_seq));
}

std::string DNAF::ReverseSeq(const std::string& p_seq) {

	std::string inverted;
	for (unsigned int i =0; i < p_seq.length(); i++)
	{
		inverted=p_seq[i]+inverted;
	}
	return inverted;
}

std::string DNAF::ComplementarySeq(const std::string& p_seq) {

	std::string complement;
	for (unsigned int i = 0; i < p_seq.length(); i++)
	{
		switch (p_seq[i])
		{
		case 'A' : complement+='T'; break;
		case 'T' : complement+='A'; break;
		case 'C' : complement+='G'; break;
		case 'G' : complement+='C'; break;
		case 'N' : complement+='N'; break;
		}
	}
	if (complement.length() != p_seq.length())
	{
		return "";
	}
	return complement;
}


bool DNAF::IsDNAsequence(const std::string& p_seq)
{
	for (unsigned int i = 0; i < p_seq.length(); i++)
	{
		if ((p_seq[i] != 'A') && (p_seq[i] != 'C') && (p_seq[i] != 'T') && (p_seq[i] != 'G') && (p_seq[i] != 'N'))
		{
			return false;
		}
	}
	return true;
}

/*
bool DNAF::CheckMatchML(const std::string& p_seq1, const std::string& p_seq2, int p_mismatch)
{
	int limit;
	int score=0;

	std::string shorterSeq;
	if (p_seq1.empty() || p_seq2.empty())
	{
		return false;
	}

	if (p_seq1.length() >= p_seq2.length()){
		limit=p_seq1.length();
		shorterSeq=p_seq2;

	}else
	{
		limit= p_seq2.length();
		shorterSeq=p_seq1;
	}

	for (int i =0; i < limit; i++){



	}


}
*/
bool DNAF::CheckMatchSL(const std::string& p_seq1, const std::string& p_seq2,
		int p_mismatch) {

	int limit;
	int score=0;
	int Pass_Score;

	if (p_seq1.empty() || p_seq2.empty())
		{
			return false;
		}
	if (p_seq1.length() != p_seq2.length())
	{
		return false;
	}
	limit=p_seq1.length();

	Pass_Score=p_seq1.length()-p_mismatch;

	for (unsigned int i=0; i < limit; i++)
	{
		if (p_seq1[i] == 'Y')
		{
			if (p_seq2[i] == 'C' || p_seq2[i] == 'T')
			{
					score++;
					continue;
			}else
			{
				continue;
			}
		}
		if (p_seq2[i] == 'Y')
				{
					if (p_seq1[i] == 'C' || p_seq1[i] == 'T')
					{
							score++;
							continue;
					}else
					{
						continue;
					}
				}
		if (p_seq1[i] == p_seq2[i])
		{
			score++;
			continue;
		}else
		{
			continue;
		}
	}
	if (score < Pass_Score)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::string DNAF::BrowseSeq(const std::string& p_seq, int p_len, int p_offset)
{
	if (p_len > (p_seq.length()-p_offset) )
	{
		return "";
	}
	return p_seq.substr(p_offset, p_len);
}

std::string DNAF::TrimSeq(const std::string& p_seq, const std::string& p_primer,
		int p_mismatch, int p_limit, int p_side) {

	if ( p_side != -1 && p_side != 1 )
	{
		return "-1";
	}

	int len_primer=p_primer.length();
	if (p_limit < p_seq.length())
	{
		p_limit=p_seq.length();
	}

	if (len_primer >= p_seq.length())
	{
		return "";
	}

	for (int i= 0; i < p_limit; i++)
	{
		std::string subseq=BrowseSeq(p_seq, len_primer, i);
		if (subseq == ""){
			break;
		}
		if (CheckMatchSL(subseq, p_primer, p_mismatch))
		{
			int cut_pos=i+len_primer;
			if (p_side == -1)
			{
				return p_seq.substr(cut_pos);
			}
			if (p_side == 1)
			{
				return p_seq.substr(0,cut_pos-1);
			}
		}
	}
	return "";

}

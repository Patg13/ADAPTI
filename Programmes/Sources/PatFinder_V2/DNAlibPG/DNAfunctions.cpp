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
		if (p_seq1[i] == 'N' || p_seq2[i] == 'N')
		{
			continue;
		}
		if (p_seq1[i] == p_seq2[i])
		{
			score++;
			continue;
		}else
		{
			if (NormalBaseDetection(p_seq1[i],p_seq2[i]) == false)
			{
				if(DegenerateBasesComp(p_seq1[i],p_seq2[i]) == true)
				{
					score++;
					continue;
				}else
				{
					continue;
				}
			}else
			{
				continue;
			}
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
	if (p_limit > p_seq.length())
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

bool DNAF::DegenerateBasesComp(const char p_upBase, const char p_downBase) {

	if (p_upBase == 'Y')
	{
		if (p_downBase == 'C' || p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'Y')
	{
		if (p_upBase == 'C' || p_upBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'U')
	{
		if (p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'U')
	{
		if (p_upBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'K')
	{
		if (p_downBase == 'G' || p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'K')
	{
		if (p_upBase == 'G' || p_upBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'M')
	{
		if (p_downBase == 'A' || p_downBase == 'C')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'M')
	{
		if (p_upBase == 'A' || p_upBase == 'C')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'R')
	{
		if (p_downBase == 'A' || p_downBase == 'G')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'R')
	{
		if (p_upBase == 'A' || p_upBase == 'G')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'S')
	{
		if (p_downBase == 'C' || p_downBase == 'G')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'S')
	{
		if (p_upBase == 'C' || p_upBase == 'G')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'W')
	{
		if (p_downBase == 'A' || p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'W')
	{
		if (p_upBase == 'A' || p_upBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'B')
	{
		if (p_downBase == 'C' || p_downBase == 'G' || p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'B')
	{
		if (p_upBase == 'C' || p_upBase == 'G' || p_upBase == 'T' )
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'V')
	{
		if (p_downBase == 'A' || p_downBase == 'C' || p_downBase == 'G')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'V')
	{
		if (p_upBase == 'A' || p_upBase == 'C' || p_upBase == 'G')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'H')
	{
		if (p_downBase == 'A' || p_downBase == 'C' || p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'H')
	{
		if (p_upBase == 'A' || p_upBase == 'C' || p_upBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'D')
	{
		if (p_downBase == 'A' || p_downBase == 'G' || p_downBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'D')
	{
		if (p_upBase == 'A' || p_upBase == 'G' || p_upBase == 'T')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_upBase == 'X' || p_upBase == 'I')
	{
		if (p_downBase == 'G' || p_downBase == 'A' || p_downBase == 'T' || p_downBase == 'C')
		{
			return true;
		}else
		{
			return false;
		}
	}
	if (p_downBase == 'X' || p_downBase == 'I')
	{
		if (p_upBase == 'G' || p_upBase == 'A' || p_upBase == 'T' || p_upBase == 'C')
		{
			return true;
		}else
		{
			return false;
		}
	}
	return false;
}

bool DNAF::NormalBaseDetection(const char p_upBase, const char p_downBase) {
	if ((p_upBase == 'A' || p_upBase == 'C' || p_upBase == 'T' || p_upBase == 'G')&&((p_downBase == 'A' || p_downBase == 'C' || p_downBase == 'T' || p_downBase == 'G'))){
		return true;
	}else
	{
		return false;
	}
}

bool DNAF::isDegeneratedPrimer(const std::string& p_primer) {

	for (int i=0; i < p_primer.length(); i++)
	{
		if ((p_primer[i] != 'A' && p_primer[i] != 'C' && p_primer[i] != 'T' && p_primer[i] != 'G'))
		{
			return true;
		}
	}
	return false;

}

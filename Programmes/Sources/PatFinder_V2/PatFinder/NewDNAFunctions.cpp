/*
 * NewDNAFunctions.cpp
 *
 *  Created on: 4 Jan 2016
 *      Author: pagagne
 */


#include "NewDNAFunctions.h"

int FindTrimPosition(const std::string& p_seq,
		const std::string& p_primer, int p_mismatch, int p_limit) {

		int len_primer=p_primer.length();
		if (p_limit > p_seq.length())
		{
			p_limit=p_seq.length();
		}

		if (len_primer >= p_seq.length())
		{
			return 0;
		}

		for (int i= 0; i < p_limit; i++)
		{
			std::string subseq=DNAF::BrowseSeq(p_seq, len_primer, i);
			if (subseq == ""){
				break;
			}
			if (DNAF::CheckMatchSL(subseq, p_primer, p_mismatch))
			{
				int cut_pos=i+len_primer;
					return cut_pos;
				}
			}
		return 0;

}

std::string TrimSeq_FirstMatch(const std::string& p_seq,
		const std::string& p_pattern, int p_mismatch, int p_side, bool p_KeepPattern) {
	if ( p_side != -1 && p_side != 1 && p_side !=0 )
		{
			return "-1";
		}

		int len_pattern=p_pattern.length();
		int p_limit=p_seq.length();

		if (len_pattern >= p_seq.length())
		{
			return "";
		}

		for (int i= 0; i < p_limit; i++)
		{
			std::string subseq=DNAF::BrowseSeq(p_seq, len_pattern, i);
			if (subseq == ""){
				break;
			}
			if (DNAF::CheckMatchSL(subseq, p_pattern, p_mismatch))
			{
				int cut_pos=i+len_pattern;
				if (p_side == -1)
				{
					if (p_KeepPattern == true)
					{
						return p_seq.substr(cut_pos-len_pattern);
					}else{
						return p_seq.substr(cut_pos);
					}
				}
				if (p_side == 1)
				{
					if (p_KeepPattern == true){
						return p_seq.substr(0,cut_pos);
					}else{
						return p_seq.substr(0,cut_pos-len_pattern);
					}
				}
				if (p_side == 0)
				{
					return p_seq;
				}
			}
		}
		return "";
}

std::string TrimSeq_BestMatch(const std::string& p_seq,
		const std::string& p_pattern, int p_mismatch, int p_side) {
}

std::string TrimSeq_HybridMatch(const std::string& p_seq,
		const std::string& p_pattern, int p_mismatch, int p_side) {
}

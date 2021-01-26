/*
 * QualityNotEqualSequenceException.cpp
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#include "QualityNotEqualSequenceException.h"

namespace PHException {

QualityNotEqualSequenceException::QualityNotEqualSequenceException(const std::string& p_raison): FastqException(p_raison) {
}


} /* namespace PHException */

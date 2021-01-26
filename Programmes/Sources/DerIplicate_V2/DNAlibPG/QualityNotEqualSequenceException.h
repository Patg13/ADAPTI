/*
 * QualityNotEqualSequenceException.h
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#ifndef QUALITYNOTEQUALSEQUENCEEXCEPTION_H_
#define QUALITYNOTEQUALSEQUENCEEXCEPTION_H_

#include "FastqException.h"
#include <string>

namespace PHException {

class QualityNotEqualSequenceException: public FastqException {
public:
	QualityNotEqualSequenceException(const std::string& p_raison);
};

} /* namespace PHException */

#endif /* QUALITYNOTEQUALSEQUENCEEXCEPTION_H_ */

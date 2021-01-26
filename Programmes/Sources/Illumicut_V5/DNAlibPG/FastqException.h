/*
 * FastqException.h
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#ifndef FASTQEXCEPTION_H_
#define FASTQEXCEPTION_H_

#include <string>
#include <stdexcept>

namespace PHException {

class FastqException: public std::runtime_error {
public:
	FastqException(const std::string& p_raison);
};

} /* namespace PHException */

#endif /* FASTQEXCEPTION_H_ */

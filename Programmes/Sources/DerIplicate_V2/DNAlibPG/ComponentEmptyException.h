/*
 * ComponentEmptyException.h
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#ifndef COMPONENTEMPTYEXCEPTION_H_
#define COMPONENTEMPTYEXCEPTION_H_

#include "FastqException.h"

namespace PHException {

class ComponentEmptyException: public FastqException {
public:
	ComponentEmptyException(const std::string& p_raison);
};

} /* namespace PHException */

#endif /* COMPONENTEMPTYEXCEPTION_H_ */

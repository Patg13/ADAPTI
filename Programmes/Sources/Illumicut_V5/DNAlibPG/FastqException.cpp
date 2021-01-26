/*
 * FastqException.cpp
 *
 *  Created on: 3 Jun 2015
 *      Author: pagagne
 */

#include "FastqException.h"

namespace PHException {

FastqException::FastqException(const std::string& p_raison):runtime_error(p_raison) {

}

} /* namespace PHException */

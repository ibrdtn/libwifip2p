/*
 * NetworkIntf.cpp
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#include "wifip2p/NetworkIntf.h"

using namespace std;

namespace wifip2p {

NetworkIntf::NetworkIntf(string name) {
	this->name = name;
}

NetworkIntf::~NetworkIntf() {
	// TODO Auto-generated destructor stub
}

string NetworkIntf::getName() {
	return this->name;
}

} /* namespace wifip2p */

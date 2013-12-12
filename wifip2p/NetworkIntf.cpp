/*
 * NetworkIntf.cpp
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#include "wifip2p/NetworkIntf.h"

namespace wifip2p
{
	NetworkIntf::NetworkIntf(const std::string &name)
	 : _name(name)
	{
	}

	NetworkIntf::~NetworkIntf()
	{
	}

	const std::string& NetworkIntf::getName() const
	{
		return _name;
	}

	bool NetworkIntf::operator==(const NetworkIntf &nic) const
	{
		return _name == nic._name;
	}
} /* namespace wifip2p */

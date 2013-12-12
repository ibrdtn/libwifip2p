/*
 * Peer.cpp
 *
 *  Created on: 08.02.2013
 *      Author: niels_w
 */

#include "wifip2p/Peer.h"

namespace wifip2p
{
	Peer::Peer(const std::string &mac)
	 : _mac_addr(mac), _name("")
	{
	}

	Peer::Peer(const std::string &mac, const std::string &name)
	 : _mac_addr(mac), _name(name)
	{
	}

	Peer::~Peer()
	{
	}

	bool Peer::operator==(const Peer &peer) const
	{
		return _mac_addr == peer._mac_addr;
	}

	const std::string& Peer::getMacAddr() const
	{
		return _mac_addr;
	}

	const std::string& Peer::getName() const
	{
		return _name;
	}

	void Peer::setName(const std::string &name)
	{
		_name = name;
	}
} /* namespace wifip2p */

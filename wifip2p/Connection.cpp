/*
 * Connection.cpp
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#include "wifip2p/Connection.h"

namespace wifip2p
{
	Connection::Connection(const NetworkIntf &nic)
	 : _peer(""), _nic(nic)
	{
	}

	Connection::Connection(const Peer &peer, const NetworkIntf &nic)
	 : _peer(peer), _nic(nic)
	{
	}

	Connection::~Connection()
	{
	}

	const Peer& Connection::getPeer() const
	{
		return _peer;
	}

	void Connection::setPeer(const Peer &peer)
	{
		_peer = peer;
	}

	const NetworkIntf& Connection::getNetworkIntf() const
	{
		return _nic;
	}
} /* namespace wifip2p */

/*
 * Connection.h
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "wifip2p/Peer.h"
#include "wifip2p/NetworkIntf.h"

namespace wifip2p
{
	class Connection {
	public:
		Connection(const NetworkIntf &nic);
		Connection(const Peer &peer, const NetworkIntf &nic);
		virtual ~Connection();

		const Peer& getPeer() const;
		void setPeer(const Peer &peer);
		const NetworkIntf& getNetworkIntf() const;

	private:
		Peer 		_peer;
		NetworkIntf _nic;

	};
} /* namespace wifip2p */
#endif /* CONNECTION_H_ */

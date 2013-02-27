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

namespace wifip2p {

class Connection {
public:
	Connection(NetworkIntf nic);
	Connection(Peer peer, NetworkIntf nic);
	virtual ~Connection();

	Peer getPeer() const;
	void setPeer(Peer peer);
	NetworkIntf getNetworkIntf() const;

private:
	Peer 		peer;
	NetworkIntf nic;

};

} /* namespace wifip2p */
#endif /* CONNECTION_H_ */

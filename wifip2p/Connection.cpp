/*
 * Connection.cpp
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#include "wifip2p/Connection.h"

namespace wifip2p {
/*
Connection::Connection(NetworkIntf nic)
	: nic(nic) {

}
*/
Connection::Connection(Peer peer, NetworkIntf nic)
	: peer(peer),
	  nic(nic) {

}

Connection::~Connection() {
	// TODO Auto-generated destructor stub
}

Peer Connection::getPeer() {
	return this->peer;
}

void Connection::setPeer(Peer peer) {
	this->peer = peer;
}

NetworkIntf Connection::getNetworkIntf() {
	return this->nic;
}

} /* namespace wifip2p */

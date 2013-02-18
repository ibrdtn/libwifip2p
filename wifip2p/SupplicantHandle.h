/*
 * SupplicantHandle.h
 *
 *  Created on: 12.12.2012
 *      Author: morgenro
 *      		niels_w
 */

#ifndef SUPPLICANTHANDLE_H_
#define SUPPLICANTHANDLE_H_

#include <list>
#include <string>

#include <iostream>
#include <sys/time.h> //FD_SET and functions...

#include "wifip2p/Peer.h"
#include "wifip2p/Connection.h"

using namespace std;

namespace wifip2p {

class SupplicantHandleException : public exception {
public:
	SupplicantHandleException(const string &what) : _what(what) {
	}

	~SupplicantHandleException() throw () {
	}

	string what() {
		return _what;
	}

private:
	const string _what;
};



class SupplicantHandle {

public:
	SupplicantHandle();
	SupplicantHandle(const char *ctrl_path, bool monitor)
		throw (SupplicantHandleException);
	virtual ~SupplicantHandle();

	void funcTest() throw (SupplicantHandleException);

	void init(string name, list<string> services) throw (SupplicantHandleException);

	//----------
	void* getHandle();
	int getFDListen();

	char* recvReply(char *replybuf, size_t reply_len);
	char* recvReply();
	//----------/

	void findPeers() throw (SupplicantHandleException);
	void findPeersStop() throw (SupplicantHandleException);
	void listen() throw (SupplicantHandleException);
	void requestService(Peer peer, string service) throw (SupplicantHandleException);
	void requestService(string service) throw (SupplicantHandleException);
	void connectToPeer(Peer peer) throw (SupplicantHandleException);
	void disconnect(Connection conn) throw (SupplicantHandleException);

private:
	bool monitor_mode;
	void *_handle;
	int fd_listen;

	bool setDeviceName(string name) throw (SupplicantHandleException);
	bool flushServices() throw (SupplicantHandleException);
	bool addService(string name, string service) throw (SupplicantHandleException);

	bool setMonitorMode() throw (SupplicantHandleException);

	bool p2pCommand(string cmd) throw (SupplicantHandleException);


};

} /* namespace wifip2p */
#endif /* SUPPLICANTHANDLE_H_ */

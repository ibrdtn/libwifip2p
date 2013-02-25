/*
 * CoreEngine.h
 *
 *  Created on: 06.02.2013
 *      Author: niels_w
 */

#ifndef COREENGINE_H_
#define COREENGINE_H_

#include <list>
#include <string>

#include "wifip2p/SupplicantHandle.h"
#include "wifip2p/WifiP2PInterface.h"
#include "wifip2p/Timer.h"
#include "wifip2p/Connection.h"
#include "wifip2p/Peer.h"
#include "wifip2p/NetworkIntf.h"

using namespace std;

namespace wifip2p {

class CoreEngineException : public exception {
public:
	CoreEngineException(const string &what) : _what(what) {
	}

	~CoreEngineException() throw () {
	}

	string what() {
		return _what;
	}

private:
	const string _what;
};


class CoreEngine {

public:
	/* Constructors, destructors >>
	 *
	 */
	CoreEngine(string ctrl_path, string name, WifiP2PInterface &ext_if);
	virtual ~CoreEngine();

	/* Member variables >>
	 *
	 */
	SupplicantHandle wpasup;
	SupplicantHandle wpamon;


	/* Methods/Member functions >>
	 *
	 */
	void run(); //really void? Or int:={-1=err, 0=okay}

	void connect(Peer peer);

	void disconnect(Connection connection);
	void disconnect(NetworkIntf nic);
	void disconnect(Peer peer);

	void setName(string name);

	void reinitialize(string ctrl_path, list<string> services) throw (CoreEngineException);

	bool addService(string service);
	bool addService(list<string> services);


	string getName();


	/* Enumeration definitions >>
	 *
	 */
	enum state { ST_IDLE,
				 ST_SCAN,
				 ST_SREQ,
				 ST_CONN };


private:
	/* Member variables >>
	 *
	 */
	string ctrl_path;
	string name;
	list<string> services;
	list<string> sdreq_id;

	list<Peer> peers;
	list<Connection> connections;

	WifiP2PInterface &ext_if;

	state actual_state;
	Timer timer;


	/* Methods/Member functions >>
	 *
	 */
	void initialize() throw (CoreEngineException);

};

} /* namespace wifip2p */

#endif /* COREENGINE_H_ */

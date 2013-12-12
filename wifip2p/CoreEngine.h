/*
 * CoreEngine.h
 *
 *  Created on: 06.02.2013
 *      Author: niels_w
 */

#ifndef COREENGINE_H_
#define COREENGINE_H_

#include <list>
#include <set>
#include <string>

#include "wifip2p/SupplicantHandle.h"
#include "wifip2p/WifiP2PInterface.h"
#include "wifip2p/Connection.h"
#include "wifip2p/Peer.h"
#include "wifip2p/NetworkIntf.h"
#include "wifip2p/Logger.h"

namespace wifip2p
{
	class CoreEngineException : public std::exception
	{
	public:
		CoreEngineException(const std::string &what)
		 : _what(what)
		{
		}

		virtual ~CoreEngineException() throw ()
		{
		}

		const std::string& what() {
			return _what;
		}

	private:
		const std::string _what;
	};

	class CoreEngine
	{
	public:

		/**
		 * Constructs a CoreEngine object initializing all variables to proper values.
		 * 	The state timer are initialized to (10, 10, 20) seconds for (st_idle,
		 * 	st_scan, st_sreq). Each of the time values may be changed independently
		 * 	of the others by using the respective setter method.
		 *
		 * @ctrl_path: string representing the path to the wpa_s ctrl_i/f domain socket.
		 * @name:	   The string representation of this devices name. This one  will
		 * 				be used to register services accordingly to the general naming
		 * 				conventions at the wpa_s. Furthermore this name will be set at the
		 * 				devices SoftMAC, if the driver allows for.
		 * @&ext_if:   Reference to the external interface implementation used for call
		 * 				backs from SupplicantHandle objects.
		 *
		 */
		CoreEngine(const std::string &ctrl_path, const std::string &name, WifiP2PInterface &ext_if, Logger &logger);

		virtual ~CoreEngine();

		SupplicantHandle wpasup;
		SupplicantHandle wpamon;

		/**
		 * Makes a CoreEngine setting up all SupplicantHandle connections, initializing
		 *  and running in the state machine's loop.
		 *
		 */
		void run();

		/**
		 * Stops a running CoreEngine.
		 *
		 */
		void stop();

		/**
		 * Enables to connect to a specifically known peer.
		 *
		 * @peer: The Peer object to connect to.
		 *
		 */
		void connect(Peer peer);

		/**
		 * Cancel a specifically known connection per respective Connection object.
		 *
		 * @connection: The Connection objects data required for canceling the connection.
		 *
		 */
		void disconnect(Connection connection);

		/**
		 * Iterates over all the yet listed connections searching for one with the respective
		 * 	(virtual) network interface name. If found: disconnect this, else do nothing.
		 *
		 * @nic: The to be removed network interface.
		 *
		 */
		void disconnect(const NetworkIntf &nic);

		/**
		 * Iterates over all the yet listed connections searching for one with the respective
		 * 	peer's MAC address. If found: disconnect this, else do nothing.
		 *
		 * @peer: The peer a running connection to should be quit.
		 *
		 */
		void disconnect(const Peer &peer);

		/**
		 * TODO Verify that calling ::initialize()::open() on already opened _handle
		 *  connections is not leading to any error.
		 *  Maybe to define and implement a method *terminate()*, closing all the connection
		 *  of the respective wpasup and wpamon _handles.
		 */
		void reinitialize(const std::string &ctrl_path, const std::list<std::string> &services) throw (CoreEngineException);

		void addService(const std::string &service);

		void setName(const std::string &name);
		const std::string& getName() const;

		void setTime_ST_IDLE(int s);
		const int getTime_ST_IDLE() const;
		void setTime_ST_SCAN(int s);
		const int getTime_ST_SCAN() const;
		void setTime_ST_SREQ(int s);
		const int getTime_ST_SREQ() const;

		enum state {
			ST_IDLE,
			ST_SCAN,
			ST_SREQ
		};

	private:
		static const std::string TAG;

		std::string _ctrl_path;
		std::string _name;
		std::list<std::string> _services;
		std::set<std::string> _sdreq_id;

		state _actual_state;
		int _st_idle_time,
			_st_scan_time,
			_st_sreq_time;

		bool _running;

		int _pipe_fds[2];

		std::list<Connection> _connections;

		WifiP2PInterface &_ext_if;
		Logger &_logger;

		void initialize() throw (CoreEngineException);

		/**
		 * This method observes a fixed set of file descriptors per blocking ::select
		 * 	in order to catch event messages created at wpa_s and escalated to the
		 * 	respectively attached SupplicantHandle.
		 * 	The method will remain in ::select for a to be defined number of seconds.
		 *
		 * @&wpa:	 Reference to the SupplicantHandle whichs wpa_s control_i/f connection
		 * 			  file descriptor will be monitored by blocking ::select for incoming
		 * 			  event messages.
		 * @seconds: Time value for how long the method will remain in blocking ::select.
		 * @next:	 The next CoreEngine state to be entered after leaving ::select.
		 *
		 */
		void triggeredEvents(const SupplicantHandle &wpa, std::list<Peer> &peers, int seconds, state next);

	};
} /* namespace wifip2p */

#endif /* COREENGINE_H_ */

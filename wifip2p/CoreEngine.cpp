/*
 * CoreEngine.cpp
 *
 *  Created on: 06.02.2013
 *      Author: niels_w
 */

#include "wifip2p/CoreEngine.h"
#include <sys/socket.h>
#include <unistd.h>

namespace wifip2p
{
	const std::string CoreEngine::TAG = "CoreEngine";

	CoreEngine::CoreEngine(const std::string &ctrl_path, const std::string &name, WifiP2PInterface &ext_if, Logger &logger)
	 : wpasup(false, logger), wpamon(true, logger), _actual_state(ST_IDLE), _st_idle_time(10), _st_scan_time(10),
	   _st_sreq_time(20), _running(false), _ext_if(ext_if), _logger(logger), _name(name), _ctrl_path(ctrl_path)
	{
		::pipe(_pipe_fds);
	}

	CoreEngine::~CoreEngine()
	{
		::close(_pipe_fds[0]);
		::close(_pipe_fds[1]);
	}

	void CoreEngine::stop()
	{
		_running = false;
		char buf[1] = { '\0' };
		::write(_pipe_fds[1], buf, 1);
	}

	void CoreEngine::run()
	{
		wpasup.open(_ctrl_path.c_str());
		wpamon.open(_ctrl_path.c_str());
		wpasup.init(_name, _services);

		std::list<Peer> peers;

		_running = true;

		while (_running) {

			switch(_actual_state) {

			case ST_IDLE: {

				_logger.log_debug(40, TAG, "[ENTERED_STATE := IDLE_STATE]");

				triggeredEvents(wpamon, peers, _st_idle_time, ST_SCAN);

				break;
			}

			case ST_SCAN: {

				_logger.log_debug(40, TAG, "[ENTERED_STATE := SCAN_STATE]");

				wpasup.findPeers();

				triggeredEvents(wpamon, peers, _st_scan_time, ST_SCAN);

				wpasup.findPeersStop();

				if (!peers.empty()) {

					bool all_known = true;

					for (std::list<Peer>::iterator peer_it = peers.begin(); peer_it != peers.end(); ++peer_it) {
						if (peer_it->getName() == "") {
							all_known = false;
							break;
						} else {
							continue;
						}
					}

					if (all_known) {
						_actual_state = ST_IDLE;
					} else {
						_actual_state = ST_SREQ;
					}

				} else {
					_actual_state = ST_IDLE;
				}

				break;
			}

			case ST_SREQ: {

				_logger.log_debug(40, TAG, "[ENTERED_STATE := SD_REQUEST_STATE]");


				for (std::list<std::string>::const_iterator it = _services.begin(); it != _services.end(); ++it)
					wpasup.requestService(*it, _sdreq_id);

				wpasup.findPeers();

				triggeredEvents(wpamon, peers, _st_sreq_time, ST_SREQ);

				for (std::set<std::string>::const_iterator jt = _sdreq_id.begin(); jt != _sdreq_id.end(); ++jt)
					wpasup.requestServiceCancel(*jt);
				_sdreq_id.clear();
				wpasup.findPeersStop();

				_actual_state = ST_IDLE;

				break;
			}

			default:

				break;

			}
		}

		try {
			wpasup.flushServices();
		} catch (SupplicantHandleException &ex) {
			_logger.log_err(TAG, "Services could not have been unregistered due to some exception raised: " + ex.what());
		}
	}

	void CoreEngine::connect(wifip2p::Peer peer)
	{
		try {
			wpasup.connectToPeer(peer);
		} catch (SupplicantHandleException &ex) {
			_logger.log_err(TAG, "Unable to connect to peer (" + peer.getMacAddr()
					+ "; " + peer.getName() + ") due to some exception raised: " + ex.what());
		}
	}

	void CoreEngine::disconnect(wifip2p::Connection connection)
	{
		wpasup.disconnect(connection);
	}

	void CoreEngine::disconnect(const wifip2p::NetworkIntf &nic)
	{
		for (std::list<Connection>::iterator conn_it = _connections.begin(); conn_it != _connections.end(); ++conn_it)
		{
			if (conn_it->getNetworkIntf() == nic) {
				disconnect(*conn_it);
			}
		}
	}

	void CoreEngine::disconnect(const wifip2p::Peer &peer)
	{
		std::list<Connection>::iterator conn_it = _connections.begin();

		for (; conn_it != _connections.end(); ++conn_it) {
			if (conn_it->getPeer() == peer) {
				disconnect(*conn_it);
			}
		}
	}

	void CoreEngine::initialize() throw (CoreEngineException)
	{
		try {
			wpasup.open(_ctrl_path.c_str());
			wpamon.open(_ctrl_path.c_str());
		} catch (SupplicantHandleException &ex) {
			throw CoreEngineException("Error initializing handles: " + ex.what());
		}

		_actual_state = ST_IDLE;
	}

	void CoreEngine::reinitialize(const std::string &ctrl_path, const std::list<std::string> &services) throw (CoreEngineException)
	{
		_ctrl_path = ctrl_path;
		_services = services;
		try {
			initialize();
		} catch (CoreEngineException &ex) {
			throw CoreEngineException(ex.what());
		}
	}

	void CoreEngine::triggeredEvents(const SupplicantHandle &wpa, std::list<Peer> &peers, int seconds, state next)
	{
		fd_set fds;
		int wpa_fd = wpa.getFD();

		if (wpa_fd <= 0) return;

		FD_ZERO(&fds);
		FD_SET(wpa_fd, &fds);
		FD_SET(_pipe_fds[0], &fds);

		int high_fd = wpa_fd;

		struct timeval tv;
		tv.tv_sec = seconds;
		tv.tv_usec = 0;

		if (high_fd < _pipe_fds[0]) {
			high_fd = _pipe_fds[0];
		}

		while (::select(high_fd + 1, &fds, NULL, NULL, &tv) != 0)
		{
			if (FD_ISSET(wpa_fd, &fds)) {
				wpamon.listen(peers, _connections, _services, _sdreq_id, _ext_if);
			}

			if (FD_ISSET(_pipe_fds[0], &fds)) break;

			FD_SET(wpa_fd, &fds);
			FD_SET(_pipe_fds[0], &fds);
		}

		_actual_state = next;
	}

	void CoreEngine::setName(const std::string &name)
	{
		_name = name;
		initialize();
	}

	const std::string& CoreEngine::getName() const
	{
		return _name;
	}

	void CoreEngine::setTime_ST_IDLE(int s)
	{
		_st_idle_time = s;
	}

	const int CoreEngine::getTime_ST_IDLE() const
	{
		return _st_idle_time;
	}

	void CoreEngine::setTime_ST_SCAN(int s)
	{
		_st_scan_time = s;
	}

	const int CoreEngine::getTime_ST_SCAN() const
	{
		return _st_scan_time;
	}

	void CoreEngine::setTime_ST_SREQ(int s)
	{
		_st_sreq_time = s;
	}

	const int CoreEngine::getTime_ST_SREQ() const
	{
		return _st_sreq_time;
	}

	void CoreEngine::addService(const std::string &service)
	{
		_services.push_back(service);
	}
} /* namespace wifip2p */

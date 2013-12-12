/*
 * Peer.h
 *
 *  Created on: 08.02.2013
 *      Author: niels_w
 */

#include <iostream>
#include <list>
#include <string>

#ifndef PEER_H_
#define PEER_H_

namespace wifip2p
{
	class PeerException : public std::exception
	{
	public:
		PeerException(const std::string &what) : _what(what)
		{
		}

		virtual ~PeerException() throw ()
		{
		}

		const std::string &what()
		{
			return _what;
		}

	private:
		const std::string _what;
	};

	class Peer
	{
	public:
		Peer(const std::string &mac);
		Peer(const std::string &mac, const std::string &name);
		virtual ~Peer();

		const std::string& getMacAddr() const;
		const std::string& getName() const;
		void setName(const std::string &name);

		/**
		 * Matches the actual peer against the input parameter peer.
		 * Equality is explicitly defined as matching MAC addresses; as
		 *  a peer's name is not of any use regarding this issue.
		 *
		 * @peer:  peer to be checked for equality.
		 * Return: true or false, whether this = peer.
		 *
		 */
		bool operator==(const Peer &peer) const;

	private:
		std::string _mac_addr;
		std::string _name;
	};
} /* namespace wifip2p */
#endif /* PEER_H_ */

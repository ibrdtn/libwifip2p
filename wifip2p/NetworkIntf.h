/*
 * NetworkIntf.h
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#ifndef NETWORKINTF_H_
#define NETWORKINTF_H_

#include <string>

namespace wifip2p
{
	class NetworkIntf {
	public:
		NetworkIntf(const std::string &name);
		virtual ~NetworkIntf();

		const std::string &getName() const;

		/**
		 * Matches the actual nic against the input parameter
		 * 	nic, which is a object-reference.
		 * Equality is defined as matching interface names.
		 *
		 * @nic:   The nic object to be tested for equality.
		 * Return: True, whether both of the NetworkIntf objects'
		 * 			names are equal or not.
		 *
		 */
		bool operator==(const NetworkIntf &nic) const;

	private:
		const std::string _name;
	};
} /* namespace wifip2p */
#endif /* NETWORKINTF_H_ */

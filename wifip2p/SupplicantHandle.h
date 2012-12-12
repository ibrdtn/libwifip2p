/*
 * SupplicantHandle.h
 *
 *  Created on: 12.12.2012
 *      Author: morgenro
 */

#ifndef SUPPLICANTHANDLE_H_
#define SUPPLICANTHANDLE_H_

namespace wifip2p {

	class SupplicantHandle {
	public:
		SupplicantHandle(const char *ctrl_path);
		virtual ~SupplicantHandle();

	private:
		void *_handle;
	};

} /* namespace wifip2p */
#endif /* SUPPLICANTHANDLE_H_ */

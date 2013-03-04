/*
 * Timer.h
 *
 *  Created on: 15.02.2013
 *      Author: niels_w
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <ctime>

namespace wifip2p {

class Timer {
public:
	Timer();
	virtual ~Timer();

	void start();
	bool isRunning();
	void setTimer(int m, int s);
	bool timeout();

private:
	int minutes;
	int seconds;

	time_t stoptime;

	bool running;

};

} /* namespace wifip2p */
#endif /* TIMER_H_ */

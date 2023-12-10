#ifndef CTIMER_H_
#define CTIMER_H_

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>
#include "SmokeDetector.h"

class cTimer {

	int connection_id;

	struct sigevent sig_event;
	struct itimerspec timer_spec;
	timer_t timer_id;



	uint64_t cycles_per_sec;
	uint64_t tick_cycles, tock_cycles;
public:
	int channel_id;
	char msg_buffer[100];
	cTimer(uint32_t,uint32_t);
	sigevent sig_event_cust;

	void setTimerSpec(uint32_t,uint32_t);
	void waitTimer();
	void startTimer();
	void tick();
	double tock();
	virtual ~cTimer();
};

#endif /* CTIMER_H_ */

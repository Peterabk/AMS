/*
 * SmokeDetector.h
 *
 *  Created on: Dec 7, 2023
 *      Author: peter
 */

#ifndef SMOKEDETECTOR_H_
#define SMOKEDETECTOR_H_

#include "cTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <csignal>

class SmokeDetector {

	int signum;
	std::string th_name;
	void *ptr;

	pthread_mutex_t *mutex;
	pthread_cond_t condit;

	friend void * start_routine_smoke(void* arg);
	struct sigevent sig_event;

	void *smokeptr;

public:
	int done;
	pthread_t thread_id;
	void initialize_smokedetector();
	void smokeInterrupt(SmokeDetector *smoke);
	void signalHandler();

	SmokeDetector(pthread_mutex_t*);

	virtual ~SmokeDetector();
};

#endif /* SMOKEDETECTOR_H_ */

/*
 * Dial.h
 *
 *  Created on: Dec 4, 2023
 *      Author: peter
 */

#ifndef DIAL_H_
#define DIAL_H_

#include "cTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <queue>
#include <sys/mman.h>


#define SIZE 4096

class Dial {
	pthread_mutex_t mutex;
	int period_sec; //seconds
	int period_msec;//milliseconds
	float safe_value;
	std::string Dial_name;

	std::queue<float> ofr_value;

	friend void * Dial_start_routine(void* arg);

	struct Lamp {
		std::string Lamp_status = "GREEN";
	};

	const char *name = "/my_shm";

	int shm_fd;
	void *ptr;

public:
	pthread_t thread_id;
	struct Lamp lamp;
	void * threadTask(void * );
	Dial(std::string name,int period_sec,int period_msec, float Safe_Value);
	~Dial();
	void initialize_reader();
	void manage_ofr_queue(float* ptr_value, std::string dialname);
	bool is_queue_ofr();
	void read(Dial dial);
};

#endif /* DIAL_H_ */

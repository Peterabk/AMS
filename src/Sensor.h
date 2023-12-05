#ifndef SENSOR_H_
#define SENSOR_H_

#include "cTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>


#define SIZE 4096


class Sensor {
	pthread_mutex_t mutex;
	std::string th_name;
	int period_sec; //seconds
	int period_msec;//milliseconds
	int upper_bound;
	int map_offset;
	friend void * start_routine(void* arg, float random);

	const char *name = "/my_shm";

	int shm_fd;

	void *ptr;

public:
	pthread_t thread_id;
	Sensor(std::string name,int period_sec,int period_msec,int upper_bound, int offset);
	~Sensor();
	void initialize_sensor();
	void sensorpoll(Sensor sensor);
	float generate_random();

};

#endif /* SENSOR_H_ */

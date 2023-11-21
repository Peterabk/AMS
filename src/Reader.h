#ifndef READER_H_
#define READER_H_

#include "cTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>


#define SIZE 4096

class Reader {
	pthread_mutex_t mutex;
	int period_sec; //seconds
	int period_msec;//milliseconds
	friend void * start_routine(void* arg);

	const char *name = "/my_shm";

	int shm_fd;
	void *ptr;

public:
	pthread_t thread_id;
	void * threadTask(void * );
	Reader(int period_sec,int period_msec);
	~Reader();
	void initialize_reader();
	void read();
};

#endif /* WRITER_H_ */
